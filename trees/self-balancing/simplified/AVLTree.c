#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

typedef struct AVLTree {
    Node *root;
} AVLTree;

// Get height of a node
int getHeight(Node *node) {
    if (node == NULL) return 0;
    int lh = getHeight(node->left);
    int rh = getHeight(node->right);
    return (lh > rh ? lh : rh) + 1;
}

// Get balance factor of a node
int getBalance(Node *node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Create a new node
Node* createNode(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
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
    
    rightChild->left = pivot;
    pivot->right = leftSubtree;
    
    return rightChild;
}

// Right rotation around pivot node
Node* rightRotate(Node *pivot) {
    Node *leftChild = pivot->left;
    Node *rightSubtree = leftChild->right;
    
    leftChild->right = pivot;
    pivot->left = rightSubtree;
    
    return leftChild;
}

// Balance the node after insertion or deletion
Node* balance(Node *node) {
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
Node* insertNode(Node *node, int data) {
    if (node == NULL)
        return createNode(data);
    
    if (data < node->data)
        node->left = insertNode(node->left, data);
    else if (data > node->data)
        node->right = insertNode(node->right, data);
    else
        return node; // Duplicate values not allowed
    
    return balance(node);
}

// Insert a node with given data
void insert(AVLTree *tree, int data) {
    tree->root = insertNode(tree->root, data);
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
    
    if (data < node->data) {
        node->left = deleteNodeHelper(node->left, data);
    } else if (data > node->data) {
        node->right = deleteNodeHelper(node->right, data);
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
        
        Node *temp = findMinimum(node->right);
        node->data = temp->data;
        node->right = deleteNodeHelper(node->right, temp->data);
    }
    
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

// Find successor using in-order traversal approach
Node* successorHelper(Node *root, Node *target, Node *succ) {
    if (root == NULL)
        return succ;
    
    if (target->data < root->data) {
        return successorHelper(root->left, target, root);
    } else if (target->data > root->data) {
        return successorHelper(root->right, target, succ);
    } else {
        if (root->right)
            return findMinimum(root->right);
        return succ;
    }
}

Node* successor(AVLTree *tree, Node *node) {
    return successorHelper(tree->root, node, NULL);
}

// Find predecessor using in-order traversal approach
Node* predecessorHelper(Node *root, Node *target, Node *pred) {
    if (root == NULL)
        return pred;
    
    if (target->data < root->data) {
        return predecessorHelper(root->left, target, pred);
    } else if (target->data > root->data) {
        return predecessorHelper(root->right, target, root);
    } else {
        if (root->left)
            return findMaximum(root->left);
        return pred;
    }
}

Node* predecessor(AVLTree *tree, Node *node) {
    return predecessorHelper(tree->root, node, NULL);
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
        printf("%d ", node->data);
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
        Node *succ = successor(tree, node);
        Node *pred = predecessor(tree, node);
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
