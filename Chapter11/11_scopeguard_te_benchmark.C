#include <functional>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

struct CommitFlag {
  mutable bool commit_ = false;
  CommitFlag() = default;
  CommitFlag(CommitFlag&& other) noexcept : commit_(std::exchange(other.commit_, true)) {}
  void commit() const noexcept { commit_ = true; }
};

namespace ScopeGuardTypeErased {
class ScopeGuard : public CommitFlag {
  public:
  template <typename Callable> ScopeGuard(Callable&& callable) : callable_(std::forward<Callable>(callable)) {}
  ~ScopeGuard() { if (!commit_) callable_(); }
  ScopeGuard(ScopeGuard&& other) = default;
  private:
  std::function<void()> callable_;
  ScopeGuard& operator=(const ScopeGuard&) = delete;
};
}

namespace ScopeGuardTypeErasedFast {
template <size_t S = 16>
class ScopeGuard : public CommitFlag {
  alignas(8) char space_[S];
  using guard_t = void(*)(void*);
  guard_t guard_ = nullptr;
  template<typename Callable> static void invoke(void* callable) {
    (*static_cast<Callable*>(callable))();
  }
  mutable bool commit_ = false;
  public:
  template <typename Callable, typename D = std::decay_t<Callable>> ScopeGuard(Callable&& callable) :
    guard_(invoke<Callable>)
  {
    static_assert(sizeof(Callable) <= sizeof(space_));
    static_assert(std::is_trivially_destructible_v<Callable>);
    static_assert(std::is_trivially_copyable_v<Callable>);
    ::new(static_cast<void*>(space_)) D(std::forward<Callable>(callable));
  }
  ScopeGuard(ScopeGuard&& other) = default;
  ~ScopeGuard() { if (!commit_) guard_(space_); }
};
}

namespace ScopeGuardTemplate {
class ScopeGuardBase : public CommitFlag {
  public:
  ScopeGuardBase() {}

  protected:
  ScopeGuardBase(ScopeGuardBase&& other) = default;
  ~ScopeGuardBase() {}

  private:
  ScopeGuardBase& operator=(const ScopeGuardBase&) = delete;
};

template <typename Callable>
class ScopeGuard : public ScopeGuardBase {
  public:
  ScopeGuard(Callable&& callable) : callable_(std::move(callable)) {}
  ScopeGuard(const Callable& callable) : callable_(callable) {}
  ~ScopeGuard() { if (!commit_) callable_(); }
  ScopeGuard(ScopeGuard&& other) = default;
  private:
  Callable callable_;
};

template <typename Callable>
ScopeGuard<Callable> MakeGuard(Callable&& callable) {
  return ScopeGuard<Callable>(std::forward<Callable>(callable));
}
}

void BM_noop_explicit(benchmark::State& state) {
  int* p = rand() < 0 ? new int(42) : nullptr;
  for (auto _ : state) {
    delete p;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_noop_type_erased(benchmark::State& state) {
  int* p = rand() < 0 ? new int(42) : nullptr;
  for (auto _ : state) {
    ScopeGuardTypeErased::ScopeGuard SG([&] { delete p; });
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_noop_type_erased_fast(benchmark::State& state) {
  int* p = rand() < 0 ? new int(42) : nullptr;
  for (auto _ : state) {
    ScopeGuardTypeErasedFast::ScopeGuard<> SG([&] { delete p; });
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_noop_template(benchmark::State& state) {
  int* p = rand() < 0 ? new int(42) : nullptr;
  for (auto _ : state) {
    auto SG = ScopeGuardTemplate::MakeGuard([&] { delete p; });
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete_explicit(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    p = rand() < 0 ? new int(42) : nullptr;
    delete p;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete_type_erased(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    ScopeGuardTypeErased::ScopeGuard SG([&] { delete p; });
    p = rand() < 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete_type_erased_fast(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    ScopeGuardTypeErasedFast::ScopeGuard<> SG([&] { delete p; });
    p = rand() < 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete_template(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    auto SG = ScopeGuardTemplate::MakeGuard([&] { delete p; });
    p = rand() < 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete2_explicit(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    int* q = nullptr;
    p = rand() < 0 ? new int(42) : nullptr;
    q = rand() < 0 ? new int(7) : nullptr;
    delete p;
    delete q;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete2_type_erased(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    int* q = nullptr;
    ScopeGuardTypeErased::ScopeGuard SG([&] { delete p; delete q; });
    p = rand() < 0 ? new int(42) : nullptr;
    q = rand() < 0 ? new int(7) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete2_type_erased_fast(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    int* q = nullptr;
    ScopeGuardTypeErasedFast::ScopeGuard<> SG([&] { delete p; delete q; });
    p = rand() < 0 ? new int(42) : nullptr;
    q = rand() < 0 ? new int(7) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_nodelete2_template(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    int* q = nullptr;
    auto SG = ScopeGuardTemplate::MakeGuard([&] { delete p; delete q; });
    p = rand() < 0 ? new int(42) : nullptr;
    q = rand() < 0 ? new int(7) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_delete_explicit(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    p = rand() >= 0 ? new int(42) : nullptr;
    delete p;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_delete_type_erased(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    ScopeGuardTypeErased::ScopeGuard SG([&] { delete p; });
    p = rand() >= 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_delete_type_erased_fast(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    ScopeGuardTypeErasedFast::ScopeGuard<> SG([&] { delete p; });
    p = rand() >= 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

void BM_delete_template(benchmark::State& state) {
  for (auto _ : state) {
    int* p = nullptr;
    auto SG = ScopeGuardTemplate::MakeGuard([&] { delete p; });
    p = rand() >= 0 ? new int(42) : nullptr;
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_noop_explicit);
BENCHMARK(BM_noop_type_erased);
BENCHMARK(BM_noop_type_erased_fast);
BENCHMARK(BM_noop_template);
BENCHMARK(BM_nodelete_explicit);
BENCHMARK(BM_nodelete_type_erased);
BENCHMARK(BM_nodelete_type_erased_fast);
BENCHMARK(BM_nodelete_template);
BENCHMARK(BM_nodelete2_explicit);
BENCHMARK(BM_nodelete2_type_erased);
BENCHMARK(BM_nodelete2_type_erased_fast);
BENCHMARK(BM_nodelete2_template);
BENCHMARK(BM_delete_explicit);
BENCHMARK(BM_delete_type_erased);
BENCHMARK(BM_delete_type_erased_fast);
BENCHMARK(BM_delete_template);

BENCHMARK_MAIN();
