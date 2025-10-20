#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int linear_search(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    
    return -1;
}

int main() {
    srand((unsigned) time(NULL));
    
    int array_size = 10;
    int array[array_size];
    
    for (int i = 0; i < array_size; i++) {
        array[i] = i + 1;
    }
    
    int target_value = rand() % 10 + 1;
    int target_position = linear_search(array, array_size, target_value);
    
    printf("Target Value: %d\n", target_value);
    printf("Target Position: %d\n", target_position);

    return 0;
}