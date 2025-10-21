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
} DoublyLinkedList;

DoublyLinkedList *create_list();
void destroy_list(DoublyLinkedList *list);
bool is_empty(DoublyLinkedList *list);
bool insert_at_beginning(DoublyLinkedList *list, int value);
bool insert_at_end(DoublyLinkedList *list, int value);
bool insert_at_position(DoublyLinkedList *list, int value, int position);
bool delete_at_beginning(DoublyLinkedList *list, int *value);
bool delete_at_end(DoublyLinkedList *list, int *value);
bool delete_at_position(DoublyLinkedList *list, int *value, int position);
bool search(DoublyLinkedList *list, int value);
void print_list_forward(DoublyLinkedList *list);
void print_list_backward(DoublyLinkedList *list);

DoublyLinkedList *create_list() {
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return list;
}

void destroy_list(DoublyLinkedList *list) {
    Node *current = list->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

bool is_empty(DoublyLinkedList *list) {
    return list->size == 0;
}

bool insert_at_beginning(DoublyLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = list->head;
    
    if (is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->head->prev = new_node;
        list->head = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_end(DoublyLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_position(DoublyLinkedList *list, int value, int position) {
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
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    new_node->next = current->next;
    new_node->prev = current;
    current->next->prev = new_node;
    current->next = new_node;
    
    list->size++;
    return true;
}

bool delete_at_beginning(DoublyLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *old_head = list->head;
    *value = old_head->value;
    
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    
    free(old_head);
    list->size--;
    
    return true;
}

bool delete_at_end(DoublyLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *old_tail = list->tail;
    *value = old_tail->value;
    
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    
    free(old_tail);
    list->size--;
    
    return true;
}

bool delete_at_position(DoublyLinkedList *list, int *value, int position) {
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

bool search(DoublyLinkedList *list, int value) {
    Node *current = list->head;
    
    while (current != NULL) {
        if (current->value == value) return true;
        current = current->next;
    }
    
    return false;
}

void print_list_forward(DoublyLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *current = list->head;
	
    printf("List (forward): ");
	
    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) printf(" <-> ");
        current = current->next;
    }
	
    printf("\n");
}

void print_list_backward(DoublyLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *current = list->tail;
	
    printf("List (backward): ");
	
    while (current != NULL) {
        printf("%d", current->value);
        if (current->prev != NULL) printf(" <-> ");
        current = current->prev;
    }
    printf("\n");
}

int main() {
    // Creating list
    DoublyLinkedList *list = create_list();
    
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