#include <thread>
#include <functional>
#include <iostream>

class Job {
    bool done_ {};
    std::function<void()> f_;
    std::thread t_;
    public:
    template <typename F> explicit Job(F&& f) : f_(f), t_(f_) {}
    void wait() {
        if (done_) return;
        t_.join(); 
        done_ = true;
    }
    ~Job() { wait(); }
};

class PiJob {
    const size_t n_;
    double& x_;
    public:
    PiJob(size_t n, double& x) : n_(n), x_(x) {}
    void operator()() {
        // Compute pi using n_ iterations.
        for (size_t i = 0, d = 1; i != n_; ++i, d += 4) {
            x_ += (4.0/d - 4.0/(d + 2));
        }
    }
};

int main() {
    double res1 = 0, res2 = 0;
    {
    Job j1(PiJob(1 << 16, res1));
    Job j2(PiJob(1 << 10, res2));
    }
    std::cout << res1 << " " << res2 << std::endl;
}
