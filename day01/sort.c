#include <stdlib.h>

struct partitions {
    int fst_idx;
    int snd_idx;
};

void sort(int *arr, int size);
void quicksort(int *arr, int left, int right);
struct partitions partition(int *arr, int l, int r, int p);
void swap(int *i, int *j);

/// @brief Sorts an array in place.
/// @param arr Array to be sorted.
/// @param size Size of the array.
void sort(int *arr, int size) {
    quicksort(arr, 0, size - 1);
}

void quicksort(int *arr, int left, int right) {
    if (left >= right)
        return;

    int pivot = left + (rand() % (right - left + 1));

    struct partitions partitions = partition(arr, left, right, pivot);

    quicksort(arr, left, partitions.fst_idx - 1);
    quicksort(arr, partitions.snd_idx + 1, right);
}

struct partitions partition(int *arr, int l, int r, int p) {
    int p_element = arr[p];
    swap(arr + l, arr + p);
    int i = l + 1;
    while (i <= r) {
        int i_element = arr[i];
        if (i_element < p_element)
            swap(arr + i++, arr + l++);
        else if (i_element > p_element)
            swap(arr + i, arr + r--);
        else i++;
    }
    return (struct partitions) { l, r };
}

void swap(int *i, int *j) {
    int temp = *i;
    *i = *j;
    *j = temp;
}
