#include <stdlib.h>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

void BM_malloc(benchmark::State& state) {
    constexpr size_t size = 1024;
    for (auto _ : state) {
        void* p = malloc(size);
        benchmark::DoNotOptimize(p);
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_increment(benchmark::State& state) {
    size_t i = 0;
    for (auto _ : state) {
        ++i;
        benchmark::DoNotOptimize(i);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_increment32(benchmark::State& state) {
    size_t i = 0;
    for (auto _ : state) {
        REPEAT(
            ++i;
            benchmark::DoNotOptimize(i);
        )
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK(BM_malloc);
BENCHMARK(BM_increment);
BENCHMARK(BM_increment32);

BENCHMARK_MAIN();

