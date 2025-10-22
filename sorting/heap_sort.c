#include <stdio.h>

void HeapSort(int arr[], int size);
void BuildMaxHeap(int arr[], int size);
void MaxHeapify(int arr[], int heap_size, int i);
void printArray(int arr[], int size);

void HeapSort(int arr[], int size) {
    BuildMaxHeap(arr, size);
    
    for (int i = size - 1; i >= 1; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        MaxHeapify(arr, i, 0);
    }
}

void BuildMaxHeap(int arr[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        MaxHeapify(arr, size, i);
    }
}

void MaxHeapify(int arr[], int heap_size, int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    
    if (left < heap_size && arr[left] > arr[largest]) {
        largest = left;
    }
    
    if (right < heap_size && arr[right] > arr[largest]) {
        largest = right;
    }
    
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        MaxHeapify(arr, heap_size, largest);
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
    
    HeapSort(arr, size);
    
    printf("Array After:\n");
    printArray(arr, size);
    
    return 0;
}