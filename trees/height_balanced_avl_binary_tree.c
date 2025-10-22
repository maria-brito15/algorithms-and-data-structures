// For every node, the heights of the left and right subtrees differ by at most 1 (e.g., AVL tree).

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    int size;
} AVLTree;

//

AVLTree *create_avl();
void clear_avl(Node *root);
void destroy_avl(AVLTree *tree);

int height(Node *node);
int max(int a, int b);
int get_balance(Node *node);

Node *right_rotate(Node *p);
Node *left_rotate(Node *c);
Node *rotate(Node *node);

bool insert(AVLTree *tree, int value);
Node *insert_helper(Node *node, int value, bool *success);

bool delete_node(AVLTree *tree, int value);
Node *delete_helper(Node *node, int value, bool *success);

bool search(AVLTree *tree, int value);
Node *find_min(Node *node);
Node *find_max(Node *node);

void pre_order(Node *root);
void in_order(Node *root);
void post_order(Node *root);
void level_order(AVLTree *tree);

int get_size(AVLTree *tree);
bool is_empty(AVLTree *tree);
bool is_balanced(Node *root);
void display_tree(Node *root, int space);

//

AVLTree *create_avl() {
    AVLTree *tree = (AVLTree *)malloc(sizeof(AVLTree));
    if (tree == NULL) return NULL;
    
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void clear_avl(Node *root) {
    if (root == NULL) return;
    
    clear_avl(root->left);
    clear_avl(root->right);
    free(root);
}

void destroy_avl(AVLTree *tree) {
    if (tree == NULL) return;
    
    clear_avl(tree->root);
    free(tree);
}

int height(Node *node) {
    if (node == NULL) return -1;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_balance(Node *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

Node *right_rotate(Node *p) {
    Node *c = p->left;
    Node *t2 = c->right;
    
    c->right = p;
    p->left = t2;
    
    p->height = max(height(p->left), height(p->right)) + 1;
    c->height = max(height(c->left), height(c->right)) + 1;
    
    return c;
}

Node *left_rotate(Node *c) {
    Node *p = c->right;
    Node *t2 = p->left;
    
    p->left = c;
    c->right = t2;
    
    c->height = max(height(c->left), height(c->right)) + 1;
    p->height = max(height(p->left), height(p->right)) + 1;
    
    return p;
}

Node *rotate(Node *node) {
    int balance = get_balance(node);
    
    // Left Heavy (balance > 1)
    if (balance > 1) {
        // Left-Left Case (LL)
        if (get_balance(node->left) >= 0) {
            return right_rotate(node);
        }
        // Left-Right Case (LR)
        if (get_balance(node->left) < 0) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }
    
    // Right Heavy (balance < -1)
    if (balance < -1) {
        // Right-Right Case (RR)
        if (get_balance(node->right) <= 0) {
            return left_rotate(node);
        }
        // Right-Left Case (RL)
        if (get_balance(node->right) > 0) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
    }
    
    return node;
}

bool insert(AVLTree *tree, int value) {
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
        new_node->height = 0;
        new_node->left = NULL;
        new_node->right = NULL;
		
        *success = true;
		
        return new_node;
    }
    
    if (value < node->value) {
        node->left = insert_helper(node->left, value, success);
    } else if (value > node->value) {
        node->right = insert_helper(node->right, value, success);
    } else {
        *success = false;
        return node;
    }
    
    node->height = max(height(node->left), height(node->right)) + 1;
    
    return rotate(node);
}

bool delete_node(AVLTree *tree, int value) {
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
    
    if (value < node->value) {
        node->left = delete_helper(node->left, value, success);
    } else if (value > node->value) {
        node->right = delete_helper(node->right, value, success);
    } else {
        *success = true;
        
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
        node->right = delete_helper(node->right, temp->value, success);
    }
    
    if (node == NULL) return node;
    
    node->height = max(height(node->left), height(node->right)) + 1;
    
    return rotate(node);
}

bool search(AVLTree *tree, int value) {
    if (tree == NULL) return false;
    
    Node *current = tree->root;
    while (current != NULL) {
        if (value == current->value) {
            return true;
        } else if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return false;
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

void level_order(AVLTree *tree) {
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

int get_size(AVLTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

bool is_empty(AVLTree *tree) {
    return tree == NULL || tree->root == NULL;
}

bool is_balanced(Node *root) {
    if (root == NULL) return true;
    
    int balance = get_balance(root);
    if (balance < -1 || balance > 1) return false;
    
    return is_balanced(root->left) && is_balanced(root->right);
}

void display_tree(Node *root, int space) {
    if (root == NULL) return;
    
    space += 5;
    
    display_tree(root->right, space);
    
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }
    printf("%d(h:%d,b:%d)\n", root->value, root->height, get_balance(root));
    
    display_tree(root->left, space);
}

int main() {
    printf("=== Testing AVL Tree (Self-Balancing BST) ===\n\n");
    
    AVLTree *avl = create_avl();
    
    printf("Test 1: Inserting sorted data (worst case for regular BST)...\n");
    printf("Inserting: 5, 7, 9, 11, 13\n\n");
    
    int sorted[] = {5, 7, 9, 11, 13};
    for (int i = 0; i < 5; i++) {
        insert(avl, sorted[i]);
    }
    
    printf("Tree structure (value(height:balance)):\n");
    display_tree(avl->root, 0);
    
    printf("\nSize: %d\n", get_size(avl));
    printf("Height: %d (would be 5 in unbalanced BST)\n", height(avl->root));
    printf("Is balanced: %s\n\n", is_balanced(avl->root) ? "Yes" : "No");
    
    printf("In-order traversal (should be sorted): ");
    in_order(avl->root);
    printf("\n\n");
    
    printf("Test 2: Demonstrating all 4 rotation cases...\n");
    destroy_avl(avl);
    avl = create_avl();
    
    printf("\nCase 1 - LL (Left-Left): Insert 30, 20, 10\n");
    insert(avl, 30);
    insert(avl, 20);
    insert(avl, 10);
    printf("Result after right rotation:\n");
    display_tree(avl->root, 0);
    
    printf("\nCase 2 - RR (Right-Right): Insert 40, 50\n");
    insert(avl, 40);
    insert(avl, 50);
    printf("Result after left rotation:\n");
    display_tree(avl->root, 0);
    
    printf("\nCase 3 - LR (Left-Right): Insert 5, 15\n");
    insert(avl, 5);
    insert(avl, 15);
    printf("Result after double rotation:\n");
    display_tree(avl->root, 0);
    
    printf("\nTest 3: Large dataset test (1000 elements)...\n");
    destroy_avl(avl);
    avl = create_avl();
    
    for (int i = 0; i < 1000; i++) {
        insert(avl, i);
    }
    
    printf("Inserted 1000 elements in sorted order\n");
    printf("Size: %d\n", get_size(avl));
    printf("Height: %d (log₂(1000) ≈ 10)\n", height(avl->root));
    printf("Is balanced: %s\n", is_balanced(avl->root) ? "Yes" : "No");
    printf("This proves AVL maintains O(log n) height!\n\n");
    
    printf("Test 4: Search operations...\n");
    printf("Searching for 500: %s\n", search(avl, 500) ? "Found" : "Not found");
    printf("Searching for 1001: %s\n\n", search(avl, 1001) ? "Found" : "Not found");
    
    Node *min = find_min(avl->root);
    Node *max = find_max(avl->root);
    printf("Minimum value: %d\n", min ? min->value : -1);
    printf("Maximum value: %d\n\n", max ? max->value : -1);
    
    printf("Test 5: Deletion...\n");
    destroy_avl(avl);
    avl = create_avl();
    
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        insert(avl, values[i]);
    }
    
    printf("Initial tree:\n");
    display_tree(avl->root, 0);
    
    printf("\nDeleting 20 (leaf node)...\n");
    delete_node(avl, 20);
    display_tree(avl->root, 0);
    
    printf("\nDeleting 30 (node with one child)...\n");
    delete_node(avl, 30);
    display_tree(avl->root, 0);
    
    printf("\nDeleting 50 (root with two children)...\n");
    delete_node(avl, 50);
    display_tree(avl->root, 0);
    
    printf("\nFinal size: %d\n", get_size(avl));
    printf("Final height: %d\n", height(avl->root));
    printf("Still balanced: %s\n", is_balanced(avl->root) ? "Yes" : "No");
    
    destroy_avl(avl);
    printf("\nAVL tree destroyed successfully.\n");
    
    return 0;
}