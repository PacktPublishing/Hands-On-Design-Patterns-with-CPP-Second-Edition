#include <stdlib.h>
#include <iostream>
#include <memory>

#include "benchmark/benchmark.h"

#define REPEAT2(x) {x} {x}
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT64(x) REPEAT32(x) REPEAT32(x)
#define REPEAT(x) REPEAT64(x)

class Base {
    protected:
    int i = 0;
    public:
    virtual ~Base() {}
    virtual int f() { return ++i; }
};

class Derived : public Base {
    int f() override { return --i; }
};

void BM_virtual(benchmark::State& state) {
    std::unique_ptr<Base> p((rand() > 0) ? new Derived : new Base);
    for (auto _ : state) {
        REPEAT(p->f();)
    }
    state.SetItemsProcessed(64*state.iterations());
}

void BM_dynamic_cast(benchmark::State& state) {
    std::unique_ptr<Base> p((rand() > 0) ? new Derived : new Base);
    for (auto _ : state) {
        REPEAT({
        Derived* q = dynamic_cast<Derived*>(p.get());
        benchmark::DoNotOptimize(q);
        })
    }
    state.SetItemsProcessed(64*state.iterations());
}

BENCHMARK(BM_virtual);
BENCHMARK(BM_dynamic_cast);

BENCHMARK_MAIN();

