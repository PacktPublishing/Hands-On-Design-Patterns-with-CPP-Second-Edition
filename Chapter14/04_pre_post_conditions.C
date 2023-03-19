#include <iostream>
#include <cassert>

class Base {
    bool StateIsValid() const { return actions_started_ == actions_completed_; }
    protected:
    size_t actions_started_ = 0;
    size_t actions_completed_ = 0;   // Class invariant - all actions started are completed
    public:
    void VerifiedAction() {
        assert(StateIsValid());
        ActionImpl();
        assert(StateIsValid());
    }
    virtual void ActionImpl() = 0;
};

class Derived : public Base {
    public:
    void ActionImpl() override { 
        ++actions_started_;
        std::cout << "Performing the action" << std::endl;
        ++actions_completed_;
    }
};

int main() {
    Derived d;
    d.VerifiedAction();
}
