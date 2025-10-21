#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *tail;
    int size;
} CircularLinkedList;

CircularLinkedList *create_list();
void destroy_list(CircularLinkedList *list);
bool is_empty(CircularLinkedList *list);
bool insert_at_beginning(CircularLinkedList *list, int value);
bool insert_at_end(CircularLinkedList *list, int value);
bool insert_at_position(CircularLinkedList *list, int value, int position);
bool delete_at_beginning(CircularLinkedList *list, int *value);
bool delete_at_end(CircularLinkedList *list, int *value);
bool delete_at_position(CircularLinkedList *list, int *value, int position);
bool search(CircularLinkedList *list, int value);
void print_list(CircularLinkedList *list);

CircularLinkedList *create_list() {
    CircularLinkedList *list = malloc(sizeof(CircularLinkedList));
    if (list == NULL) return NULL;
    
    list->tail = NULL;
    list->size = 0;
    
    return list;
}

void destroy_list(CircularLinkedList *list) {
    if (is_empty(list)) {
        free(list);
        return;
    }
    
    Node *head = list->tail->next;
    Node *current = head;
    Node *next;
    
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);
    
    free(list);
}

bool is_empty(CircularLinkedList *list) {
    return list->size == 0;
}

bool insert_at_beginning(CircularLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    if (is_empty(list)) {
        new_node->next = new_node;
        list->tail = new_node;
    } else {
        new_node->next = list->tail->next;
        list->tail->next = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_end(CircularLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) return false;
    
    new_node->value = value;
    
    if (is_empty(list)) {
        new_node->next = new_node;
        list->tail = new_node;
    } else {
        new_node->next = list->tail->next;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool insert_at_position(CircularLinkedList *list, int value, int position) {
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
    
    Node *current = list->tail->next;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
    
    list->size++;
    return true;
}

bool delete_at_beginning(CircularLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    Node *head = list->tail->next;
    *value = head->value;
    
    if (list->size == 1) {
        free(head);
        list->tail = NULL;
    } else {
        list->tail->next = head->next;
        free(head);
    }
    
    list->size--;
    return true;
}

bool delete_at_end(CircularLinkedList *list, int *value) {
    if (is_empty(list)) return false;
    
    *value = list->tail->value;
    
    if (list->size == 1) {
        free(list->tail);
        list->tail = NULL;
    } else {
        Node *head = list->tail->next;
        Node *current = head;
        
        while (current->next != list->tail) {
            current = current->next;
        }
        
        current->next = head;
        free(list->tail);
        list->tail = current;
    }
    
    list->size--;
    return true;
}

bool delete_at_position(CircularLinkedList *list, int *value, int position) {
    if (is_empty(list) || position < 0 || position >= list->size) return false;
    
    if (position == 0) {
        return delete_at_beginning(list, value);
    }
    
    if (position == list->size - 1) {
        return delete_at_end(list, value);
    }
    
    Node *head = list->tail->next;
    Node *current = head;
    
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

bool search(CircularLinkedList *list, int value) {
    if (is_empty(list)) return false;
    
    Node *head = list->tail->next;
    Node *current = head;
    
    do {
        if (current->value == value) return true;
        current = current->next;
    } while (current != head);
    
    return false;
}

void print_list(CircularLinkedList *list) {
    if (is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    
    Node *head = list->tail->next;
    Node *current = head;
    
    printf("List: ");
	
    do {
        printf("%d", current->value);
        current = current->next;
        if (current != head) printf(" -> ");
    } while (current != head);
	
    printf(" -> (back to head)\n");
}

int main() {
    // Creating list
    CircularLinkedList *list = create_list();
    
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