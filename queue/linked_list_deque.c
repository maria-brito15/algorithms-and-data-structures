#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int size;
} Deque;

Deque *create_deque();
void destroy_deque(Deque *deque);
bool is_full(Deque *deque);
bool is_empty(Deque *deque);
bool push_front(Deque *deque, int value);
bool push_back(Deque *deque, int value);
bool pop_front(Deque *deque, int *value);
bool pop_back(Deque *deque, int *value);
bool peek_front(Deque *deque, int *value);
bool peek_back(Deque *deque, int *value);

Deque *create_deque() {
    Deque *deque = malloc(sizeof(Deque));
    if (deque == NULL) return NULL;
    
    deque->head = NULL;
    deque->tail = NULL;
    deque->size = 0;
    
    return deque;
}

void destroy_deque(Deque *deque) {
    Node *current = deque->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(deque);
}

bool is_full(Deque *deque) {
    return false;
}

bool is_empty(Deque *deque) {
    return deque->size == 0;
}

bool push_front(Deque *deque, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = deque->head;
    new_node->prev = NULL;
    
    if (is_empty(deque)) {
        deque->head = new_node;
        deque->tail = new_node;
    } else {
        deque->head->prev = new_node;
        deque->head = new_node;
    }
    
    deque->size++;
    return true;
}

bool push_back(Deque *deque, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = NULL;
    new_node->prev = deque->tail;
    
    if (is_empty(deque)) {
        deque->head = new_node;
        deque->tail = new_node;
    } else {
        deque->tail->next = new_node;
        deque->tail = new_node;
    }
    
    deque->size++;
    return true;
}

bool pop_front(Deque *deque, int *value) {
    if (is_empty(deque)) return false;
    
    Node *old_head = deque->head;
    *value = old_head->value;
    
    if (deque->size == 1) {
        deque->head = NULL;
        deque->tail = NULL;
    } else {
        deque->head = deque->head->next;
        deque->head->prev = NULL;
    }
    
    free(old_head);
    deque->size--;
    
    return true;
}

bool pop_back(Deque *deque, int *value) {
    if (is_empty(deque)) return false;
    
    Node *old_tail = deque->tail;
    *value = old_tail->value;
    
    if (deque->size == 1) {
        deque->head = NULL;
        deque->tail = NULL;
    } else {
        deque->tail = deque->tail->prev;
        deque->tail->next = NULL;
    }
    
    free(old_tail);
    deque->size--;
    
    return true;
}

bool peek_front(Deque *deque, int *value) {
    if (is_empty(deque)) return false;
    
    *value = deque->head->value;
    
    return true;
}

bool peek_back(Deque *deque, int *value) {
    if (is_empty(deque)) return false;
    
    *value = deque->tail->value;
    
    return true;
}

int main() {
    
    // Criando deque
    Deque *deque = create_deque();
    
    if (deque == NULL) {
        printf("Erro ao criar deque.\n");
        return 1;
    }
    
    printf("=== Testando Deque (Lista Duplamente Encadeada) ===\n\n");
    
    // Deque vazio
    printf("Deque vazio: %s\n", is_empty(deque) ? "Sim" : "Não");
    printf("Size: %d\n\n", deque->size);
    
    // Inserindo no final
    printf("Push Back: 1, 2, 3\n");
    push_back(deque, 1);
    push_back(deque, 2);
    push_back(deque, 3);
    printf("Size: %d\n\n", deque->size);
    
    // Inserindo no início
    printf("Push Front: 10, 20, 30\n");
    push_front(deque, 10);
    push_front(deque, 20);
    push_front(deque, 30);
    printf("Size: %d\n\n", deque->size);
    
    // Peek nas extremidades
    int front_val, back_val;
    peek_front(deque, &front_val);
    peek_back(deque, &back_val);
    printf("Peek Front: %d\n", front_val);
    printf("Peek Back: %d\n\n", back_val);
    
    // Removendo do início
    printf("Pop Front (3 vezes):\n");
    int val;
    for (int i = 0; i < 3; i++) {
        pop_front(deque, &val);
        printf("  Valor: %d\n", val);
    }
    printf("Size: %d\n\n", deque->size);
    
    // Removendo do final
    printf("Pop Back (2 vezes):\n");
    for (int i = 0; i < 2; i++) {
        pop_back(deque, &val);
        printf("  Valor: %d\n", val);
    }
    printf("Size: %d\n\n", deque->size);
    
    // Adicionando mais elementos
    printf("Adicionando mais elementos...\n");
    push_front(deque, 100);
    push_back(deque, 200);
    push_front(deque, 300);
    printf("Size: %d\n\n", deque->size);
    
    // Esvaziando alternando entre frente e trás
    printf("Esvaziando (alternando Pop Front e Pop Back):\n");
    while (!is_empty(deque)) {
        if (pop_front(deque, &val)) {
            printf("  Pop Front: %d\n", val);
        }
        if (!is_empty(deque) && pop_back(deque, &val)) {
            printf("  Pop Back: %d\n", val);
        }
    }
    
    printf("Size: %d\n", deque->size);
    printf("Deque vazio: %s\n\n", is_empty(deque) ? "Sim" : "Não");
    
    // Testando operações em deque vazio
    bool try_pop_front = pop_front(deque, &val);
    if (!try_pop_front) printf("Pop Front falhou (deque vazio).\n");
    
    bool try_pop_back = pop_back(deque, &val);
    if (!try_pop_back) printf("Pop Back falhou (deque vazio).\n");
    
    bool try_peek = peek_front(deque, &val);
    if (!try_peek) printf("Peek Front falhou (deque vazio).\n");
    
    // Deletando deque
    destroy_deque(deque);
    
    return 0;
}