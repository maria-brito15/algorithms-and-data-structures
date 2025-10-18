#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *collection;
    int capacity;
    int size;
} Stack;

Stack *create_stack(int capacity);
void destroy_stack(Stack *stack);
bool is_full(Stack *stack);
bool is_empty(Stack *stack);
bool pop(Stack *stack, int *item);
bool push(Stack *stack, int item);
bool peek(Stack *stack, int *item);

Stack *create_stack(int capacity) {
    if (capacity <= 0) return NULL;
    
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    
    stack->collection = malloc(sizeof(int) * capacity);
    if (stack->collection == NULL) {
        free(stack);
        return NULL;
    }
    
    stack->capacity = capacity;
    stack->size = 0;
    
    return stack;
}

void destroy_stack(Stack *stack) {
    free(stack->collection);
    free(stack);
}

bool is_full(Stack *stack) {
    return stack->capacity == stack->size;
}

bool is_empty(Stack *stack) {
    return stack->size == 0;
}

bool push(Stack *stack, int item) {
    if (is_full(stack)) return false;

    stack->collection[stack->size] = item;
    stack->size++;
    
    return true;
}

bool peek(Stack *stack, int *item) {
    if (is_empty(stack)) return false;
    
    *item = stack->collection[stack->size - 1];
    
    return true;
}

bool pop(Stack *stack, int *item) {
    if (is_empty(stack)) return false;
    
    stack->size--;
    *item = stack->collection[stack->size];
    
    return true;
}

int main() {
    
    // Creating Stack
    int n = 5;
    Stack *stack = create_stack(n);
    
    if (stack == NULL) {
        printf("Error creating stack.\n");
        return 1;
    }
    
    // Stack Empty
    printf("Stack Capacity: %d\n", stack->capacity);
    printf("Stack Size (1): %d\n", stack->size);
    printf("Stack Empty.\n");
    
    for (int i = 0; i < n; i++) {
        push(stack, i + 1);
    }
    
    // Stack Full
    printf("Stack Size (2): %d\n", stack->size);
    printf("Stack Full.\n");
    
    int peek_val = 0;
    peek(stack, &peek_val);
    printf("Peek Val: %d\n", peek_val);
    
    bool try_push = push(stack, 6);
    if (try_push == false) printf("Push Failed.\n");
    
    // Stack Empty
    int pop_val = 0;
    for (int i = 0; i < n; i++) {
        pop(stack, &pop_val);
        printf("Pop Val: %d\n", pop_val);
    }
    
    printf("Stack Size (3): %d\n", stack->size);
    printf("Stack Empty.\n");
    
    int peek_val2 = 0;
    bool try_peek = peek(stack, &peek_val2);
    if (try_peek == false) printf("Peek Failed.\n");
    
    int try_pop2 = 0;
    bool try_pop = pop(stack, &try_pop2);
    if (try_pop == false) printf("Pop Failed.\n");
    
    // Deleting Stack
    destroy_stack(stack);
    
    return 0;
}