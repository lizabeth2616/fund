#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void fill_dynamic_array(int **arr, int *size, int min_size, int max_size, int min_val, int max_val) {
    *size = rand() % (max_size - min_size + 1) + min_size;
    *arr = (int *)malloc(*size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < *size; i++) {
        (*arr)[i] = rand() % (max_val - min_val + 1) + min_val;
    }
}

int find_nearest_value(int *arr, int size, int value) {
    int nearest = arr[0];
    for (int i = 1; i < size; i++) {
        if (abs(arr[i] - value) < abs(nearest - value)) {
            nearest = arr[i];
        }
    }
    return nearest;
}

void form_array_C(int *A, int size_A, int *B, int size_B, int **C) {
    *C = (int *)malloc(size_A * sizeof(int));
    if (!C) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < size_A; i++) {
        int nearest_b = find_nearest_value(B, size_B, A[i]);
        (*C)[i] = A[i] + nearest_b;
    }
}

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    int *A, *B, *C;
    int size_A, size_B;

    fill_dynamic_array(&A, &size_A, 10, 10000, -1000, 1000);
    fill_dynamic_array(&B, &size_B, 10, 10000, -1000, 1000);

    printf("Array A:\n");
    print_array(A, size_A);
    printf("Array B:\n");
    print_array(B, size_B);
    form_array_C(A, size_A, B, size_B, &C);

    printf("Array C:\n");
    print_array(C, size_A);
    free(A);
    free(B);
    free(C);

    return 0;
}
