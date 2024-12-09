#include <iostream>
#include <memory>
#include <vector>

class Base {
    public:
    virtual ~Base() = default;
    bool TheAlgorithm() {
        std::cout << "\nAttempting step 1..." << std::endl;
        if (!Step1()) {
            std::cout << "Step 1 failed." << std::endl;
            return false; // Step 1 failed
        }
        std::cout << "Proceeding to step 2..." << std::endl;
        Step2();
        std::cout << "Step 2 done." << std::endl;
        return true;
    }
    virtual bool Step1() {
        std::cout << "Step1 - version 0" << std::endl;
        return true;
    }
    virtual void Step2() = 0;
};

class Derived1 : public Base {
    public:
    void Step2() override { std::cout << "Step2 - version 1" << std::endl; }
};

class Derived2 : public Base {
    public:
    bool Step1() override { std::cout << "Step1 - version 2" << std::endl; return false; }
    void Step2() override { std::cout << "Step2 - version 2" << std::endl; }
};

int main() {
    std::vector<std::unique_ptr<Base>> bv;
    bv.emplace_back(new Derived1);
    bv.emplace_back(new Derived2);
    for (const auto& b : bv) {
        b->TheAlgorithm();
    }
}
