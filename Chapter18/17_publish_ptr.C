#include <atomic>
#include <thread>
#include <iostream>

struct Data {
    int n;
    double x;
};

std::atomic<Data*> data;

void produce() {
    Data* p = new Data;
    p->n = 1 << 20;
    p->x = 0;
    // Compute pi using n iterations.
    for (int i = 0, d = 1; i != p->n; ++i, d += 4) {
        p->x += (4.0/d - 4.0/(d + 2));
    }
    data.store(p, std::memory_order_release);
}

void consume(int i, double& res) {
    Data* p = nullptr;
    while (!(p = data.load(std::memory_order_acquire))) {}
    res = p->x*i;       // "Work"
}

int main() {
    double res1 = 0, res2 = 0;
    // Launch consumers, they will wait.
    std::thread t1(consume, 1, std::ref(res1));
    std::thread t2(consume, 2, std::ref(res2));
    // Launch producer.
    std::thread t3(produce);
    // Wait for consumers to finish.
    t1.join();
    t2.join();
    std::cout << res1 << " " << res2 << std::endl;
    // Join producer (it must be done already).
    t3.join();
    delete data;
}

