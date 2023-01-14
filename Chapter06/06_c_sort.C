#include <stdlib.h>
#include <iostream>

int less(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    int a[10] = { 1, 10, 2, 9, 3, 8, 4, 7, 5, 0 };
    qsort(a, 10, sizeof(int), less);
    for (int x : a) std::cout << x << " ";
    std::cout << std::endl;
}
