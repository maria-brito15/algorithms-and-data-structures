// All internal nodes have 2 children, and all leaves are on the same level.

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
} PerfectBinaryTree;

PerfectBinaryTree *create_perfect_tree(int capacity);
void clear(PerfectBinaryTree *tree);
void destroy(PerfectBinaryTree *tree);

bool insert(PerfectBinaryTree *tree, int value);
int search(PerfectBinaryTree *tree, int value);

void pre_order(PerfectBinaryTree *tree, int index);
void in_order(PerfectBinaryTree *tree, int index);
void post_order(PerfectBinaryTree *tree, int index);
void level_order(PerfectBinaryTree *tree, int index);

void get_children(PerfectBinaryTree *tree, int parent_index);
void get_parent(PerfectBinaryTree *tree, int child_index);

int get_levels(PerfectBinaryTree *tree);
int get_height(PerfectBinaryTree *tree);
int get_leaf_nodes(PerfectBinaryTree *tree);

bool is_empty(PerfectBinaryTree *tree);
bool is_full(PerfectBinaryTree *tree);
bool is_perfect(PerfectBinaryTree *tree);
bool is_complete(PerfectBinaryTree *tree);

// 

PerfectBinaryTree *create_perfect_tree(int capacity) {
    PerfectBinaryTree *tree = (PerfectBinaryTree *)malloc(sizeof(PerfectBinaryTree));
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

void clear(PerfectBinaryTree *tree) {
    if (tree == NULL) return;
    
    for (int i = 0; i < tree->capacity; i++) {
        tree->collection[i].occupied = false;
        tree->collection[i].value = 0;
    }

    tree->total_nodes = 0;
}

void destroy(PerfectBinaryTree *tree) {
    if (tree == NULL) return;
    
    free(tree->collection);
    free(tree);
}

bool insert(PerfectBinaryTree *tree, int value) {
    if (tree == NULL || tree->total_nodes >= tree->capacity) {
        return false;
    }
    
    tree->collection[tree->total_nodes].value = value;
    tree->collection[tree->total_nodes].occupied = true;
    tree->total_nodes++;

    return true;
}

int search(PerfectBinaryTree *tree, int value) {
    if (tree == NULL) return -1;
    
    for (int i = 0; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied && tree->collection[i].value == value) {
            return i;
        }
    }

    return -1;
}

void pre_order(PerfectBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }
    
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;

    printf("%d ", tree->collection[index].value);
    pre_order(tree, left_child);
    pre_order(tree, right_child);
}

void in_order(PerfectBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }

    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    
    in_order(tree, left_child);
    printf("%d ", tree->collection[index].value);
    in_order(tree, right_child);
}

void post_order(PerfectBinaryTree *tree, int index) {
    if (tree == NULL || index >= tree->total_nodes || !tree->collection[index].occupied) {
        return;
    }

    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    
    post_order(tree, left_child);
    post_order(tree, right_child);
    printf("%d ", tree->collection[index].value);
}

void level_order(PerfectBinaryTree *tree, int index) {
    if (tree == NULL) return;
    
    for (int i = index; i < tree->total_nodes; i++) {
        if (tree->collection[i].occupied) {
            printf("%d ", tree->collection[i].value);
        }
    }
}

void get_children(PerfectBinaryTree *tree, int parent_index) {
    if (tree == NULL || parent_index >= tree->total_nodes) {
        printf("Invalid parent index\n");
        return;
    }
    
    int left_child = 2 * parent_index + 1;
    int right_child = 2 * parent_index + 2;
    
    printf("Children of node at index %d (value: %d):\n", 
        parent_index, tree->collection[parent_index].value);
    
    if (left_child < tree->total_nodes && tree->collection[left_child].occupied) {
        printf("  Left child (index %d): %d\n", left_child, tree->collection[left_child].value);
    } else {
        printf("  Left child: None\n");
    }
    
    if (right_child < tree->total_nodes && tree->collection[right_child].occupied) {
        printf("  Right child (index %d): %d\n", right_child, tree->collection[right_child].value);
    } else {
        printf("  Right child: None\n");
    }
}

void get_parent(PerfectBinaryTree *tree, int child_index) {
    if (tree == NULL || child_index >= tree->total_nodes || child_index == 0) {
        if (child_index == 0) {
            printf("Node at index 0 is the root (no parent)\n");
        } else {
            printf("Invalid child index\n");
        }
        return;
    }
    
    int parent = (child_index - 1) / 2;

    printf("Parent of node at index %d (value: %d):\n", 
        child_index, tree->collection[child_index].value);
    printf("  Parent (index %d): %d\n", parent, tree->collection[parent].value);
}

int get_levels(PerfectBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0)
        return 0;

    return (int)floor(log2(tree->total_nodes)) + 1;
}

int get_height(PerfectBinaryTree *tree) {
    int levels = get_levels(tree);
    return levels > 0 ? levels - 1 : 0;
}

int get_leaf_nodes(PerfectBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0)
        return 0;
    
    int height = get_height(tree);
    return (int)pow(2, height);
}

bool is_empty(PerfectBinaryTree *tree) {
    return tree == NULL || tree->total_nodes == 0;
}

bool is_full(PerfectBinaryTree *tree) {
    return tree != NULL && tree->total_nodes == tree->capacity;
}

bool is_perfect(PerfectBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0)
        return false;

    int height = get_height(tree);
    int expected_nodes = (int)pow(2, height + 1) - 1;

    return tree->total_nodes == expected_nodes;
}

bool is_complete(PerfectBinaryTree *tree) {
    if (tree == NULL || tree->total_nodes == 0)
        return false;

    for (int i = 0; i < tree->total_nodes; i++) {
        if (!tree->collection[i].occupied) {
            return false;
        }
    }

    return true;
}

int main() {
    PerfectBinaryTree *tree = create_perfect_tree(15);
    
    printf("=== Testing Binary Tree Implementation ===\n\n");
    
    printf("Inserting values 1-10...\n");
    for (int i = 1; i <= 10; i++) {
        insert(tree, i * 10);
    }
    
    printf("Total nodes: %d\n", tree->total_nodes);
    printf("Levels: %d\n", get_levels(tree));
    printf("Height: %d\n", get_height(tree));
    printf("Leaf nodes: %d\n\n", get_leaf_nodes(tree));
    
    printf("Pre-order: ");
    pre_order(tree, 0);
    printf("\n");
    
    printf("In-order: ");
    in_order(tree, 0);
    printf("\n");
    
    printf("Post-order: ");
    post_order(tree, 0);
    printf("\n");
    
    printf("Level-order: ");
    level_order(tree, 0);
    printf("\n\n");
    
    printf("Searching for 50: %s\n", search(tree, 50) != -1 ? "Found" : "Not found");
    printf("Searching for 99: %s\n\n", search(tree, 99) != -1 ? "Found" : "Not found");    
    
    get_children(tree, 0);
    printf("\n");
    get_children(tree, 2);
    printf("\n");
    get_parent(tree, 5);
    printf("\n");
    
    printf("Is empty: %s\n", is_empty(tree) ? "Yes" : "No");
    printf("Is full: %s\n", is_full(tree) ? "Yes" : "No");
    printf("Is perfect: %s\n", is_perfect(tree) ? "Yes" : "No");
    printf("Is complete: %s\n\n", is_complete(tree) ? "Yes" : "No");
    
    clear(tree);
    printf("Creating perfect tree with 7 nodes...\n");

    for (int i = 1; i <= 7; i++) {
        insert(tree, i * 5);
    }

    printf("Total nodes: %d\n", tree->total_nodes);
    printf("Height: %d\n", get_height(tree));
    printf("Leaf nodes (2^h): %d\n", get_leaf_nodes(tree));
    printf("Is perfect (N = 2^(h+1) - 1): %s\n", is_perfect(tree) ? "Yes" : "No");
    printf("Is complete: %s\n\n", is_complete(tree) ? "Yes" : "No");
    
    clear(tree);
    printf("After clear - Total nodes: %d\n", tree->total_nodes);
    printf("Is empty: %s\n", is_empty(tree) ? "Yes" : "No");
    
    destroy(tree);
    printf("\nTree destroyed successfully.\n");
    
    return 0;
}