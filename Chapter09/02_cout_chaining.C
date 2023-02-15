#include <iostream>

using std::cout;
using std::endl;

int main() {
    int i = 1, j = 2;
    cout << i << j;
    cout << endl;

    std::cout.operator<<(i).operator<<(j);
    cout << endl;

    auto& out1 = cout.operator<<(i);
    out1.operator<<(j);
    cout << endl;
}
