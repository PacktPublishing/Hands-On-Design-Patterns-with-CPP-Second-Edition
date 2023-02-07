#include <stdlib.h>
#include <queue>

#include "benchmark/benchmark.h"

class small_queue {
    public:
    small_queue() = default;
    ~small_queue() = default;
    bool push(int i) {
        if (front_ - tail_ > buf_size_) return false;
        buf_[(++front_) & (buf_size_ - 1)] = i;
        return true;
    }
    int front() const {
        return buf_[tail_ & (buf_size_ - 1)];
    }
    void pop() { ++tail_; }
    size_t size() const { return front_ - tail_; }
    bool empty() const { return front_ == tail_; }
    private:
    static constexpr size_t buf_size_ = 16;
    static_assert((buf_size_ & (buf_size_ - 1)) == 0, "Buffer size must be a power of 2");
    int buf_[buf_size_];
    size_t front_ = 0;
    size_t tail_ = 0;
};

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

template <typename T>
void BM_queue(benchmark::State& state) {
    T q;
    for (auto _ : state) {
        REPEAT({
            for (int i = 0; i < 7; ++i) q.push(i);
            while (!q.empty()) {
                benchmark::DoNotOptimize(q.front());
                q.pop();
            }
        });
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK_TEMPLATE1(BM_queue, std::queue<int>);
BENCHMARK_TEMPLATE1(BM_queue, small_queue);

BENCHMARK_MAIN();
