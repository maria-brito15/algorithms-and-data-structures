#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEYS 3
#define MAX_CHILDREN 4

typedef struct Node {
    int keys[MAX_KEYS];
    struct Node *children[MAX_CHILDREN];
    struct Node *parent;
    int numKeys;
    bool isLeaf;
} Node;

typedef struct Tree234 {
    Node *root;
} Tree234;

// Create a new node
Node* createNode(bool isLeaf) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->numKeys = 0;
    node->isLeaf = isLeaf;
    node->parent = NULL;
    
    for (int i = 0; i < MAX_CHILDREN; i++)
        node->children[i] = NULL;
    
    return node;
}

// Initialize tree
Tree234* createTree() {
    Tree234 *tree = (Tree234*)malloc(sizeof(Tree234));
    tree->root = createNode(true);
    return tree;
}

// Check if node is full (has 3 keys)
bool isFull(Node *node) {
    return node->numKeys == MAX_KEYS;
}

// Search for a key in the tree
Node* search(Tree234 *tree, int key) {
    Node *current = tree->root;
    
    while (current != NULL) {
        int i = 0;
        while (i < current->numKeys && key > current->keys[i])
            i++;
        
        if (i < current->numKeys && key == current->keys[i])
            return current;
        
        if (current->isLeaf)
            return NULL;
        
        current = current->children[i];
    }
    
    return NULL;
}

// Split a full child node
void splitChild(Node *parent, int index, Node *child) {
    Node *newNode = createNode(child->isLeaf);
    newNode->numKeys = 1;
    
    // Middle key goes to parent
    int middleKey = child->keys[1];
    
    // Right key goes to new node
    newNode->keys[0] = child->keys[2];
    
    // If not leaf, move children
    if (!child->isLeaf) {
        newNode->children[0] = child->children[2];
        newNode->children[1] = child->children[3];
        
        if (newNode->children[0])
            newNode->children[0]->parent = newNode;
        if (newNode->children[1])
            newNode->children[1]->parent = newNode;
        
        child->children[2] = NULL;
        child->children[3] = NULL;
    }
    
    // Left key stays in original node
    child->numKeys = 1;
    
    // Insert middle key into parent
    for (int i = parent->numKeys; i > index; i--) {
        parent->keys[i] = parent->keys[i - 1];
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->keys[index] = middleKey;
    parent->children[index + 1] = newNode;
    parent->numKeys++;
    
    newNode->parent = parent;
}

// Insert a key into a non-full node
void insertNonFull(Node *node, int key) {
    int i = node->numKeys - 1;
    
    if (node->isLeaf) {
        // Insert key in sorted order
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        // Find child to insert into
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        
        // Split child if full
        if (isFull(node->children[i])) {
            splitChild(node, i, node->children[i]);
            
            if (key > node->keys[i])
                i++;
        }
        
        insertNonFull(node->children[i], key);
    }
}

// Insert a key into the tree
void insert(Tree234 *tree, int key) {
    Node *root = tree->root;
    
    if (isFull(root)) {
        Node *newRoot = createNode(false);
        newRoot->children[0] = root;
        root->parent = newRoot;
        
        splitChild(newRoot, 0, root);
        tree->root = newRoot;
        
        insertNonFull(newRoot, key);
    } else {
        insertNonFull(root, key);
    }
}

// Find the minimum key in a subtree
int findMinimum(Node *node) {
    while (!node->isLeaf)
        node = node->children[0];
    
    return node->keys[0];
}

// Find the maximum key in a subtree
int findMaximum(Node *node) {
    while (!node->isLeaf)
        node = node->children[node->numKeys];
    
    return node->keys[node->numKeys - 1];
}

// Merge a key and two children
void merge(Node *parent, int index) {
    Node *left = parent->children[index];
    Node *right = parent->children[index + 1];
    
    // Move key from parent to left child
    left->keys[left->numKeys] = parent->keys[index];
    left->numKeys++;
    
    // Copy keys from right to left
    for (int i = 0; i < right->numKeys; i++) {
        left->keys[left->numKeys] = right->keys[i];
        left->numKeys++;
    }
    
    // Copy children from right to left
    if (!left->isLeaf) {
        for (int i = 0; i <= right->numKeys; i++) {
            left->children[left->numKeys - right->numKeys + i] = right->children[i];
            if (right->children[i])
                right->children[i]->parent = left;
        }
    }
    
    // Remove key from parent
    for (int i = index; i < parent->numKeys - 1; i++) {
        parent->keys[i] = parent->keys[i + 1];
        parent->children[i + 1] = parent->children[i + 2];
    }
    parent->numKeys--;
    
    free(right);
}

// Borrow a key from left sibling
void borrowFromLeft(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *sibling = parent->children[index - 1];
    
    // Move all keys in child one step ahead
    for (int i = child->numKeys; i > 0; i--)
        child->keys[i] = child->keys[i - 1];
    
    // Move children
    if (!child->isLeaf) {
        for (int i = child->numKeys + 1; i > 0; i--)
            child->children[i] = child->children[i - 1];
    }
    
    // Move key from parent to child
    child->keys[0] = parent->keys[index - 1];
    
    // Move key from sibling to parent
    parent->keys[index - 1] = sibling->keys[sibling->numKeys - 1];
    
    // Move child pointer
    if (!child->isLeaf) {
        child->children[0] = sibling->children[sibling->numKeys];
        if (child->children[0])
            child->children[0]->parent = child;
        sibling->children[sibling->numKeys] = NULL;
    }
    
    child->numKeys++;
    sibling->numKeys--;
}

// Borrow a key from right sibling
void borrowFromRight(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *sibling = parent->children[index + 1];
    
    // Move key from parent to child
    child->keys[child->numKeys] = parent->keys[index];
    
    // Move key from sibling to parent
    parent->keys[index] = sibling->keys[0];
    
    // Move child pointer
    if (!child->isLeaf) {
        child->children[child->numKeys + 1] = sibling->children[0];
        if (child->children[child->numKeys + 1])
            child->children[child->numKeys + 1]->parent = child;
    }
    
    // Shift keys in sibling
    for (int i = 0; i < sibling->numKeys - 1; i++)
        sibling->keys[i] = sibling->keys[i + 1];
    
    // Shift children in sibling
    if (!sibling->isLeaf) {
        for (int i = 0; i < sibling->numKeys; i++)
            sibling->children[i] = sibling->children[i + 1];
    }
    
    child->numKeys++;
    sibling->numKeys--;
}

// Fill child if it has minimum keys
void fill(Node *parent, int index) {
    // Borrow from left sibling
    if (index != 0 && parent->children[index - 1]->numKeys > 1)
        borrowFromLeft(parent, index);
    
    // Borrow from right sibling
    else if (index != parent->numKeys && parent->children[index + 1]->numKeys > 1)
        borrowFromRight(parent, index);
    
    // Merge with left sibling
    else if (index != 0)
        merge(parent, index - 1);
    
    // Merge with right sibling
    else
        merge(parent, index);
}

// Delete key from a leaf node
void deleteFromLeaf(Node *node, int index) {
    for (int i = index; i < node->numKeys - 1; i++)
        node->keys[i] = node->keys[i + 1];
    
    node->numKeys--;
}

// Delete key from an internal node
void deleteFromInternal(Tree234 *tree, Node *node, int index) {
    int key = node->keys[index];
    
    if (node->children[index]->numKeys > 1) {
        int pred = findMaximum(node->children[index]);
        node->keys[index] = pred;
        deleteHelper(tree, node->children[index], pred);
    } else if (node->children[index + 1]->numKeys > 1) {
        int succ = findMinimum(node->children[index + 1]);
        node->keys[index] = succ;
        deleteHelper(tree, node->children[index + 1], succ);
    } else {
        merge(node, index);
        deleteHelper(tree, node->children[index], key);
    }
}

// Delete helper function
void deleteHelper(Tree234 *tree, Node *node, int key) {
    int i = 0;
    while (i < node->numKeys && key > node->keys[i])
        i++;
    
    if (i < node->numKeys && key == node->keys[i]) {
        if (node->isLeaf)
            deleteFromLeaf(node, i);
        else
            deleteFromInternal(tree, node, i);
    } else if (!node->isLeaf) {
        bool isInLastChild = (i == node->numKeys);
        
        if (node->children[i]->numKeys < 2)
            fill(node, i);
        
        if (isInLastChild && i > node->numKeys)
            deleteHelper(tree, node->children[i - 1], key);
        else
            deleteHelper(tree, node->children[i], key);
    }
}

// Delete a key from the tree
void deleteKey(Tree234 *tree, int key) {
    if (tree->root == NULL)
        return;
    
    deleteHelper(tree, tree->root, key);
    
    // If root is empty, make its only child the new root
    if (tree->root->numKeys == 0) {
        Node *oldRoot = tree->root;
        if (tree->root->isLeaf)
            tree->root = NULL;
        else {
            tree->root = tree->root->children[0];
            if (tree->root)
                tree->root->parent = NULL;
        }
        free(oldRoot);
    }
}

// Find minimum key in tree
int findMin(Tree234 *tree) {
    if (tree->root == NULL)
        return -1;
    
    return findMinimum(tree->root);
}

// Find maximum key in tree
int findMax(Tree234 *tree) {
    if (tree->root == NULL)
        return -1;
    
    return findMaximum(tree->root);
}

// In-order traversal
void inorder(Node *node) {
    if (node != NULL) {
        for (int i = 0; i < node->numKeys; i++) {
            if (!node->isLeaf)
                inorder(node->children[i]);
            printf("%d ", node->keys[i]);
        }
        if (!node->isLeaf)
            inorder(node->children[node->numKeys]);
    }
}

// Print tree structure (level by level)
void printLevel(Node *node, int level, int currentLevel) {
    if (node == NULL)
        return;
    
    if (currentLevel == level) {
        printf("[");
        for (int i = 0; i < node->numKeys; i++) {
            printf("%d", node->keys[i]);
            if (i < node->numKeys - 1)
                printf(",");
        }
        printf("] ");
    } else if (currentLevel < level) {
        for (int i = 0; i <= node->numKeys; i++)
            printLevel(node->children[i], level, currentLevel + 1);
    }
}

void printTree(Tree234 *tree) {
    if (tree->root == NULL) {
        printf("Empty tree\n");
        return;
    }
    
    int height = 0;
    Node *temp = tree->root;
    while (!temp->isLeaf) {
        temp = temp->children[0];
        height++;
    }
    
    for (int i = 0; i <= height; i++) {
        printf("Level %d: ", i);
        printLevel(tree->root, i, 0);
        printf("\n");
    }
}

// Example usage
int main() {
    Tree234 *tree = createTree();
    
    // Insert elements
    int values[] = {10, 20, 30, 15, 25, 5, 1, 35, 40, 50};
    for (int i = 0; i < 10; i++) {
        insert(tree, values[i]);
        printf("\nAfter inserting %d:\n", values[i]);
        printTree(tree);
        printf("In-order: ");
        inorder(tree->root);
        printf("\n");
    }
    
    // Search for a key
    Node *found = search(tree, 15);
    printf("\n%s 15\n", found ? "Found:" : "Not found:");
    
    // Find minimum and maximum
    printf("Min: %d, Max: %d\n", findMin(tree), findMax(tree));
    
    // Delete a key
    deleteKey(tree, 20);
    printf("\nAfter deleting 20:\n");
    printTree(tree);
    printf("In-order: ");
    inorder(tree->root);
    printf("\n");
    
    return 0;
}
