// A binary tree that follows an ordering rule, e.g., Binary Search Tree (BST) where left < root < right.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    int size;
} BinarySearchTree;

BinarySearchTree *create_bst();
void clear_bst(Node *root);
void destroy_bst(BinarySearchTree *tree);

bool insert(BinarySearchTree *tree, int value);
Node *insert_helper(Node *node, int value);
bool search(BinarySearchTree *tree, int value);
bool search_helper(Node *node, int value);
bool delete(BinarySearchTree *tree, int value);
Node *delete_helper(Node *node, int value);

Node *find_min(Node *node);
Node *find_max(Node *node);

void pre_order(Node *root);
void in_order(Node *root);
void post_order(Node *root);
void level_order(BinarySearchTree *tree);

int get_height(Node *node);
int get_size(BinarySearchTree *tree);
bool is_empty(BinarySearchTree *tree);

Node *get_parent(BinarySearchTree *tree, int value);
void get_children(BinarySearchTree *tree, int value);

// 

BinarySearchTree *create_bst() {
    BinarySearchTree *tree = (BinarySearchTree *)malloc(sizeof(BinarySearchTree));
    if (tree == NULL) return NULL;
    
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void clear_bst(Node *root) {
    if (root == NULL) return;
    
    clear_bst(root->left);
    clear_bst(root->right);
    free(root);
}

void destroy_bst(BinarySearchTree *tree) {
    if (tree == NULL) return;
    
    clear_bst(tree->root);
    free(tree);
}

bool insert(BinarySearchTree *tree, int value) {
    if (tree == NULL) return false;
    
    tree->root = insert_helper(tree->root, value);

    if (tree->root != NULL) {
        tree->size++;
        return true;
    }

    return false;
}

Node *insert_helper(Node *node, int value) {
    if (node == NULL) {
        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL) return NULL;
        
        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
    
    if (value < node->value) {
        node->left = insert_helper(node->left, value);
    } else if (value > node->value) {
        node->right = insert_helper(node->right, value);
    }
    
    return node;
}

bool search(BinarySearchTree *tree, int value) {
    if (tree == NULL) return false;
    return search_helper(tree->root, value);
}

bool search_helper(Node *node, int value) {
    if (node == NULL) return false;
    
    if (value == node->value) {
        return true;
    } else if (value < node->value) {
        return search_helper(node->left, value);
    } else {
        return search_helper(node->right, value);
    }
}

bool delete(BinarySearchTree *tree, int value) {
    if (tree == NULL || tree->root == NULL) return false;
    
    if (search(tree, value)) {
        tree->root = delete_helper(tree->root, value);
        tree->size--;

        return true;
    }

    return false;
}

Node *delete_helper(Node *node, int value) {
    if (node == NULL) return NULL;
    
    if (value < node->value) {
        node->left = delete_helper(node->left, value);
    } else if (value > node->value) {
        node->right = delete_helper(node->right, value);
    } else {
        if (node->left == NULL) {
            Node *temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node *temp = node->left;
            free(node);
            return temp;
        }

        Node *temp = find_min(node->right);
        node->value = temp->value;
        node->right = delete_helper(node->right, temp->value);
    }

    return node;
}

Node *find_min(Node *node) {
    if (node == NULL) return NULL;

    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

Node *find_max(Node *node) {
    if (node == NULL) return NULL;

    while (node->right != NULL) {
        node = node->right;
    }

    return node;
}

void pre_order(Node *root) {
    if (root == NULL) return;
    
    printf("%d ", root->value);
    pre_order(root->left);
    pre_order(root->right);
}

void in_order(Node *root) {
    if (root == NULL) return;
    
    in_order(root->left);
    printf("%d ", root->value);
    in_order(root->right);
}

void post_order(Node *root) {
    if (root == NULL) return;
    
    post_order(root->left);
    post_order(root->right);
    printf("%d ", root->value);
}

void level_order(BinarySearchTree *tree) {
    if (tree == NULL || tree->root == NULL) return;
    
    Node **queue = (Node **)malloc(tree->size * sizeof(Node *));
    if (queue == NULL) return;
    
    int front = 0, rear = 0;
    queue[rear++] = tree->root;
    
    while (front < rear) {
        Node *current = queue[front++];
        printf("%d ", current->value);
        
        if (current->left != NULL) {
            queue[rear++] = current->left;
        }
        if (current->right != NULL) {
            queue[rear++] = current->right;
        }
    }

    free(queue);
}

int get_height(Node *node) {
    if (node == NULL) return -1;
    
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    
    return (left_height > right_height ? left_height : right_height) + 1;
}

int get_size(BinarySearchTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

bool is_empty(BinarySearchTree *tree) {
    return tree == NULL || tree->root == NULL;
}

Node *get_parent(BinarySearchTree *tree, int value) {
    if (tree == NULL || tree->root == NULL || tree->root->value == value) {
        return NULL;
    }
    
    Node *current = tree->root;
    Node *parent = NULL;
    
    while (current != NULL && current->value != value) {
        parent = current;

        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return current != NULL ? parent : NULL;
}

void get_children(BinarySearchTree *tree, int value) {
    if (tree == NULL) {
        printf("Tree is NULL\n");
        return;
    }
    
    Node *current = tree->root;

    while (current != NULL && current->value != value) {
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    if (current == NULL) {
        printf("Value %d not found in tree\n", value);
        return;
    }
    
    printf("Children of node with value %d:\n", value);
    if (current->left != NULL) {
        printf("  Left child: %d\n", current->left->value);
    } else {
        printf("  Left child: None\n");
    }
    
    if (current->right != NULL) {
        printf("  Right child: %d\n", current->right->value);
    } else {
        printf("  Right child: None\n");
    }
}

int main() {
    printf("=== Testing Binary Search Tree Implementation ===\n\n");
    
    BinarySearchTree *bst = create_bst();
    
    printf("Inserting values: 50, 30, 70, 20, 40, 60, 80\n");
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        insert(bst, values[i]);
    }
    
    printf("Size: %d\n", get_size(bst));
    printf("Height: %d\n", get_height(bst->root));
    printf("Is empty: %s\n\n", is_empty(bst) ? "Yes" : "No");
    
    printf("Pre-order: ");
    pre_order(bst->root);
    printf("\n");
    
    printf("In-order: ");
    in_order(bst->root);
    printf("\n");
    
    printf("Post-order: ");
    post_order(bst->root);
    printf("\n");
    
    printf("Level-order: ");
    level_order(bst);
    printf("\n\n");
    
    printf("Searching for 40: %s\n", search(bst, 40) ? "Found" : "Not found");
    printf("Searching for 90: %s\n\n", search(bst, 90) ? "Found" : "Not found");
    
    Node *min_node = find_min(bst->root);
    Node *max_node = find_max(bst->root);
    printf("Minimum value: %d\n", min_node != NULL ? min_node->value : -1);
    printf("Maximum value: %d\n\n", max_node != NULL ? max_node->value : -1);
    
    get_children(bst, 30);
    printf("\n");
    
    Node *parent = get_parent(bst, 40);
    if (parent != NULL) {
        printf("Parent of 40: %d\n\n", parent->value);
    } else {
        printf("40 is root or not found\n\n");
    }
    
    printf("Deleting 20 (leaf node)...\n");
    delete(bst, 20);
    printf("In-order after deletion: ");
    in_order(bst->root);
    printf("\n");
    
    printf("Deleting 30 (node with one child)...\n");
    delete(bst, 30);
    printf("In-order after deletion: ");
    in_order(bst->root);
    printf("\n");
    
    printf("Deleting 50 (node with two children)...\n");
    delete(bst, 50);
    printf("In-order after deletion: ");
    in_order(bst->root);
    printf("\n\n");
    
    printf("Final size: %d\n", get_size(bst));
    printf("Final height: %d\n", get_height(bst->root));
    
    destroy_bst(bst);
    printf("\nBST destroyed successfully.\n");
    
    return 0;
}