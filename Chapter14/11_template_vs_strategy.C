#include <iostream>
#include <fstream>
#include <string>

class CommunicationStrategy {
    public:
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void Send(int v) = 0;
    virtual void Send(long v) = 0;
    virtual void Send(double v) = 0;
};

class Base {
    public:
    Base(int i, double x) : i_(i), x_(x) {}
    virtual ~Base() = default;
    void Send(CommunicationStrategy* comm) {
        comm->Open();
        SendData(comm);
        comm->Close();
    }
    protected:
    virtual void SendData(CommunicationStrategy* comm) {
        // send base class data
        comm->Send(i_);
        comm->Send(x_);
    }
    private:
    int i_;
    double x_;
};

class Derived : public Base {
    long l_;
    protected:
    void SendData(CommunicationStrategy* comm) override {
        Base::SendData(comm);
        // send derived class data 
        comm->Send(l_);
    }
    public:
    Derived(long l, int i, double x) : Base(i, x), l_(l) {}
};

class FileCommunication : public CommunicationStrategy {
    const char* const filename_;
    std::ofstream f_;
    public:
    explicit FileCommunication(const char* filename) : filename_(filename) {}
    void Open() override {
        f_.open(filename_, std::ios_base::app);
    }
    void Close() override {
        f_ << std::endl;
        f_.close();
    }
    void Send(int v) override { f_ << v << " "; }
    void Send(long v) override { f_ << v << " "; }
    void Send(double v) override { f_ << v << " "; }
};

int main() {
    FileCommunication comm("out.txt");
    Base* b = new Derived(99, 42, 3.14);
    b->Send(&comm);
    delete b;
}
