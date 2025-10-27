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
    int size;
} DoublyCircularLinkedList;

DoublyCircularLinkedList *create_list();
void destroy_list(DoublyCircularLinkedList *list);
bool is_empty(DoublyCircularLinkedList *list);
bool insert_at_beginning(DoublyCircularLinkedList *list, int value);
bool insert_at_end(DoublyCircularLinkedList *list, int value);
bool insert_at_position(DoublyCircularLinkedList *list, int value, int position);
bool delete_at_beginning(DoublyCircularLinkedList *list, int *value);
bool delete_at_end(DoublyCircularLinkedList *list, int *value);
bool delete_at_position(DoublyCircularLinkedList *list, int *value, int position);
bool search(DoublyCircularLinkedList *list, int value);
void print_list_forward(DoublyCircularLinkedList *list);
void print_list_backward(DoublyCircularLinkedList *list);

DoublyCircularLinkedList *create_list() {
    DoublyCircularLinkedList *list = malloc(sizeof(DoublyCircularLinkedList));
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->size = 0;
    
    return list;
}

void destroy_list(DoublyCircularLinkedList *list) {
    if (is_empty(list)) {
        free(list);
        return;
    }
    
    Node *current = list->head;
    Node *next;
    
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    
    free(list);
}

bool is_empty(DoublyCircularLinkedList *list) {
    return list->size == 0;
}

bool insert_at_beginning(DoublyCircularLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    if (is_empty(list)) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
    } else {
        Node *tail = list->head->prev;
        
        new_node->next = list->head;
        new_node->prev = tail;
        tail->next = new_node;
        list->head->prev = new_node;
        list->head = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_end(DoublyCircularLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    if (is_empty(list)) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
    } else {
        Node *tail = list->head->prev;
        
        new_node->next = list->head;
        new_node->prev = tail;
        tail->next = new_node;
        list->head->prev = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_position(DoublyCircularLinkedList *list, int value, int position) {
    if (position < 0 || position > list->size) return false;
    
    if (position == 0) {
        return insert_at_beginning(list, value);
    }
    
    if (position == list->size) {
        return insert_at_end(list, value);
    }
    
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    Node *current = list->head;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }
    
    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;
    
    list->size++;
    return true;
}

bool delete_at_beginning(DoublyCircularLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *old_head = list->head;
    *value = old_head->value;
    
    if (list->size == 1) {
        list->head = NULL;
    } else {
        Node *tail = list->head->prev;
        
        list->head = list->head->next;
        list->head->prev = tail;
        tail->next = list->head;
    }
    
    free(old_head);
    list->size--;
    
    return true;
}

bool delete_at_end(DoublyCircularLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *tail = list->head->prev;
    *value = tail->value;
    
    if (list->size == 1) {
        list->head = NULL;
    } else {
        Node *new_tail = tail->prev;
        
        new_tail->next = list->head;
        list->head->prev = new_tail;
    }
    
    free(tail);
    list->size--;
    
    return true;
}

bool delete_at_position(DoublyCircularLinkedList *list, int *value, int position) {
    if (is_empty(list) || position < 0 || position >= list->size) return false;
    
    if (position == 0) {
        return delete_at_beginning(list, value);
    }
    
    if (position == list->size - 1) {
        return delete_at_end(list, value);
    }
    
    Node *current = list->head;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }
    
    *value = current->value;
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    
    list->size--;
    return true;
}

bool search(DoublyCircularLinkedList *list, int value) {
    if (is_empty(list)) return false;
    
    Node *current = list->head;
    
    do {
        if (current->value == value) return true;
        current = current->next;
    } while (current != list->head);
    
    return false;
}

void print_list_forward(DoublyCircularLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *current = list->head;
    
    printf("List (forward): ");
    
    do {
        printf("%d", current->value);
        current = current->next;
        if (current != list->head) printf(" <-> ");
    } while (current != list->head);
    
    printf(" <-> (back to head)\n");
}

void print_list_backward(DoublyCircularLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *tail = list->head->prev;
    Node *current = tail;
    
    printf("List (backward): ");
    
    do {
        printf("%d", current->value);
        current = current->prev;
        if (current != tail) printf(" <-> ");
    } while (current != tail);
    
    printf(" <-> (back to tail)\n");
}

int main() {
    // Creating list
    DoublyCircularLinkedList *list = create_list();
    
    if (list == NULL) {
        printf("Error creating list.\n");
        return 1;
    }
    
    // Testing insertions
    printf("=== Testing Insertions ===\n");
    insert_at_beginning(list, 10);
    insert_at_beginning(list, 5);
    insert_at_end(list, 20);
    insert_at_end(list, 25);
    print_list_forward(list);
    print_list_backward(list);
    printf("Size: %d\n\n", list->size);
    
    // Testing insertion at position
    insert_at_position(list, 15, 2);
    printf("After inserting 15 at position 2:\n");
    print_list_forward(list);
    print_list_backward(list);
    printf("Size: %d\n\n", list->size);
    
    // Testing search
    printf("=== Testing Search ===\n");
    printf("Search for 15: %s\n", search(list, 15) ? "Found" : "Not Found");
    printf("Search for 100: %s\n\n", search(list, 100) ? "Found" : "Not Found");
    
    // Testing deletions
    printf("=== Testing Deletions ===\n");
    int deleted_value;
    
    delete_at_beginning(list, &deleted_value);
    printf("Deleted from beginning: %d\n", deleted_value);
    print_list_forward(list);
    
    delete_at_end(list, &deleted_value);
    printf("Deleted from end: %d\n", deleted_value);
    print_list_forward(list);
    
    delete_at_position(list, &deleted_value, 1);
    printf("Deleted from position 1: %d\n", deleted_value);
    print_list_forward(list);
    print_list_backward(list);
    printf("Size: %d\n\n", list->size);
    
    // Deleting list
    destroy_list(list);
    
    return 0;
}