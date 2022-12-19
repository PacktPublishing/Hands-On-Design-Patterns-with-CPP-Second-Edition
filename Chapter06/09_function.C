#include <iostream>
#include <string>
#include <string.h>
#include <utility>
#include <type_traits>
#include <new>
#include <typeinfo>
#include <functional>
#include <algorithm>

#define REQUIRES(...) std::enable_if_t<__VA_ARGS__, int> = 0

template<typename Signature, size_t Size = sizeof(void*)*2, size_t Alignment = alignof(void*)> class Function;

template<size_t Size, size_t Alignment, typename Res, typename... Args>
class Function<Res(Args...), Size, Alignment>
{
    alignas(Alignment) char space_[Size];
    using executor_t = Res(*)(Args..., void*);
    static constexpr Res default_executor(Args..., void*) { throw std::bad_function_call(); }
    constexpr static executor_t default_executor_ = default_executor;
    executor_t executor_ = default_executor_;

    template<typename Callable> static Res executor(Args... args, void* this_function) {
        static_assert(!std::is_member_function_pointer<Callable>::value, "");
        return (*reinterpret_cast<Callable*>(static_cast<Function*>(this_function)->space_))(std::forward<Args>(args)...);
    }

public:
    // Default-constructed function is null (invocation will throw).
    Function() = default;

    // Construct a function from a callable.
    // The callable must be trivially copyable and trivially destructible.
    // The callable must not exceed the size and alignment of this function.
    template <typename CallableArg, typename Callable = std::decay_t<CallableArg> 
        //, REQUIRES(!std::is_same<Function, Callable>::value)
        >
        requires(!std::same_as<Function, Callable>)
    Function(CallableArg&& callable) : 
        executor_(executor<Callable>)
    {
        static_assert(sizeof(Callable) <= Size, "Callable size is too large, increase size of the space type");
        static_assert(alignof(Callable) <= Alignment, "Callable alignment is too large, increase alignment of the space type");
        static_assert(std::is_trivially_destructible<Callable>::value, "Use Function for non-trivially-destructible callables");
        static_assert(std::is_trivially_copyable<Callable>::value, "Use Function for non-trivially-copyable callables");
        ::new (static_cast<void*>(space_)) Callable(std::forward<CallableArg>(callable));
    }

    // The call operator invokes the callable.
    template <typename... CallArgs>
    Res operator()(CallArgs&&... callargs) const {
        return this->executor_(std::forward<CallArgs>(callargs)..., const_cast<Function*>(this));
    }

}; // Function
template<size_t Size, size_t Alignment, typename Res, typename... Args>
constexpr typename Function<Res(Args...), Size, Alignment>::executor_t Function<Res(Args...), Size, Alignment>::default_executor_;

using F = int(int, int, int, int);

using FF = Function<F>;
using SF = std::function<F>;

#ifdef ASM
//auto invoke_f(int a, int b, int c, int d, const F& f) { return f(a, b, c, d); }
//auto invoke_ff(int a, int b, int c, int d, const FF& f) { return f(a, b, c, d); }
auto invoke_sf(int a, int b, int c, int d, const SF& f) { return f(a, b, c, d); }
#else 
int fi(int a, int b, int c, int d) { return a + b + c + d; }
__attribute__ ((noinline)) int f(int a, int b, int c, int d) { return a + b + c + d; }

template <typename F> auto invoke(int a, int b, int c, int d, const F& f) { return f(a, b, c, d); }

#include "benchmark/benchmark.h"

void BM_invoke_function_inline(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, fi));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_function(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, f));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_fast_function(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    FF ff(f);
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, ff));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_std_function(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    SF sf(f);
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, sf));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    auto lf = [&](int a, int b, int c, int d) { return x + a + b + c + d; };
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, lf));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_fast_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    FF ff{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, ff));
        benchmark::ClobberMemory();
    }
}

void BM_invoke_std_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    SF sf{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        benchmark::DoNotOptimize(invoke(a, b, c, d, sf));
        benchmark::ClobberMemory();
    }
}

void BM_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    auto f = [&](int a, int b, int c, int d) { return x + a + b + c + d; };
    for (auto _ : state) {
        benchmark::DoNotOptimize(f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

void BM_fast_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    FF f{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        benchmark::DoNotOptimize(f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

void BM_std_lambda(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    SF f{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        benchmark::DoNotOptimize(f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

class Base {
    public:
    virtual ~Base() {}
    virtual int f(int a, int b, int c, int d) { return a + b + c + d; };
};

class Derived : public Base {
    int x = 0;
    public:
    Derived(int x) : x(x) {}
    virtual int f(int a, int b, int c, int d) override { return x + a + b + c + d; };
};

void BM_invoke_virtual(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    Base* p = rand() > 0 ? new Derived(x) : new Base;
    for (auto _ : state) {
        benchmark::DoNotOptimize(p->f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_invoke_function_inline);
BENCHMARK(BM_invoke_function);
BENCHMARK(BM_invoke_fast_function);
BENCHMARK(BM_invoke_std_function);
BENCHMARK(BM_invoke_virtual);

BENCHMARK(BM_invoke_lambda);
BENCHMARK(BM_invoke_fast_lambda);
BENCHMARK(BM_invoke_std_lambda);

BENCHMARK(BM_lambda);
BENCHMARK(BM_fast_lambda);
BENCHMARK(BM_std_lambda);

BENCHMARK_MAIN();
#endif // ASM 
