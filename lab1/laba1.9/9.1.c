#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_array(int *arr, int size, int a, int b) {
    // Заполняем массив случайными числами в диапазоне [a..b]
    for (int i = 0; i < size; i++) {
        arr[i] = a + rand() % (b - a + 1);
    }
}

void swap_min_max(int *arr, int size) {
    if (size == 0) return;

    int min_idx = 0, max_idx = 0;

    // Находим индексы минимального и максимального элемента
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
        if (arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }

    // Меняем местами минимальный и максимальный элементы
    int temp = arr[min_idx];
    arr[min_idx] = arr[max_idx];
    arr[max_idx] = temp;
}

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}




int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <size> <lower_bound> <upper_bound>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    if (size <= 0 || a > b) {
        printf("Invalid input. Ensure size is positive and lower bound is less than or equal to upper bound.\n");
        return 1;
    }

    srand(time(NULL));
    int *arr = (int *)malloc(size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    fill_array(arr, size, a, b);

    printf("Initial array:\n");
    print_array(arr, size);

    swap_min_max(arr, size);

    printf("Array after swapping min and max:\n");
    print_array(arr, size);

    free(arr);
    return 0;
}
