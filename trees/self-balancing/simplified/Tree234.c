#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEYS 3
#define MAX_CHILDREN 4

typedef struct Node {
    int keys[MAX_KEYS];
    struct Node *children[MAX_CHILDREN];
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

// Check if node is full
bool isFull(Node *node) {
    return node->numKeys == MAX_KEYS;
}

// Search for a key
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

// Split result structure
typedef struct SplitResult {
    Node *left;
    int middleKey;
    Node *right;
} SplitResult;

// Split a full node
SplitResult splitNode(Node *node) {
    SplitResult result;
    Node *newNode = createNode(node->isLeaf);
    
    result.middleKey = node->keys[1];
    result.left = node;
    result.right = newNode;
    
    // Right key goes to new node
    newNode->keys[0] = node->keys[2];
    newNode->numKeys = 1;
    
    // Left key stays in original node
    node->numKeys = 1;
    
    // Move children if not leaf
    if (!node->isLeaf) {
        newNode->children[0] = node->children[2];
        newNode->children[1] = node->children[3];
        node->children[2] = NULL;
        node->children[3] = NULL;
    }
    
    return result;
}

// Insert into a non-full node
void insertNonFull(Node *node, int key) {
    int i = node->numKeys - 1;
    
    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        
        if (isFull(node->children[i])) {
            SplitResult split = splitNode(node->children[i]);
            
            // Insert middle key into parent
            for (int j = node->numKeys; j > i; j--) {
                node->keys[j] = node->keys[j - 1];
                node->children[j + 1] = node->children[j];
            }
            node->keys[i] = split.middleKey;
            node->children[i] = split.left;
            node->children[i + 1] = split.right;
            node->numKeys++;
            
            if (key > node->keys[i])
                i++;
        }
        
        insertNonFull(node->children[i], key);
    }
}

// Insert a key
void insert(Tree234 *tree, int key) {
    if (isFull(tree->root)) {
        Node *newRoot = createNode(false);
        SplitResult split = splitNode(tree->root);
        
        newRoot->keys[0] = split.middleKey;
        newRoot->children[0] = split.left;
        newRoot->children[1] = split.right;
        newRoot->numKeys = 1;
        
        tree->root = newRoot;
        insertNonFull(newRoot, key);
    } else {
        insertNonFull(tree->root, key);
    }
}

// Find minimum key in subtree
int findMinimum(Node *node) {
    while (!node->isLeaf)
        node = node->children[0];
    return node->keys[0];
}

// Find maximum key in subtree
int findMaximum(Node *node) {
    while (!node->isLeaf)
        node = node->children[node->numKeys];
    return node->keys[node->numKeys - 1];
}

// Get child index
int getChildIndex(Node *parent, Node *child) {
    for (int i = 0; i <= parent->numKeys; i++)
        if (parent->children[i] == child)
            return i;
    return -1;
}

// Merge nodes
void merge(Node *parent, int idx) {
    Node *left = parent->children[idx];
    Node *right = parent->children[idx + 1];
    
    left->keys[left->numKeys] = parent->keys[idx];
    left->numKeys++;
    
    for (int i = 0; i < right->numKeys; i++) {
        left->keys[left->numKeys] = right->keys[i];
        left->numKeys++;
    }
    
    if (!left->isLeaf) {
        for (int i = 0; i <= right->numKeys; i++)
            left->children[left->numKeys - right->numKeys + i] = right->children[i];
    }
    
    for (int i = idx; i < parent->numKeys - 1; i++) {
        parent->keys[i] = parent->keys[i + 1];
        parent->children[i + 1] = parent->children[i + 2];
    }
    parent->numKeys--;
    
    free(right);
}

// Borrow from left sibling
void borrowFromLeft(Node *parent, int idx) {
    Node *child = parent->children[idx];
    Node *sibling = parent->children[idx - 1];
    
    for (int i = child->numKeys; i > 0; i--)
        child->keys[i] = child->keys[i - 1];
    
    if (!child->isLeaf) {
        for (int i = child->numKeys + 1; i > 0; i--)
            child->children[i] = child->children[i - 1];
    }
    
    child->keys[0] = parent->keys[idx - 1];
    parent->keys[idx - 1] = sibling->keys[sibling->numKeys - 1];
    
    if (!child->isLeaf) {
        child->children[0] = sibling->children[sibling->numKeys];
        sibling->children[sibling->numKeys] = NULL;
    }
    
    child->numKeys++;
    sibling->numKeys--;
}

// Borrow from right sibling
void borrowFromRight(Node *parent, int idx) {
    Node *child = parent->children[idx];
    Node *sibling = parent->children[idx + 1];
    
    child->keys[child->numKeys] = parent->keys[idx];
    parent->keys[idx] = sibling->keys[0];
    
    if (!child->isLeaf)
        child->children[child->numKeys + 1] = sibling->children[0];
    
    for (int i = 0; i < sibling->numKeys - 1; i++)
        sibling->keys[i] = sibling->keys[i + 1];
    
    if (!sibling->isLeaf) {
        for (int i = 0; i < sibling->numKeys; i++)
            sibling->children[i] = sibling->children[i + 1];
    }
    
    child->numKeys++;
    sibling->numKeys--;
}

// Fill child
void fill(Node *parent, int idx) {
    if (idx != 0 && parent->children[idx - 1]->numKeys > 1)
        borrowFromLeft(parent, idx);
    else if (idx != parent->numKeys && parent->children[idx + 1]->numKeys > 1)
        borrowFromRight(parent, idx);
    else if (idx != 0)
        merge(parent, idx - 1);
    else
        merge(parent, idx);
}

// Delete from leaf
void deleteFromLeaf(Node *node, int idx) {
    for (int i = idx; i < node->numKeys - 1; i++)
        node->keys[i] = node->keys[i + 1];
    node->numKeys--;
}

// Delete from internal node
void deleteFromInternal(Node *node, int idx) {
    int key = node->keys[idx];
    
    if (node->children[idx]->numKeys > 1) {
        int pred = findMaximum(node->children[idx]);
        node->keys[idx] = pred;
        deleteHelper(node->children[idx], pred);
    } else if (node->children[idx + 1]->numKeys > 1) {
        int succ = findMinimum(node->children[idx + 1]);
        node->keys[idx] = succ;
        deleteHelper(node->children[idx + 1], succ);
    } else {
        merge(node, idx);
        deleteHelper(node->children[idx], key);
    }
}

// Delete helper
void deleteHelper(Node *node, int key) {
    int i = 0;
    while (i < node->numKeys && key > node->keys[i])
        i++;
    
    if (i < node->numKeys && key == node->keys[i]) {
        if (node->isLeaf)
            deleteFromLeaf(node, i);
        else
            deleteFromInternal(node, i);
    } else if (!node->isLeaf) {
        bool isInLastChild = (i == node->numKeys);
        
        if (node->children[i]->numKeys < 2)
            fill(node, i);
        
        if (isInLastChild && i > node->numKeys)
            deleteHelper(node->children[i - 1], key);
        else
            deleteHelper(node->children[i], key);
    }
}

// Delete a key
void deleteKey(Tree234 *tree, int key) {
    if (tree->root == NULL)
        return;
    
    deleteHelper(tree->root, key);
    
    if (tree->root->numKeys == 0) {
        Node *oldRoot = tree->root;
        if (tree->root->isLeaf)
            tree->root = NULL;
        else
            tree->root = tree->root->children[0];
        free(oldRoot);
    }
}

// Find minimum
int findMin(Tree234 *tree) {
    if (tree->root == NULL)
        return -1;
    return findMinimum(tree->root);
}

// Find maximum
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

// Print tree structure
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
    
    int values[] = {10, 20, 30, 15, 25, 5, 1, 35, 40, 50};
    for (int i = 0; i < 10; i++) {
        insert(tree, values[i]);
        printf("\nAfter inserting %d:\n", values[i]);
        printTree(tree);
        printf("In-order: ");
        inorder(tree->root);
        printf("\n");
    }
    
    Node *found = search(tree, 15);
    printf("\n%s 15\n", found ? "Found:" : "Not found:");
    
    printf("Min: %d, Max: %d\n", findMin(tree), findMax(tree));
    
    deleteKey(tree, 20);
    printf("\nAfter deleting 20:\n");
    printTree(tree);
    printf("In-order: ");
    inorder(tree->root);
    printf("\n");
    
    return 0;
}
