// Every internal node has either 0 or 2 children.

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
} FullBinaryTree;

FullBinaryTree *create_full_tree();
void clear_full_tree(Node *root);
void destroy_full_tree(FullBinaryTree *tree);

bool insert(FullBinaryTree *tree, int value);
Node *insert_helper(Node *node, int value, bool *success);
bool delete(FullBinaryTree *tree, int value);
Node *delete_helper(Node *node, int value, bool *success);

bool search(FullBinaryTree *tree, int value);
bool search_helper(Node *node, int value);

bool is_full_node(Node *node);
bool is_full_tree(Node *root);
int count_internal_nodes(Node *root);
int count_leaf_nodes(Node *root);

void pre_order(Node *root);
void in_order(Node *root);
void post_order(Node *root);
void level_order(FullBinaryTree *tree);

int get_height(Node *node);
int get_size(FullBinaryTree *tree);
bool is_empty(FullBinaryTree *tree);

Node *find_node(Node *root, int value);
Node *get_parent(FullBinaryTree *tree, int value);
void get_children(FullBinaryTree *tree, int value);

// 

FullBinaryTree *create_full_tree() {
    FullBinaryTree *tree = (FullBinaryTree *)malloc(sizeof(FullBinaryTree));
    if (tree == NULL) return NULL;
    
    tree->root = NULL;
    tree->size = 0;

    return tree;
}

void clear_full_tree(Node *root) {
    if (root == NULL) return;
    
    clear_full_tree(root->left);
    clear_full_tree(root->right);
    free(root);
}

void destroy_full_tree(FullBinaryTree *tree) {
    if (tree == NULL) return;
    
    clear_full_tree(tree->root);
    free(tree);
}

bool insert(FullBinaryTree *tree, int value) {
    if (tree == NULL) return false;
    
    bool success = false;
    tree->root = insert_helper(tree->root, value, &success);
    
    if (success) {
        tree->size++;
    }

    return success;
}

Node *insert_helper(Node *node, int value, bool *success) {
    if (node == NULL) {
        Node *new_node = (Node *)malloc(sizeof(Node));

        if (new_node == NULL) {
            *success = false;
            return NULL;
        }

        new_node->value = value;
        new_node->left = NULL;
        new_node->right = NULL;
        *success = true;

        return new_node;
    }
    
    if (node->left == NULL && node->right == NULL) {
        node->left = insert_helper(NULL, value, success);
        if (*success) {
            bool dummy_success;
            node->right = insert_helper(NULL, value + 1, &dummy_success);
            node->right->value = value;
        }
        return node;
    }
    
    node->left = insert_helper(node->left, value, success);
    if (!*success) {
        node->right = insert_helper(node->right, value, success);
    }
    
    return node;
}

bool delete(FullBinaryTree *tree, int value) {
    if (tree == NULL || tree->root == NULL) return false;
    
    bool success = false;
    tree->root = delete_helper(tree->root, value, &success);
    
    if (success) {
        tree->size--;
    }

    return success;
}

Node *delete_helper(Node *node, int value, bool *success) {
    if (node == NULL) {
        *success = false;
        return NULL;
    }
    
    if (node->value == value) {
        if (node->left == NULL && node->right == NULL) {
            free(node);
            *success = true;

            return NULL;
        } else {
            printf("Cannot delete internal node %d without violating full binary tree property\n", value);
            *success = false;

            return node;
        }
    }
    
    node->left = delete_helper(node->left, value, success);
    if (!*success) {
        node->right = delete_helper(node->right, value, success);
    }
    
    return node;
}

bool search(FullBinaryTree *tree, int value) {
    if (tree == NULL) return false;
    return search_helper(tree->root, value);
}

bool search_helper(Node *node, int value) {
    if (node == NULL) return false;
    
    if (node->value == value) return true;
    
    return search_helper(node->left, value) || search_helper(node->right, value);
}

bool is_full_node(Node *node) {
    if (node == NULL) return true;
    
    return (node->left == NULL && node->right == NULL) ||
            (node->left != NULL && node->right != NULL);
}

bool is_full_tree(Node *root) {
    if (root == NULL) return true;
    
    if (!is_full_node(root)) return false;
    
    return is_full_tree(root->left) && is_full_tree(root->right);
}

int count_internal_nodes(Node *root) {
    if (root == NULL) return 0;
    
    if (root->left == NULL && root->right == NULL) {
        return 0;
    }
    
    return 1 + count_internal_nodes(root->left) + count_internal_nodes(root->right);
}

int count_leaf_nodes(Node *root) {
    if (root == NULL) return 0;
    
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    
    return count_leaf_nodes(root->left) + count_leaf_nodes(root->right);
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

void level_order(FullBinaryTree *tree) {
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

int get_size(FullBinaryTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

bool is_empty(FullBinaryTree *tree) {
    return tree == NULL || tree->root == NULL;
}

Node *find_node(Node *root, int value) {
    if (root == NULL) return NULL;
    
    if (root->value == value) return root;
    
    Node *left_result = find_node(root->left, value);
    if (left_result != NULL) return left_result;
    
    return find_node(root->right, value);
}

Node *get_parent(FullBinaryTree *tree, int value) {
    if (tree == NULL || tree->root == NULL || tree->root->value == value) {
        return NULL;
    }
    
    return find_parent_helper(tree->root, value);
}

Node *find_parent_helper(Node *node, int value) {
    if (node == NULL) return NULL;
    
    if ((node->left != NULL && node->left->value == value) ||
        (node->right != NULL && node->right->value == value)) {
        return node;
    }
    
    Node *left_result = find_parent_helper(node->left, value);
    if (left_result != NULL) return left_result;
    
    return find_parent_helper(node->right, value);
}

void get_children(FullBinaryTree *tree, int value) {
    if (tree == NULL) {
        printf("Tree is NULL\n");
        return;
    }
    
    Node *node = find_node(tree->root, value);
    if (node == NULL) {
        printf("Value %d not found in tree\n", value);
        return;
    }
    
    printf("Children of node %d:\n", value);
    if (node->left != NULL) {
        printf("  Left child: %d\n", node->left->value);
    } else {
        printf("  Left child: None\n");
    }
    
    if (node->right != NULL) {
        printf("  Right child: %d\n", node->right->value);
    } else {
        printf("  Right child: None\n");
    }
    
    if (is_full_node(node)) {
        printf("  This node satisfies full binary tree property\n");
    } else {
        printf("  WARNING: This node violates full binary tree property!\n");
    }
}

FullBinaryTree *create_perfect_full_tree(int levels) {
    FullBinaryTree *tree = create_full_tree();
    if (tree == NULL) return NULL;
    
    printf("Creating perfect full binary tree with %d levels...\n", levels);
    
    tree->root = create_perfect_subtree(levels, 1);
    tree->size = (1 << levels) - 1;
    
    return tree;
}

Node *create_perfect_subtree(int levels_remaining, int current_value) {
    if (levels_remaining <= 0) return NULL;
    
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    
    node->value = current_value;
    
    if (levels_remaining > 1) {
        node->left = create_perfect_subtree(levels_remaining - 1, current_value * 2);
        node->right = create_perfect_subtree(levels_remaining - 1, current_value * 2 + 1);
    } else {
        node->left = NULL;
        node->right = NULL;
    }
    
    return node;
}

int main() {
    printf("=== Testing Full (Strict) Binary Tree Implementation ===\n\n");
    
    FullBinaryTree *tree = create_full_tree();
    
    printf("Test 1: Building a full binary tree...\n");
    
    tree->root = (Node *)malloc(sizeof(Node));
    tree->root->value = 1;
    
    tree->root->left = (Node *)malloc(sizeof(Node));
    tree->root->left->value = 2;
    
    tree->root->right = (Node *)malloc(sizeof(Node));
    tree->root->right->value = 3;
    
    tree->root->left->left = (Node *)malloc(sizeof(Node));
    tree->root->left->left->value = 4;
    tree->root->left->left->left = NULL;
    tree->root->left->left->right = NULL;
    
    tree->root->left->right = (Node *)malloc(sizeof(Node));
    tree->root->left->right->value = 5;
    tree->root->left->right->left = NULL;
    tree->root->left->right->right = NULL;
    
    tree->root->right->left = (Node *)malloc(sizeof(Node));
    tree->root->right->left->value = 6;
    tree->root->right->left->left = NULL;
    tree->root->right->left->right = NULL;
    
    tree->root->right->right = (Node *)malloc(sizeof(Node));
    tree->root->right->right->value = 7;
    tree->root->right->right->left = NULL;
    tree->root->right->right->right = NULL;
    
    tree->size = 7;
    
    printf("Tree created successfully!\n");
    printf("Size: %d\n", get_size(tree));
    printf("Height: %d\n", get_height(tree->root));
    printf("Is full tree: %s\n", is_full_tree(tree->root) ? "Yes" : "No");
    printf("Internal nodes: %d\n", count_internal_nodes(tree->root));
    printf("Leaf nodes: %d\n", count_leaf_nodes(tree->root));
    
    int internal = count_internal_nodes(tree->root);
    int leaves = count_leaf_nodes(tree->root);
    printf("Verification (L = I + 1): %d = %d + 1 -> %s\n\n", 
        leaves, internal, leaves == internal + 1 ? "True" : "False");
    
    printf("Pre-order: ");
    pre_order(tree->root);
    printf("\n");
    
    printf("In-order: ");
    in_order(tree->root);
    printf("\n");
    
    printf("Post-order: ");
    post_order(tree->root);
    printf("\n");
    
    printf("Level-order: ");
    level_order(tree);
    printf("\n\n");
    
    printf("Testing parent-child relationships:\n");
    get_children(tree, 1);
    get_children(tree, 2);
    get_children(tree, 4);
    printf("\n");
    
    printf("Searching for 5: %s\n", search(tree, 5) ? "Found" : "Not found");
    printf("Searching for 8: %s\n\n", search(tree, 8) ? "Found" : "Not found");
    
    printf("Testing deletion:\n");
    printf("Deleting leaf node 7...\n");
    bool deleted = delete(tree, 7);
    printf("Deletion %s\n", deleted ? "successful" : "failed");
    
    if (deleted) {
        printf("Tree after deletion - Is full: %s\n", is_full_tree(tree->root) ? "Yes" : "No");
        printf("Level-order after deletion: ");
        level_order(tree);
        printf("\n");
    }
    
    printf("\nTest 2: Creating a larger perfect full binary tree...\n");
    destroy_full_tree(tree);
    
    tree = create_perfect_full_tree(4);
    if (tree != NULL) {
        printf("Size: %d\n", get_size(tree));
        printf("Height: %d\n", get_height(tree->root));
        printf("Is full tree: %s\n", is_full_tree(tree->root) ? "Yes" : "No");
        
        internal = count_internal_nodes(tree->root);
        leaves = count_leaf_nodes(tree->root);
        printf("Internal nodes: %d, Leaf nodes: %d\n", internal, leaves);
        printf("Verification (L = I + 1): %d = %d + 1 -> %s\n", 
            leaves, internal, leaves == internal + 1 ? "True" : "False");
        
        printf("Level-order: ");
        level_order(tree);
        printf("\n");
    }
    
    destroy_full_tree(tree);
    printf("\nFull binary tree destroyed successfully.\n");
    
    return 0;
}