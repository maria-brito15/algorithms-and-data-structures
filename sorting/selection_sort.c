#include <stdio.h>

void SelectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        
        for (int m = i + 1; m < size; m++) {
            if (arr[m] < arr[min_idx]) {
                min_idx = m;
            }
        }
        
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
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
    
    SelectionSort(arr, size);
    
    printf("Array After:\n");
    printArray(arr, size);
    
    return 0;
}