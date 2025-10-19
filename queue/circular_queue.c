#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *collection;
    int capacity;
    int size;
    int front;
    int rear;
} Queue;

Queue *create_queue(int capacity);
void destroy_queue(Queue *queue);
bool is_full(Queue *queue);
bool is_empty(Queue *queue);
bool enqueue(Queue *queue, int item);
bool dequeue(Queue *queue, int *item);
bool peek(Queue *queue, int *item);

Queue *create_queue(int capacity) {
    if (capacity <= 0) return NULL;
    
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) return NULL;
    
    queue->collection = malloc(sizeof(int) * capacity);
    if (queue->collection == NULL) {
        free(queue);
        return NULL;
    }
    
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    
    return queue;
}

void destroy_queue(Queue *queue) {
    free(queue->collection);
    free(queue);
}

bool is_full(Queue *queue) {
    return queue->capacity == queue->size;
}

bool is_empty(Queue *queue) {
    return queue->size == 0;
}

bool enqueue(Queue *queue, int item) {
    if (is_full(queue)) return false;
    
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->collection[queue->rear] = item;
    queue->size++;
    
    return true;
}

bool peek(Queue *queue, int *item) {
    if (is_empty(queue)) return false;
    
    *item = queue->collection[queue->front];
    
    return true;
}

bool dequeue(Queue *queue, int *item) {
    if (is_empty(queue)) return false;
    
    *item = queue->collection[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    return true;
}

int main() {
    
    // Creating Queue
    int n = 5;
    Queue *queue = create_queue(n);
    
    if (queue == NULL) {
        printf("Error creating queue.\n");
        return 1;
    }
    
    // Queue Empty
    printf("Queue Capacity: %d\n", queue->capacity);
    printf("Queue Size (1): %d\n", queue->size);
    printf("Queue Empty.\n");
    
    for (int i = 0; i < n; i++) {
        enqueue(queue, i + 1);
    }
    
    // Queue Full
    printf("Queue Size (2): %d\n", queue->size);
    printf("Queue Full.\n");
    
    int peek_val = 0;
    peek(queue, &peek_val);
    printf("Peek Val: %d\n", peek_val);
    
    bool try_enqueue = enqueue(queue, 6);
    if (try_enqueue == false) printf("Enqueue Failed.\n");
    
    // Queue Empty
    int dequeue_val = 0;
    for (int i = 0; i < n; i++) {
        dequeue(queue, &dequeue_val);
        printf("Dequeue Val: %d\n", dequeue_val);
    }
    
    printf("Queue Size (3): %d\n", queue->size);
    printf("Queue Empty.\n");
    
    int peek_val2 = 0;
    bool try_peek = peek(queue, &peek_val2);
    if (try_peek == false) printf("Peek Failed.\n");
    
    int try_dequeue2 = 0;
    bool try_dequeue = dequeue(queue, &try_dequeue2);
    if (try_dequeue == false) printf("Dequeue Failed.\n");
    
    // Deleting Queue
    destroy_queue(queue);
    
    return 0;
}