#include "07_smartptr.h"

#include "benchmark/benchmark.h"

void BM_rawptr(benchmark::State& state) {
    for (auto _ : state) {
        int* p(get_raw_ptr());
        d(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_uniqueptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_unique_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_sharedptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_shared_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_make_sharedptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(make_shared_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_lb(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te_lb());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_lb0(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te_lb0());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_static(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te_static());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_vtable(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te_vtable());
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_rawptr);
BENCHMARK(BM_uniqueptr);
BENCHMARK(BM_sharedptr);
BENCHMARK(BM_make_sharedptr);
BENCHMARK(BM_smartptr);
BENCHMARK(BM_smartptr_te);
BENCHMARK(BM_smartptr_te_lb);
BENCHMARK(BM_smartptr_te_lb0);
BENCHMARK(BM_smartptr_te_static);
BENCHMARK(BM_smartptr_te_vtable);

BENCHMARK_MAIN();

