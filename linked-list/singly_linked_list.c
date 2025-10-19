#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int size;
} SinglyLinkedList;

SinglyLinkedList *create_list();
void destroy_list(SinglyLinkedList *list);
bool is_empty(SinglyLinkedList *list);
bool insert_at_beginning(SinglyLinkedList *list, int value);
bool insert_at_end(SinglyLinkedList *list, int value);
bool insert_at_position(SinglyLinkedList *list, int value, int position);
bool delete_at_beginning(SinglyLinkedList *list, int *value);
bool delete_at_end(SinglyLinkedList *list, int *value);
bool delete_at_position(SinglyLinkedList *list, int *value, int position);
bool search(SinglyLinkedList *list, int value);
void print_list(SinglyLinkedList *list);

SinglyLinkedList *create_list() {
    SinglyLinkedList *list = malloc(sizeof(SinglyLinkedList));
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->size = 0;
    
    return list;
}

void destroy_list(SinglyLinkedList *list) {
    Node *current = list->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

bool is_empty(SinglyLinkedList *list) {
    return list->size == 0;
}

bool insert_at_beginning(SinglyLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
    
    list->size++;
    return true;
}

bool insert_at_end(SinglyLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    new_node->next = NULL;
    
    if (is_empty(list)) {
        list->head = new_node;
    } else {
        Node *current = list->head;
		
        while (current->next != NULL) {
            current = current->next;
        }
		
        current->next = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_position(SinglyLinkedList *list, int value, int position) {
    if (position < 0 || position > list->size) return false;
    
    if (position == 0) {
        return insert_at_beginning(list, value);
    }
    
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    Node *current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
    
    list->size++;
    return true;
}

bool delete_at_beginning(SinglyLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *old_head = list->head;
    *value = old_head->value;
    
    list->head = list->head->next;
    free(old_head);
    
    list->size--;
    return true;
}

bool delete_at_end(SinglyLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    if (list->size == 1) {
        *value = list->head->value;
        free(list->head);
        list->head = NULL;
    } else {
        Node *current = list->head;
		
        while (current->next->next != NULL) {
            current = current->next;
        }
		
        *value = current->next->value;
        free(current->next);
        current->next = NULL;
    }
    
    list->size--;
    return true;
}

bool delete_at_position(SinglyLinkedList *list, int *value, int position) {
    if (is_empty(list) || position < 0 || position >= list->size) return false;
    
    if (position == 0) {
        return delete_at_beginning(list, value);
    }
    
    Node *current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    Node *to_delete = current->next;
    *value = to_delete->value;
    current->next = to_delete->next;
    free(to_delete);
    
    list->size--;
    return true;
}

bool search(SinglyLinkedList *list, int value) {
    Node *current = list->head;
    
    while (current != NULL) {
        if (current->value == value) return true;
        current = current->next;
    }
    
    return false;
}

void print_list(SinglyLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *current = list->head;
	
    printf("List: ");

    while (current != NULL) {
        printf("%d", current->value);
        if (current->next != NULL) printf(" -> ");
        current = current->next;
    }
	
    printf("\n");
}

int main() {
    // Creating list
    SinglyLinkedList *list = create_list();
    
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
    print_list(list);
    printf("Size: %d\n\n", list->size);
    
    // Testing insertion at position
    insert_at_position(list, 15, 2);
    printf("After inserting 15 at position 2:\n");
    print_list(list);
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
    print_list(list);
    
    delete_at_end(list, &deleted_value);
    printf("Deleted from end: %d\n", deleted_value);
    print_list(list);
    
    delete_at_position(list, &deleted_value, 1);
    printf("Deleted from position 1: %d\n", deleted_value);
    print_list(list);
    printf("Size: %d\n\n", list->size);
    
    // Deleting list
    destroy_list(list);
    
    return 0;
}