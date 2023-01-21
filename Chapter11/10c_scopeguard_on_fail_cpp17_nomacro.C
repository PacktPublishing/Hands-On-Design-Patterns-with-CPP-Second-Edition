#include <exception>
#include <iostream>

enum Outcome { SUCCESS, FAIL_RETURN, FAIL_THROW };

// Demo disk storage, does nothing useful but may throw exception.
class Storage {
    public:
    Storage() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    void finalize() { finalized_ = true; }
    bool finalized() const { return finalized_; }
    int get() const { return i_; }
    private:
    int i_ = 0;
    int i1_ = 0;
    bool finalized_ = false;
};

// Demo memory index, does nothing useful but may throw exception.
class Index {
    public:
    Index() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    int get() const { return i_; }
    private:
    int i_ = 0;
    int i1_ = 0;
};

class UncaughtExceptionDetector {
    public:
    UncaughtExceptionDetector() : count_(std::uncaught_exceptions()) {}
    operator bool() const noexcept { return std::uncaught_exceptions() > count_; }
    private:
    const int count_;
};

template <typename Func>
class FailScopeGuard {
    public:
    FailScopeGuard(Func&& func) : func_(func) {}
    FailScopeGuard(const Func& func) : func_(func) {}
    ~FailScopeGuard() {
        if (detector_) func_();
    }
    FailScopeGuard(FailScopeGuard&& other) = default;
    private:
    Func func_;
    UncaughtExceptionDetector detector_;
    FailScopeGuard() = delete;
    FailScopeGuard& operator=(const FailScopeGuard&) = delete;
};

template <typename Func>
class ExitScopeGuard {
    public:
    ExitScopeGuard(Func&& func) : func_(func) {}
    ExitScopeGuard(const Func& func) : func_(func) {}
    ~ExitScopeGuard() { func_(); }
    ExitScopeGuard(ExitScopeGuard&& other) = default;
    private:
    Func func_;
    UncaughtExceptionDetector detector_;
    ExitScopeGuard() = delete;
    ExitScopeGuard& operator=(const ExitScopeGuard&) = delete;
};

int main() {
    Storage S;
    Index I;
    try {
        S.insert(42, SUCCESS);
        ExitScopeGuard SF([&] { S.finalize(); });
        FailScopeGuard SU([&] { S.undo(); });
        I.insert(42, FAIL_THROW);
    } catch (...) {
    }

    {
        ExitScopeGuard SO([&] { std::cout << "Success" << std::endl; });
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else if (!S.finalized()) std::cout << "Not finalized" << std::endl;
    else std::cout << "Database OK" << std::endl;
}
