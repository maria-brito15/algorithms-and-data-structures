#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    int size;
} Stack;

Stack *create_stack();
void destroy_stack(Stack *stack);
bool is_full(Stack *stack);
bool is_empty(Stack *stack);
bool push(Stack *stack, int value);
bool pop(Stack *stack, int *value);
bool peek(Stack *stack, int *value);

Stack *create_stack() {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    
    stack->top = NULL;
    stack->size = 0;
    
    return stack;
}

void destroy_stack(Stack *stack) {
    Node *current = stack->top;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(stack);
}

bool is_full(Stack *stack) {
    return false;
}

bool is_empty(Stack *stack) {
    return stack->size == 0;
}

bool push(Stack *stack, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = stack->top;
    stack->top = new_node;
    
    stack->size++;
    return true;
}

bool pop(Stack *stack, int *value) {
    if (is_empty(stack)) return false;
    
    Node *old_top = stack->top;
    *value = old_top->value;
    
    stack->top = stack->top->next;
    free(old_top);
    stack->size--;
    
    return true;
}

bool peek(Stack *stack, int *value) {
    if (is_empty(stack)) return false;
    
    *value = stack->top->value;
    
    return true;
}

int main() {
    
    // Creating Stack
    Stack *stack = create_stack();
    
    if (stack == NULL) {
        printf("Error creating stack.\n");
        return 1;
    }
    
    // Stack Empty
    printf("Stack Size (1): %d\n", stack->size);
    printf("Stack Empty.\n");
    
    int n = 5;
    for (int i = 0; i < n; i++) {
        push(stack, i + 1);
    }
    
    printf("Stack Size (2): %d\n", stack->size);
    
    int peek_val = 0;
    peek(stack, &peek_val);
    printf("Peek Val: %d\n", peek_val);
    
    // Push more items
    bool try_push = push(stack, 6);
    if (try_push == true) printf("Push Succeeded.\n");
    
    // Pop all elements
    int pop_val = 0;
    for (int i = 0; i < n + 1; i++) {
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