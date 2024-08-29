#include <atomic>
#include <thread>
#include <iostream>
#include <condition_variable>

struct Data {
    int n {};
    double x {};
};

std::mutex m;
std::condition_variable cv;
Data data {};

void produce() {
    {
        std::lock_guard l(m);
        data.n = 1 << 20;
        data.x = 0;
        // Compute pi using n iterations.
        for (int i = 0, d = 1; i != data.n; ++i, d += 4) {
            data.x += (4.0/d - 4.0/(d + 2));
        }
    }
    cv.notify_one(); // Wake up the main thread - work is done
}

int main() {
    std::unique_lock l(m);
    // Launch producer.
    std::thread t(produce);
    // Wait for the work to be done.
    cv.wait(l, []{ return data.n != 0; });
    std::cout << data.n << " " << data.x << std::endl;
    // Join producer (it must be done already).
    t.join();
}
