#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right;
} Node;

typedef struct RedBlackTree {
    Node *root;
    Node *NIL;
} RedBlackTree;

// ==================== UTILITY FUNCTIONS ====================

bool isRed(RedBlackTree *tree, Node *node) {
    return node != tree->NIL && node->color == RED;
}

bool isBlack(RedBlackTree *tree, Node *node) {
    return node == tree->NIL || node->color == BLACK;
}

Node* createNode(RedBlackTree *tree, int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    return node;
}

RedBlackTree* createTree() {
    RedBlackTree *tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    tree->NIL = (Node*)malloc(sizeof(Node));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = NULL;
    tree->root = tree->NIL;
    return tree;
}

// ==================== ROTATION FUNCTIONS ====================

Node* leftRotate(Node *pivot) {
    Node *rightChild = pivot->right;
    pivot->right = rightChild->left;
    rightChild->left = pivot;
    return rightChild;
}

Node* rightRotate(Node *pivot) {
    Node *leftChild = pivot->left;
    pivot->left = leftChild->right;
    leftChild->right = pivot;
    return leftChild;
}

// ==================== INSERTION ====================

void recolorNode(Node *node) {
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
}

Node* fixLeftInsertViolation(RedBlackTree *tree, Node *node) {
    if (isRed(tree, node->left->left)) {
        // Left-Left case
        node = rightRotate(node);
    } else {
        // Left-Right case
        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }
    node->color = BLACK;
    node->right->color = RED;
    return node;
}

Node* fixRightInsertViolation(RedBlackTree *tree, Node *node) {
    if (isRed(tree, node->right->right)) {
        // Right-Right case
        node = leftRotate(node);
    } else {
        // Right-Left case
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }
    node->color = BLACK;
    node->left->color = RED;
    return node;
}

Node* fixInsertViolations(RedBlackTree *tree, Node *node) {
    // Fix left side violations
    if (isRed(tree, node->left) && 
        (isRed(tree, node->left->left) || isRed(tree, node->left->right))) {
        if (isRed(tree, node->right)) {
            recolorNode(node);
        } else {
            node = fixLeftInsertViolation(tree, node);
        }
    }
    
    // Fix right side violations
    if (isRed(tree, node->right) && 
        (isRed(tree, node->right->right) || isRed(tree, node->right->left))) {
        if (isRed(tree, node->left)) {
            recolorNode(node);
        } else {
            node = fixRightInsertViolation(tree, node);
        }
    }
    
    return node;
}

Node* insertRecursive(RedBlackTree *tree, Node *node, int data, bool *inserted) {
    if (node == tree->NIL) {
        *inserted = true;
        return createNode(tree, data);
    }
    
    if (data < node->data) {
        node->left = insertRecursive(tree, node->left, data, inserted);
    } else if (data > node->data) {
        node->right = insertRecursive(tree, node->right, data, inserted);
    } else {
        return node; // Duplicate
    }
    
    if (*inserted) {
        node = fixInsertViolations(tree, node);
    }
    
    return node;
}

void insert(RedBlackTree *tree, int data) {
    bool inserted = false;
    tree->root = insertRecursive(tree, tree->root, data, &inserted);
    tree->root->color = BLACK;
}

// ==================== DELETION ====================

Node* findMinimum(RedBlackTree *tree, Node *node) {
    while (node->left != tree->NIL)
        node = node->left;
    return node;
}

Node* findMaximum(RedBlackTree *tree, Node *node) {
    while (node->right != tree->NIL)
        node = node->right;
    return node;
}

Node* fixDeleteLeftCase(RedBlackTree *tree, Node *node) {
    Node *sibling = node->right;
    
    // Case 1: Red sibling
    if (isRed(tree, sibling)) {
        node = leftRotate(node);
        node->color = BLACK;
        node->left->color = RED;
        sibling = node->left->right;
    }
    
    // Case 2: Black sibling with two black children
    if (isBlack(tree, sibling->left) && isBlack(tree, sibling->right)) {
        sibling->color = RED;
        return node;
    }
    
    // Case 3: Black sibling with red left child and black right child
    if (isBlack(tree, sibling->right)) {
        sibling = rightRotate(sibling);
        sibling->color = BLACK;
        sibling->right->color = RED;
        node->right = sibling;
    }
    
    // Case 4: Black sibling with red right child
    node = leftRotate(node);
    node->color = node->left->color;
    node->left->color = BLACK;
    node->right->color = BLACK;
    
    return node;
}

Node* fixDeleteRightCase(RedBlackTree *tree, Node *node) {
    Node *sibling = node->left;
    
    // Case 1: Red sibling
    if (isRed(tree, sibling)) {
        node = rightRotate(node);
        node->color = BLACK;
        node->right->color = RED;
        sibling = node->right->left;
    }
    
    // Case 2: Black sibling with two black children
    if (isBlack(tree, sibling->left) && isBlack(tree, sibling->right)) {
        sibling->color = RED;
        return node;
    }
    
    // Case 3: Black sibling with red right child and black left child
    if (isBlack(tree, sibling->left)) {
        sibling = leftRotate(sibling);
        sibling->color = BLACK;
        sibling->left->color = RED;
        node->left = sibling;
    }
    
    // Case 4: Black sibling with red left child
    node = rightRotate(node);
    node->color = node->right->color;
    node->right->color = BLACK;
    node->left->color = BLACK;
    
    return node;
}

Node* deleteNode(RedBlackTree *tree, Node *node, bool *done) {
    if (node->left == tree->NIL || node->right == tree->NIL) {
        // Node has at most one child
        Node *child = (node->left != tree->NIL) ? node->left : node->right;
        *done = isRed(tree, node);
        free(node);
        return child;
    }
    
    // Node has two children - replace with successor
    Node *successor = findMinimum(tree, node->right);
    node->data = successor->data;
    node->right = deleteRecursive(tree, node->right, successor->data, done);
    
    if (!*done) {
        node = fixDeleteRightCase(tree, node);
    }
    
    return node;
}

Node* deleteRecursive(RedBlackTree *tree, Node *node, int data, bool *done) {
    if (node == tree->NIL)
        return node;
    
    if (data < node->data) {
        node->left = deleteRecursive(tree, node->left, data, done);
        if (!*done)
            node = fixDeleteLeftCase(tree, node);
    } else if (data > node->data) {
        node->right = deleteRecursive(tree, node->right, data, done);
        if (!*done)
            node = fixDeleteRightCase(tree, node);
    } else {
        node = deleteNode(tree, node, done);
    }
    
    return node;
}

void deleteValue(RedBlackTree *tree, int data) {
    bool done = false;
    tree->root = deleteRecursive(tree, tree->root, data, &done);
    if (tree->root != tree->NIL)
        tree->root->color = BLACK;
}

// ==================== SEARCH OPERATIONS ====================

Node* search(RedBlackTree *tree, int data) {
    Node *current = tree->root;
    while (current != tree->NIL && current->data != data) {
        current = (data < current->data) ? current->left : current->right;
    }
    return (current != tree->NIL) ? current : NULL;
}

Node* findMin(RedBlackTree *tree) {
    return (tree->root == tree->NIL) ? NULL : findMinimum(tree, tree->root);
}

Node* findMax(RedBlackTree *tree) {
    return (tree->root == tree->NIL) ? NULL : findMaximum(tree, tree->root);
}

// ==================== SUCCESSOR/PREDECESSOR ====================

Node* findSuccessor(RedBlackTree *tree, Node *node, int data, Node *successor) {
    if (node == tree->NIL)
        return successor;
    
    if (data < node->data)
        return findSuccessor(tree, node->left, data, node);
    else if (data > node->data)
        return findSuccessor(tree, node->right, data, successor);
    else
        return (node->right != tree->NIL) ? findMinimum(tree, node->right) : successor;
}

Node* successor(RedBlackTree *tree, Node *node) {
    if (node == NULL) return NULL;
    Node *result = findSuccessor(tree, tree->root, node->data, tree->NIL);
    return (result != tree->NIL) ? result : NULL;
}

Node* findPredecessor(RedBlackTree *tree, Node *node, int data, Node *predecessor) {
    if (node == tree->NIL)
        return predecessor;
    
    if (data < node->data)
        return findPredecessor(tree, node->left, data, predecessor);
    else if (data > node->data)
        return findPredecessor(tree, node->right, data, node);
    else
        return (node->left != tree->NIL) ? findMaximum(tree, node->left) : predecessor;
}

Node* predecessor(RedBlackTree *tree, Node *node) {
    if (node == NULL) return NULL;
    Node *result = findPredecessor(tree, tree->root, node->data, tree->NIL);
    return (result != tree->NIL) ? result : NULL;
}

// ==================== TRAVERSAL ====================

void inorder(RedBlackTree *tree, Node *node) {
    if (node != tree->NIL) {
        inorder(tree, node->left);
        printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        inorder(tree, node->right);
    }
}

// ==================== MEMORY CLEANUP ====================

void freeTree(RedBlackTree *tree, Node *node) {
    if (node != tree->NIL) {
        freeTree(tree, node->left);
        freeTree(tree, node->right);
        free(node);
    }
}

void destroyTree(RedBlackTree *tree) {
    freeTree(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

// ==================== MAIN FUNCTION ====================

int main() {
    RedBlackTree *tree = createTree();
    
    int values[] = {10, 20, 30, 15, 25, 5, 1};
    int n = sizeof(values) / sizeof(values[0]);
    
    printf("=== INSERTION ===\n");
    for (int i = 0; i < n; i++) {
        insert(tree, values[i]);
        printf("After inserting %d: ", values[i]);
        inorder(tree, tree->root);
        printf("\n");
    }
    
    printf("\n=== SEARCH ===\n");
    Node *found = search(tree, 15);
    if (found)
        printf("Found: %d\n", found->data);
    
    printf("\n=== MIN/MAX ===\n");
    Node *minNode = findMin(tree);
    Node *maxNode = findMax(tree);
    printf("Min: %d, Max: %d\n", minNode->data, maxNode->data);
    
    printf("\n=== SUCCESSOR/PREDECESSOR ===\n");
    Node *node = search(tree, 15);
    if (node) {
        Node *succ = successor(tree, node);
        Node *pred = predecessor(tree, node);
        printf("Successor of 15: %s\n", succ ? 
            (char[20]){0} : "null");
        if (succ) printf("Successor of 15: %d\n", succ->data);
        else printf("Successor of 15: null\n");
        
        if (pred) printf("Predecessor of 15: %d\n", pred->data);
        else printf("Predecessor of 15: null\n");
    }
    
    printf("\n=== DELETION ===\n");
    deleteValue(tree, 20);
    printf("After deleting 20: ");
    inorder(tree, tree->root);
    printf("\n");
    
    destroyTree(tree);
    return 0;
}