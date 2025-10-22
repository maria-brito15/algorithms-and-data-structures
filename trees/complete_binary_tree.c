// All levels are completely filled except possibly the last, which is filled from left to right.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int value;
    bool occupied;
} Node;

typedef struct {
    int capacity;
    Node *collection;
    int total_nodes;
} CompleteBinaryTree;

// 

CompleteBinaryTree *create_complete_tree(int capacity);
void clear_tree(CompleteBinaryTree *tree);
void destroy_tree(CompleteBinaryTree *tree);

bool insert(CompleteBinaryTree *tree, int value);
bool delete_last(CompleteBinaryTree *tree);
bool delete_value(CompleteBinaryTree *tree, int value);

int search(CompleteBinaryTree *tree, int value);
int get_min(CompleteBinaryTree *tree);
int get_max(CompleteBinaryTree *tree);

void pre_order(CompleteBinaryTree *tree, int index);
void in_order(CompleteBinaryTree *tree, int index);
void post_order(CompleteBinaryTree *tree, int index);
void level_order(CompleteBinaryTree *tree);

int get_parent_index(int child_index);
int get_left_child_index(int parent_index);
int get_right_child_index(int parent_index);

void get_parent(CompleteBinaryTree *tree, int child_index);
void get_children(CompleteBinaryTree *tree, int parent_index);

int get_height(CompleteBinaryTree *tree);
int get_levels(CompleteBinaryTree *tree);
int get_size(CompleteBinaryTree *tree);
bool is_empty(CompleteBinaryTree *tree);
bool is_full(CompleteBinaryTree *tree);
bool is_complete(CompleteBinaryTree *tree);
bool is_perfect(CompleteBinaryTree *tree);

void display_tree(CompleteBinaryTree *tree);
void display_tree_structure(CompleteBinaryTree *tree, int index, int level);

// 

CompleteBinaryTree *create_complete_tree(int capacity) {
    CompleteBinaryTree *tree = (CompleteBinaryTree *)malloc(sizeof(CompleteBinaryTree));
    if (tree == NULL) return NULL;
    
    tree->capacity = capacity;
    tree->collection = (Node *)malloc(capacity * sizeof(Node));
    tree->total_nodes = 0;
    
    if (tree->collection == NULL) {
        free(tree);
        return NULL;
    }
    
    for (int i = 0; i < capacity; i++) {
        tree->collection[i].occupied = false;
        tree->collection[i].value = 0;
    }
    
    return tree;
}

void clear_tree(CompleteBinaryTree *tree) {
    if (tree == NULL) return;
    
    for (int i = 0; i < tree->capacity; i++) {
        tree->collection[i].occupied = false;
        tree->collection[i].value = 0;
    }
    
    tree->total_nodes = 0;
}

void destroy_tree(CompleteBinaryTree *tree) {
    if (tree == NULL) return;
    
    free(tree->collection);
    free(tree);
}

bool insert(CompleteBinaryTree *tree, int value) {
    if (tree == NULL || tree->total_nodes >= tree->capacity) {
        return false;
    }
    
    tree->collection[tree->total_nodes].value = value;
    tree->collection[tree->total_nodes].occupied = true;
    tree->total_nodes++;
    
    return true;
}

bool delete_last(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) {
        return false;
    }
    
    tree->total_nodes--;
    tree->collection[tree->total_nodes].occupied = false;
    tree->collection[tree->total_nodes].value = 0;
    
    return true;
}

bool delete_value(CompleteBinaryTree *tree, int value) {
    if (tree == NULL || tree->total_nodes == 0) {
        return false;
    }
    
    int index = search(tree, value);
    if (index == -1) {
        return false;
    }
    
    tree->collection[index].value = tree->collection[tree->total_nodes - 1].value;
    
    tree->total_nodes--;
    tree->collection[tree->total_nodes].occupied = false;
    tree->collection[tree->total_nodes].value = 0;
    
    return true;
}

int search(CompleteBinaryTree *tree, int value) {
    if (tree == NULL) return -1;
    
    for (int i = 0; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied && tree->collection[i].value == value) {
            return i;
        }
    }
    
    return -1;
}

int get_min(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return -1;
    
    int min = tree->collection[0].value;
    for (int i = 1; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied && tree->collection[i].value < min) {
            min = tree->collection[i].value;
        }
    }
    
    return min;
}

int get_max(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return -1;
    
    int max = tree->collection[0].value;
    for (int i = 1; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied && tree->collection[i].value > max) {
            max = tree->collection[i].value;
        }
    }
    
    return max;
}

void pre_order(CompleteBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }
    
    printf("%d ", tree->collection[index].value);
    pre_order(tree, get_left_child_index(index));
    pre_order(tree, get_right_child_index(index));
}

void in_order(CompleteBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }
    
    in_order(tree, get_left_child_index(index));
    printf("%d ", tree->collection[index].value);
    in_order(tree, get_right_child_index(index));
}

void post_order(CompleteBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }
    
    post_order(tree, get_left_child_index(index));
    post_order(tree, get_right_child_index(index));
    printf("%d ", tree->collection[index].value);
}

void level_order(CompleteBinaryTree *tree) {
    if (tree == NULL) return;
    
    for (int i = 0; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied) {
            printf("%d ", tree->collection[i].value);
        }
    }
}

int get_parent_index(int child_index) {
    if (child_index == 0) return -1;
    return (child_index - 1) / 2;
}

int get_left_child_index(int parent_index) {
    return 2 * parent_index + 1;
}

int get_right_child_index(int parent_index) {
    return 2 * parent_index + 2;
}

void get_parent(CompleteBinaryTree *tree, int child_index) {
    if (tree == NULL || child_index >= tree->total_nodes) {
        printf("Invalid child index\n");
        return;
    }
    
    if (child_index == 0) {
        printf("Node at index 0 is the root (no parent)\n");
        return;
    }
    
    int parent_idx = get_parent_index(child_index);
    printf("Parent of node at index %d (value: %d):\n", 
           child_index, tree->collection[child_index].value);
    printf("  Parent (index %d): %d\n", parent_idx, tree->collection[parent_idx].value);
}

void get_children(CompleteBinaryTree *tree, int parent_index) {
    if (tree == NULL || parent_index >= tree->total_nodes) {
        printf("Invalid parent index\n");
        return;
    }
    
    int left_idx = get_left_child_index(parent_index);
    int right_idx = get_right_child_index(parent_index);
    
    printf("Children of node at index %d (value: %d):\n",
           parent_index, tree->collection[parent_index].value);
    
    if (left_idx < tree->total_nodes && tree->collection[left_idx].occupied) {
        printf("  Left child (index %d): %d\n", left_idx, tree->collection[left_idx].value);
    } else {
        printf("  Left child: None\n");
    }
    
    if (right_idx < tree->total_nodes && tree->collection[right_idx].occupied) {
        printf("  Right child (index %d): %d\n", right_idx, tree->collection[right_idx].value);
    } else {
        printf("  Right child: None\n");
    }
}

int get_height(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return -1;
    return (int)floor(log2(tree->total_nodes));
}

int get_levels(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return 0;
    return get_height(tree) + 1;
}

int get_size(CompleteBinaryTree *tree) {
    return tree == NULL ? 0 : tree->total_nodes;
}

bool is_empty(CompleteBinaryTree *tree) {
    return tree == NULL || tree->total_nodes == 0;
}

bool is_full(CompleteBinaryTree *tree) {
    return tree != NULL && tree->total_nodes == tree->capacity;
}

bool is_complete(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return false;
    
    for (int i = 0; i < tree->total_nodes; i++) {
        if (!tree->collection[i].occupied) {
            return false;
        }
    }
    
    return true;
}

bool is_perfect(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) return false;
    
    int height = get_height(tree);
    int expected_nodes = (int)pow(2, height + 1) - 1;
    
    return tree->total_nodes == expected_nodes;
}

void display_tree(CompleteBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0) {
        printf("Tree is empty\n");
        return;
    }
    
    printf("\nArray representation: [");
    for (int i = 0; i < tree->total_nodes; i++) {
        printf("%d", tree->collection[i].value);
        if (i < tree->total_nodes - 1) printf(", ");
    }
    printf("]\n");
}

void display_tree_structure(CompleteBinaryTree *tree, int index, int level) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }
    
    display_tree_structure(tree, get_right_child_index(index), level + 1);
    
    for (int i = 0; i < level * 4; i++) {
        printf(" ");
    }
    printf("%d\n", tree->collection[index].value);
    
    display_tree_structure(tree, get_left_child_index(index), level + 1);
}

CompleteBinaryTree *build_from_array(int *arr, int size, int capacity) {
    CompleteBinaryTree *tree = create_complete_tree(capacity);
    if (tree == NULL) return NULL;
    
    for (int i = 0; i < size; i++) {
        insert(tree, arr[i]);
    }
    
    return tree;
}

int main() {
    printf("=== Testing Complete Binary Tree Implementation ===\n\n");
    
    CompleteBinaryTree *tree = create_complete_tree(20);
    
    printf("Test 1: Building a complete binary tree...\n");
    printf("Inserting values: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10\n");
    
    for (int i = 1; i <= 10; i++) {
        insert(tree, i);
    }
    
    display_tree(tree);
    
    printf("\nTree structure:\n");
    display_tree_structure(tree, 0, 0);
    
    printf("\nTree properties:\n");
    printf("Size: %d\n", get_size(tree));
    printf("Height: %d\n", get_height(tree));
    printf("Levels: %d\n", get_levels(tree));
    printf("Is empty: %s\n", is_empty(tree) ? "Yes" : "No");
    printf("Is full: %s\n", is_full(tree) ? "Yes" : "No");
    printf("Is complete: %s\n", is_complete(tree) ? "Yes" : "No");
    printf("Is perfect: %s\n\n", is_perfect(tree) ? "Yes" : "No");
    
    printf("Test 2: Traversals\n");
    printf("Pre-order:   ");
    pre_order(tree, 0);
    printf("\n");
    
    printf("In-order:    ");
    in_order(tree, 0);
    printf("\n");
    
    printf("Post-order:  ");
    post_order(tree, 0);
    printf("\n");
    
    printf("Level-order: ");
    level_order(tree);
    printf("\n\n");
    
    printf("Test 3: Search operations\n");
    int search_val = 7;
    int index = search(tree, search_val);
    printf("Searching for %d: %s (index: %d)\n", search_val, 
           index != -1 ? "Found" : "Not found", index);
    
    search_val = 99;
    index = search(tree, search_val);
    printf("Searching for %d: %s\n\n", search_val, 
           index != -1 ? "Found" : "Not found");
    
    printf("Minimum value: %d\n", get_min(tree));
    printf("Maximum value: %d\n\n", get_max(tree));
    
    printf("Test 4: Parent-child relationships\n");
    get_children(tree, 0);
    printf("\n");
    get_children(tree, 2);
    printf("\n");
    get_parent(tree, 5);
    printf("\n");
    get_parent(tree, 9);
    printf("\n");
    
    printf("Test 5: Deletion (maintaining complete property)\n");
    printf("Deleting value 5...\n");
    delete_value(tree, 5);
    display_tree(tree);
    display_tree_structure(tree, 0, 0);
    printf("Still complete: %s\n\n", is_complete(tree) ? "Yes" : "No");
    
    printf("Deleting last element...\n");
    delete_last(tree);
    display_tree(tree);
    printf("Size after deletion: %d\n\n", get_size(tree));
    
    printf("Test 6: Creating a perfect complete binary tree\n");
    clear_tree(tree);
    
    printf("Inserting 7 nodes (2^3 - 1 = perfect tree)...\n");
    for (int i = 1; i <= 7; i++) {
        insert(tree, i * 10);
    }
    
    display_tree(tree);
    display_tree_structure(tree, 0, 0);
    
    printf("\nTree properties:\n");
    printf("Size: %d\n", get_size(tree));
    printf("Height: %d\n", get_height(tree));
    printf("Is complete: %s\n", is_complete(tree) ? "Yes" : "No");
    printf("Is perfect: %s\n", is_perfect(tree) ? "Yes" : "No");
    printf("Expected nodes for perfect tree: 2^(%d+1) - 1 = %d\n", 
           get_height(tree), (int)pow(2, get_height(tree) + 1) - 1);
    
    printf("\nTest 7: Building from array\n");
    destroy_tree(tree);
    
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25};
    tree = build_from_array(values, 9, 15);
    
    printf("Built tree from array: [50, 30, 70, 20, 40, 60, 80, 10, 25]\n");
    display_tree(tree);
    display_tree_structure(tree, 0, 0);
    
    printf("\nLevel-by-level view:\n");
    int level_count = get_levels(tree);
    for (int level = 0; level < level_count; level++) {
        int start = (int)pow(2, level) - 1;
        int end = (int)pow(2, level + 1) - 1;
        if (end > tree->total_nodes) end = tree->total_nodes;
        
        printf("Level %d: ", level);
        for (int i = start; i < end; i++) {
            printf("%d ", tree->collection[i].value);
        }
        printf("\n");
    }
    
    printf("\nTest 8: Demonstrating complete property\n");
    clear_tree(tree);
    
    printf("Inserting 1-6 (incomplete last level):\n");
    for (int i = 1; i <= 6; i++) {
        insert(tree, i);
    }
    display_tree(tree);
    display_tree_structure(tree, 0, 0);
    printf("Is complete: %s (last level filled left-to-right)\n", 
           is_complete(tree) ? "Yes" : "No");
    printf("Is perfect: %s (last level not full)\n\n", 
           is_perfect(tree) ? "Yes" : "No");
    
    destroy_tree(tree);
    printf("Complete binary tree destroyed successfully.\n");
    
    return 0;
}