#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int binary_search(int arr[], int size, int value) {
    int start_position = 0;
    int end_position = size - 1;
    
    while (start_position <= end_position) {
        int middle_position = (start_position + end_position) / 2;
        
        if (arr[middle_position] < value) {
            start_position = middle_position + 1;
        } else if (arr[middle_position] > value) {
            end_position = middle_position - 1;
        } else {
            return middle_position; 
        }
    }
    
    return -1;
}

void selection_sort(int arr[], int size) {
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

int main() {
    srand((unsigned) time(NULL));
    
    int array_size = 30;
    int arr[array_size];
    
    for (int i = 0; i < array_size; i++) {
        arr[i] = i + 1;
    }
    
    selection_sort(arr, array_size);
    
    int target_value = rand() % 30 + 1;
    int target_position = binary_search(arr, array_size, target_value);
    
    printf("Target Value: %d\n", target_value);
    printf("Target Position: %d\n", target_position);

    return 0;
}