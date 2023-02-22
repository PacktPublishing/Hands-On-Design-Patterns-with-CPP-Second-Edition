#include <vector>
#include <iostream>

template <typename T> class registry {
  public:
  static size_t count;
  static T* head;
  T* prev;
  T* next;
  protected:
  registry() {
    ++count;
    prev = nullptr;
    next = head;
    head = static_cast<T*>(this);
    if (next) next->prev = head;
  }
  registry(const registry&) {
    ++count;
    prev = nullptr;
    next = head;
    head = static_cast<T*>(this);
    if (next) next->prev = head;
  }
  ~registry() {
    --count;
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (head == this) head = next;
  }
};
template <typename T> size_t registry<T>::count(0);
template <typename T> T* registry<T>::head(nullptr);


class A : public registry<A> {
  int i_;
  public:
  A(int i) : i_(i) {(void)i_;}
};

class B : public registry<B> {
  int i_;
  public:
  B(int i) : i_(i) {(void)i_;}
};

int main() {
    {
        A a(0);
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;
    {
        B b1(1);
        B b2 = std::move(b1);
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;
    {
        std::vector<A> v { 1, 2, 3, 4 };
        std::cout << A::count << " " << B::count << std::endl;
    }
    std::cout << A::count << " " << B::count << std::endl;
}
