#include <stdio.h>

void InsertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int m = i - 1;
        
        while (m >= 0 && arr[m] > key) {
            arr[m + 1] = arr[m];
            m--;
        }
        
        arr[m + 1] = key;
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
    
    InsertionSort(arr, size);
    
    printf("Array After:\n");
    printArray(arr, size);
    
    return 0;
}