#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge_sort(int arr[], int size);
void merge_sorted_arrays(int arr[], int start, int mid, int end);
void merge_sort_recursion(int arr[], int start, int end);

//

void merge_sort(int arr[], int size) {
    merge_sort_recursion(arr, 0, size - 1);
}

void merge_sort_recursion(int arr[], int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        merge_sort_recursion(arr, start, mid);
        merge_sort_recursion(arr, mid + 1, end);

        merge_sorted_arrays(arr, start, mid, end);
    }
}

void merge_sorted_arrays(int arr[], int start, int mid, int end) {
    int left_size = mid - start + 1;
    int right_size = end - mid;

    int temp_left[left_size];
    int temp_right[right_size];

    for (int i = 0; i < left_size; i++) {
        temp_left[i] = arr[start + i];
    }

    for (int i = 0; i < right_size; i++) {
        temp_right[i] = arr[mid + 1 + i];
    }

    int i = 0, m = 0;

    for (int k = start; k <= end; k++) {
        if ((i < left_size) && (m >= right_size || temp_left[i] <= temp_right[m])) {
            arr[k] = temp_left[i];
            i++;
        } else {
            arr[k] = temp_right[m];
            m++;
        }
    }
}

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

int main() {
    srand(time(NULL));

    int n = 50;
    int array[n];

    for (int i = 0; i < n; i++) {
        int random = rand() % 100 + 1;
        array[i] = random;
    }

    printf("Array Before Merge Sort:\n");
    print_array(array, n);

    merge_sort(array, n);

    printf("\nArray After Merge Sort:\n");
    print_array(array, n);

    return 0;
}