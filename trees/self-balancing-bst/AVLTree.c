#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int height;
    struct Node *left, *right, *parent;
} Node;

typedef struct AVLTree {
    Node *root;
} AVLTree;

// Get height of a node
int getHeight(Node *node) {
    return node ? node->height : 0;
}

// Get balance factor of a node
int getBalance(Node *node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Update height of a node
void updateHeight(Node *node) {
    if (node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }
}

// Create a new node
Node* createNode(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// Initialize tree
AVLTree* createTree() {
    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    tree->root = NULL;
    return tree;
}

// Left rotation around pivot node
Node* leftRotate(Node *pivot) {
    Node *rightChild = pivot->right;
    Node *leftSubtree = rightChild->left;
    
    // Perform rotation
    rightChild->left = pivot;
    pivot->right = leftSubtree;
    
    // Update parent pointers
    rightChild->parent = pivot->parent;
    pivot->parent = rightChild;
    if (leftSubtree)
        leftSubtree->parent = pivot;
    
    // Update heights
    updateHeight(pivot);
    updateHeight(rightChild);
    
    return rightChild;
}

// Right rotation around pivot node
Node* rightRotate(Node *pivot) {
    Node *leftChild = pivot->left;
    Node *rightSubtree = leftChild->right;
    
    // Perform rotation
    leftChild->right = pivot;
    pivot->left = rightSubtree;
    
    // Update parent pointers
    leftChild->parent = pivot->parent;
    pivot->parent = leftChild;
    if (rightSubtree)
        rightSubtree->parent = pivot;
    
    // Update heights
    updateHeight(pivot);
    updateHeight(leftChild);
    
    return leftChild;
}

// Balance the node after insertion or deletion
Node* balance(Node *node) {
    updateHeight(node);
    int balanceFactor = getBalance(node);
    
    // Left-Left Case
    if (balanceFactor > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);
    
    // Left-Right Case
    if (balanceFactor > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right-Right Case
    if (balanceFactor < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);
    
    // Right-Left Case
    if (balanceFactor < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

// Insert helper function
Node* insertNode(Node *node, int data, Node *parent) {
    // Perform standard BST insertion
    if (node == NULL) {
        Node *newNode = createNode(data);
        newNode->parent = parent;
        return newNode;
    }
    
    if (data < node->data)
        node->left = insertNode(node->left, data, node);
    else if (data > node->data)
        node->right = insertNode(node->right, data, node);
    else
        return node; // Duplicate values not allowed
    
    // Balance the node
    return balance(node);
}

// Insert a node with given data
void insert(AVLTree *tree, int data) {
    tree->root = insertNode(tree->root, data, NULL);
}

// Find the node with minimum value in subtree
Node* findMinimum(Node *subtreeRoot) {
    while (subtreeRoot && subtreeRoot->left)
        subtreeRoot = subtreeRoot->left;
    return subtreeRoot;
}

// Find the node with maximum value in subtree
Node* findMaximum(Node *subtreeRoot) {
    while (subtreeRoot && subtreeRoot->right)
        subtreeRoot = subtreeRoot->right;
    return subtreeRoot;
}

// Delete helper function
Node* deleteNodeHelper(Node *node, int data) {
    if (node == NULL)
        return node;
    
    // Perform standard BST deletion
    if (data < node->data) {
        node->left = deleteNodeHelper(node->left, data);
    } else if (data > node->data) {
        node->right = deleteNodeHelper(node->right, data);
    } else {
        // Node with one child or no child
        if (node->left == NULL) {
            Node *temp = node->right;
            if (temp)
                temp->parent = node->parent;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node *temp = node->left;
            if (temp)
                temp->parent = node->parent;
            free(node);
            return temp;
        }
        
        // Node with two children
        Node *temp = findMinimum(node->right);
        node->data = temp->data;
        node->right = deleteNodeHelper(node->right, temp->data);
    }
    
    // Balance the node
    return balance(node);
}

// Delete a node with given data
void deleteNode(AVLTree *tree, int data) {
    tree->root = deleteNodeHelper(tree->root, data);
}

// Search for a node with given data
Node* search(AVLTree *tree, int data) {
    Node *current = tree->root;
    while (current && current->data != data) {
        if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return current;
}

// Find the successor (next larger node) of given node
Node* successor(Node *node) {
    if (node->right)
        return findMinimum(node->right);
    
    Node *parent = node->parent;
    while (parent && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

// Find the predecessor (next smaller node) of given node
Node* predecessor(Node *node) {
    if (node->left)
        return findMaximum(node->left);
    
    Node *parent = node->parent;
    while (parent && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

// Find the minimum value in the entire tree
Node* findMin(AVLTree *tree) {
    if (tree->root == NULL)
        return NULL;
    return findMinimum(tree->root);
}

// Find the maximum value in the entire tree
Node* findMax(AVLTree *tree) {
    if (tree->root == NULL)
        return NULL;
    return findMaximum(tree->root);
}

// Perform in-order traversal and print nodes
void inorder(Node *node) {
    if (node) {
        inorder(node->left);
        printf("%d(h=%d) ", node->data, node->height);
        inorder(node->right);
    }
}

// Example usage demonstrating all operations
int main() {
    AVLTree *tree = createTree();
    
    // Insert elements
    int values[] = {10, 20, 30, 15, 25, 5, 1};
    for (int i = 0; i < 7; i++) {
        insert(tree, values[i]);
        printf("After inserting %d: ", values[i]);
        inorder(tree->root);
        printf("\n");
    }
    
    // Search for a node
    Node *found = search(tree, 15);
    if (found)
        printf("\nFound: %d\n", found->data);
    
    // Find minimum and maximum
    Node *minNode = findMin(tree);
    Node *maxNode = findMax(tree);
    printf("Min: %d, Max: %d\n", minNode->data, maxNode->data);
    
    // Find successor and predecessor
    Node *node = search(tree, 15);
    if (node) {
        Node *succ = successor(node);
        Node *pred = predecessor(node);
        printf("Successor of 15: %d\n", succ ? succ->data : -1);
        printf("Predecessor of 15: %d\n", pred ? pred->data : -1);
    }
    
    // Delete a node
    deleteNode(tree, 20);
    printf("\nAfter deleting 20: ");
    inorder(tree->root);
    printf("\n");
    
    return 0;
}
