#include <stdlib.h>
#include <stdio.h>

int less(const void* a, const void* b) {
    return *(const int*)a - *(const int*)b;
}

int main() {
    int a[] = { 1, 10, 2, 9, 3, 8, 4, 7, 5, 0 };

    int n = sizeof(a) / sizeof(a[0]);
    qsort(a, n, sizeof(int), less);

    for (int i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
