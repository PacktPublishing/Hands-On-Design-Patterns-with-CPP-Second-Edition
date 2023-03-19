#include <iostream>
#include <cassert>

class Base {
    bool StateIsValid() const { return actions_started_ == actions_completed_ + actions_failed_; }
    protected:
    size_t actions_started_ = 0;
    size_t actions_completed_ = 0;
    size_t actions_failed_ = 0;   // Class invariant - all actions started are completed or failed
    public:
    void VerifiedAction(bool fail) {
        assert(StateIsValid());
        ActionImpl(fail);
        assert(StateIsValid());
    }
    virtual void ActionImpl(bool fail) = 0;
};

class Derived : public Base {
    public:
    void ActionImpl(bool fail) override { 
        ++actions_started_;
        std::cout << "Performing the action" << std::endl;
        try {
            if (fail) throw 0;
            std::cout << "Action succeeded" << std::endl;
            ++actions_completed_;
        } catch (...) {
            std::cout << "Action failed" << std::endl;
            ++actions_failed_;
        }
    }
};

int main() {
    Derived d;
    d.VerifiedAction(false);
    d.VerifiedAction(true);
}
