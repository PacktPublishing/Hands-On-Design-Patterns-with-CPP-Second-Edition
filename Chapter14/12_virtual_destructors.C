#include <iostream>

class Base {
    public:
    ~Base() { // Non-virtual interface!
        std::cout << "Deleting now" << std::endl;
        clear(); // Employing Template Method here
        std::cout << "Deleting done" << std::endl;
    }
    protected:
    virtual void clear() { std::cout << "Base::clear()" << std::endl; } // Customizable part
};

class Derived : public Base {
    private:
    void clear() override {
        std::cout << "Derived::clear()" << std::endl; 
        Base::clear();
    }
};

int main() {
    Base* b = new Derived;
    delete b;   // Calls Base::clear() only!
}
