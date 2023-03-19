#include <iostream>
#include <fstream>

class Base {
    public:
    Base(int i, double x) : i_(i), x_(x) {}
    virtual ~Base() = default;
    void Send(const char* filename) { // Template Method used here
        std::ofstream f(filename, std::ios_base::app);     // open connection ...
        SendData(f); // Customization point
        f << std::endl;
        f.close(); // close connection ...
    }
    protected:
    virtual void SendData(std::ostream& f) {
        // send base class data
        f << i_ << " " << x_ << " ";
    }
    private:
    int i_;
    double x_;
};

class Derived : public Base {
    long l_;
    protected:
    void SendData(std::ostream& f) override {
        Base::SendData(f);
        // send derived class data 
        f << l_ << " ";
    }
    public:
    Derived(long l, int i, double x) : Base(i, x), l_(l) {}
};

int main() {
    Base* b = new Derived(99, 42, 3.14);
    b->Send("out.txt");
    delete b;
}
