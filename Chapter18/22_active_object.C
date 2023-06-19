#include <thread>
#include <iostream>

class Job {
    const size_t n_;
    double x_ {};
    std::thread t_;
    bool done_ {};
    public:
    explicit Job(size_t n) : n_(n) {}
    void operator()() {
        t_ = std::thread([this]()
            {
                // Compute pi using n_ iterations.
                for (size_t i = 0, d = 1; i != n_; ++i, d += 4) {
                    x_ += (4.0/d - 4.0/(d + 2));
                }
            }
        );
    }
    void wait() {
        if (done_) return;
        t_.join(); 
        done_ = true;
    }
    ~Job() { wait(); }
    double get() { this->wait(); return x_; }
};

int main() {
    Job j1(1 << 16); j1();
    Job j2(1 << 10); j2();
    std::cout << j1.get() << " " << j2.get() << std::endl;
}
