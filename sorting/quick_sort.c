#include <stdio.h>

void QuickSort(int arr[], int size);
void QuickSortRecursive(int arr[], int start, int end);
int partition(int arr[], int start, int end);
void printArray(int arr[], int size);

void QuickSort(int arr[], int size) {
    QuickSortRecursive(arr, 0, size - 1);
}

void QuickSortRecursive(int arr[], int start, int end) {
    if (start < end) {
        int pivot_index = partition(arr, start, end);
        QuickSortRecursive(arr, start, pivot_index - 1);
        QuickSortRecursive(arr, pivot_index + 1, end);
    }
}

int partition(int arr[], int start, int end) {
    int pivot = arr[end];
    int m = start;
    
    for (int i = start; i < end; i++) {
        if (arr[i] <= pivot) {
            int temp = arr[m];
            arr[m] = arr[i];
            arr[i] = temp;
            
            m++;
        }
    }
    
    int temp = arr[m];
    arr[m] = arr[end];
    arr[end] = temp;
    
    return m;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    
    printf("\n");
}

int main() {
    int size = 10;
    int arr[size];
    
    printf("Array Before:\n");
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    
    printArray(arr, size);
    
    QuickSort(arr, size);
    
    printf("Array After:\n");
    printArray(arr, size);
    
    return 0;
}