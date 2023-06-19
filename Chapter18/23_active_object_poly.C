#include <thread>
#include <iostream>

class Job {
    std::thread t_;
    bool done_ {};
    virtual void operator()() = 0;
    public:
    void wait() {
        if (done_) return;
        t_.join(); 
        done_ = true;
    }
    void run() {
        t_ = std::thread([this](){ (*this)(); });
    }
    virtual ~Job() { wait(); }
};

class PiJob final : public Job {
    const size_t n_;
    double x_ {};
    void operator()() override {
        // Compute pi using n_ iterations.
        for (size_t i = 0, d = 1; i != n_; ++i, d += 4) {
            x_ += (4.0/d - 4.0/(d + 2));
        }
    }
    public:
    explicit PiJob(size_t n) : n_(n) { this->run(); }
    double get() { this->wait(); return x_; }
};

int main() {
    PiJob j1(1 << 16);
    PiJob j2(1 << 10);
    std::cout << j1.get() << " " << j2.get() << std::endl;
}
