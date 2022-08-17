#include <iostream>

void whatami(int x) { std::cout << x << " is int" << std::endl; }
void whatami(double x) { std::cout << x << " is double" << std::endl; }
template <typename T>
void whatami(T&&) { std::cout << "Something weird" << std::endl; }

int main() {
    int i = 5;
    float x = 4.2;
    whatami(i); // i is int
    whatami(x); // Something weird
}
