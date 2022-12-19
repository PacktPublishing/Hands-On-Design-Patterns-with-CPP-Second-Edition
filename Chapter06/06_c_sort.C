#include <stdlib.h>

int less(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    int a[10] = { 1, 10, 2, 9, 3, 8, 4, 7, 5, 0 };
    qsort(a, sizeof(int), 10, less);
}
