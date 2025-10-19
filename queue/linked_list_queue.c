#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
} Queue;

Queue *create_queue();
void destroy_queue(Queue *queue);
bool is_full(Queue *queue);
bool is_empty(Queue *queue);
bool enqueue(Queue *queue, int value);
bool dequeue(Queue *queue, int *value);
bool peek(Queue *queue, int *value);

Queue *create_queue() {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) return NULL;
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    
    return queue;
}

void destroy_queue(Queue *queue) {
    Node *current = queue->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(queue);
}

bool is_full(Queue *queue) {
    return false;
}

bool is_empty(Queue *queue) {
    return queue->size == 0;
}

bool enqueue(Queue *queue, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = NULL;
    
    if (is_empty(queue)) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    
    queue->size++;
    return true;
}

bool dequeue(Queue *queue, int *value) {
    if (is_empty(queue)) return false;
    
    Node *oldHead = queue->head;
    *value = oldHead->value;
    
    if (queue->size == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }
    
    free(oldHead);
    queue->size--;
    
    return true;
}

bool peek(Queue *queue, int *value) {
    if (is_empty(queue)) return false;
    
    *value = queue->head->value;
    
    return true;
}

int main() {
    
    // Creating queue
    Queue *queue = create_queue();
    
    if (queue == NULL) {
        printf("Error creating queue.\n");
        return 1;
    }
    
    // Queue empty
    printf("Queue Size (1): %d\n", queue->size);
    printf("Queue Empty.\n");
    
    int n = 5;
    for (int i = 0; i < n; i++) {
        enqueue(queue, i + 1);
    }
    
    int peek_val = 0;
    peek(queue, &peek_val);
    printf("Peek Val: %d\n", peek_val);
    
    // Enqueue more items
    bool try_enqueue = enqueue(queue, 6);
    if (try_enqueue == true) printf("Enqueue Succeeded.\n");
    
    // Dequeue all elements
    int dequeue_val = 0;
    for (int i = 0; i < n + 1; i++) {
        dequeue(queue, &dequeue_val);
        printf("Dequeue Val: %d\n", dequeue_val);
    }
    
    printf("Queue Size (2): %d\n", queue->size);
    printf("Queue Empty.\n");
    
    int peek_val2 = 0;
    bool try_peek = peek(queue, &peek_val2);
    if (try_peek == false) printf("Peek Failed.\n");
    
    int try_dequeue2 = 0;
    bool try_dequeue = dequeue(queue, &try_dequeue2);
    if (try_dequeue == false) printf("Dequeue Failed.\n");
    
    // Deleting queue
    destroy_queue(queue);
    
    return 0;
}