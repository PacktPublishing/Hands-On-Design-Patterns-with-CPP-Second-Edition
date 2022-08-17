#include <iostream>
using namespace std;

template <typename T> concept HasXOR = requires(T a, T b) { a ^ b; };
template <typename T> concept Assignable = requires(T a, T b) { T(std::move(b)); b = std::move(a); };

int swap_id = 0;
const char* swap_name[] = { "XOR swap", "Move swap" };

template <HasXOR T> void MySwap(T& x, T& y) { 
    x = x ^ y;
    y = x ^ y;
    x = x ^ y;
    swap_id = 0;
}

template <Assignable T> void MySwap(T& x, T& y)
    requires (!HasXOR<T>)
{ 
    T tmp(std::move(x));
    x = std::move(y);
    y = std::move(tmp);
    swap_id = 1;
}

int main() {
    {
        int a = 7, b = 42;
        MySwap(a, b);
        cout << "Int: " << a << " " << b << " " << swap_name[swap_id] << endl;
    }
    {
        double a = 7, b = 42;
        MySwap(a, b);
        cout << "Double: " << a << " " << b << " " << swap_name[swap_id] << endl;
    }
}
