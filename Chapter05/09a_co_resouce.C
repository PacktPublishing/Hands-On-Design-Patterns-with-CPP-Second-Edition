#include <cstddef>
#include <iostream>

#include <mutex>
std::mutex m;
const bool mt_run = true;

class lock_guard {
  std::mutex& m_;
  const bool log_;
  const bool mt_;

  public:
  lock_guard(std::mutex& m, bool log, bool mt) : m_(m), log_(log), mt_(mt) {
    if (log_) std::cout << "Before locking" << std::endl;
    if (mt_) m.lock();
  }

  ~lock_guard() {
    if (mt_) m.unlock();
    if (log_) std::cout << "After locking" << std::endl;
  }
};

int main(int argc, char** argv) {
  try {
    lock_guard lg(m, true, mt_run);
    if (argc > 1) throw 0;
    std::cout << "Work" << std::endl;
  } catch (...) {}
}

