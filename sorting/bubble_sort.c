#include <stdio.h>

void BubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int m = 0; m < size - i - 1; m++) {
            if (arr[m] > arr[m + 1]) {
                int temp = arr[m];
                arr[m] = arr[m + 1];
                arr[m + 1] = temp;
            }
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
    
    BubbleSort(arr, size);
    
    printf("Array After:\n");
    printArray(arr, size);
    
    return 0;
}