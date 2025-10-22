#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *collection;
    int capacity;
    int size;
    int front;
    int rear;
} Deque;

Deque *create_deque(int capacity);
void destroy_deque(Deque *deque);
bool is_full(Deque *deque);
bool is_empty(Deque *deque);
bool push_front(Deque *deque, int item);
bool push_back(Deque *deque, int item);
bool pop_front(Deque *deque, int *item);
bool pop_back(Deque *deque, int *item);
bool peek_front(Deque *deque, int *item);
bool peek_back(Deque *deque, int *item);

Deque *create_deque(int capacity) {
    if (capacity <= 0) return NULL;
    
    Deque *deque = malloc(sizeof(Deque));
    if (deque == NULL) return NULL;
    
    deque->collection = malloc(sizeof(int) * capacity);
    if (deque->collection == NULL) {
        free(deque);
        return NULL;
    }
    
    deque->capacity = capacity;
    deque->size = 0;
    deque->front = 0;
    deque->rear = -1;
    
    return deque;
}

void destroy_deque(Deque *deque) {
    free(deque->collection);
    free(deque);
}

bool is_full(Deque *deque) {
    return deque->size == deque->capacity;
}

bool is_empty(Deque *deque) {
    return deque->size == 0;
}

bool push_front(Deque *deque, int item) {
    if (is_full(deque)) return false;
    
    deque->front = (deque->front - 1 + deque->capacity) % deque->capacity;
    deque->collection[deque->front] = item;
    deque->size++;
    
    if (deque->size == 1) {
        deque->rear = deque->front;
    }
    
    return true;
}

bool push_back(Deque *deque, int item) {
    if (is_full(deque)) return false;
    
    deque->rear = (deque->rear + 1) % deque->capacity;
    deque->collection[deque->rear] = item;
    deque->size++;
    
    if (deque->size == 1) {
        deque->front = deque->rear;
    }
    
    return true;
}

bool pop_front(Deque *deque, int *item) {
    if (is_empty(deque)) return false;
    
    *item = deque->collection[deque->front];
    deque->front = (deque->front + 1) % deque->capacity;
    deque->size--;
    
    return true;
}

bool pop_back(Deque *deque, int *item) {
    if (is_empty(deque)) return false;
    
    *item = deque->collection[deque->rear];
    deque->rear = (deque->rear - 1 + deque->capacity) % deque->capacity;
    deque->size--;
    
    return true;
}

bool peek_front(Deque *deque, int *item) {
    if (is_empty(deque)) return false;
    
    *item = deque->collection[deque->front];
    
    return true;
}

bool peek_back(Deque *deque, int *item) {
    if (is_empty(deque)) return false;
    
    *item = deque->collection[deque->rear];
    
    return true;
}

int main() {
    
    // Criando Deque
    int n = 5;
    Deque *deque = create_deque(n);
    
    if (deque == NULL) {
        printf("Erro ao criar deque.\n");
        return 1;
    }
    
    printf("=== Testando Deque ===\n");
    printf("Capacidade: %d\n\n", deque->capacity);
    
    // Inserindo no final
    printf("Push Back: 1, 2, 3\n");
    push_back(deque, 1);
    push_back(deque, 2);
    push_back(deque, 3);
    printf("Size: %d\n\n", deque->size);
    
    // Inserindo no início
    printf("Push Front: 10, 20\n");
    push_front(deque, 10);
    push_front(deque, 20);
    printf("Size: %d\n", deque->size);
    printf("Deque Full: %s\n\n", is_full(deque) ? "Sim" : "Não");
    
    // Tentando inserir quando cheio
    bool try_push = push_back(deque, 99);
    if (!try_push) printf("Push Back falhou (deque cheio).\n\n");
    
    // Peek nas extremidades
    int front_val, back_val;
    peek_front(deque, &front_val);
    peek_back(deque, &back_val);
    printf("Peek Front: %d\n", front_val);
    printf("Peek Back: %d\n\n", back_val);
    
    // Removendo do início
    printf("Pop Front:\n");
    int val;
    pop_front(deque, &val);
    printf("  Valor: %d\n", val);
    pop_front(deque, &val);
    printf("  Valor: %d\n", val);
    printf("Size: %d\n\n", deque->size);
    
    // Removendo do final
    printf("Pop Back:\n");
    pop_back(deque, &val);
    printf("  Valor: %d\n", val);
    printf("Size: %d\n\n", deque->size);
    
    // Esvaziando o deque
    printf("Esvaziando deque...\n");
    while (!is_empty(deque)) {
        pop_front(deque, &val);
        printf("  Pop: %d\n", val);
    }
    
    printf("Size: %d\n", deque->size);
    printf("Deque Empty: %s\n\n", is_empty(deque) ? "Sim" : "Não");
    
    // Testando operações em deque vazio
    bool try_pop = pop_front(deque, &val);
    if (!try_pop) printf("Pop Front falhou (deque vazio).\n");
    
    bool try_peek = peek_back(deque, &val);
    if (!try_peek) printf("Peek Back falhou (deque vazio).\n");
    
    destroy_deque(deque);
    
    return 0;
}