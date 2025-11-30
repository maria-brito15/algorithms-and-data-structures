#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct RedBlackTree {
    Node *root;
    Node *NIL;
} RedBlackTree;

// Create a new node
Node* createNode(RedBlackTree *tree, int data) {
    Node *node = (Node*)malloc(sizeof(Node));

    node->data = data;
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    node->parent = tree->NIL;

    return node;
}

// Initialize tree
RedBlackTree* createTree() {
    RedBlackTree *tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));

    tree->NIL = (Node*)malloc(sizeof(Node));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->root = tree->NIL;

    return tree;
}

// Left rotation around pivot node
void leftRotate(RedBlackTree *tree, Node *pivot) {
    Node *rightChild = pivot->right;
    pivot->right = rightChild->left;
    
    if (rightChild->left != tree->NIL)
        rightChild->left->parent = pivot;
    
    rightChild->parent = pivot->parent;
    
    if (pivot->parent == tree->NIL)
        tree->root = rightChild;
    else if (pivot == pivot->parent->left)
        pivot->parent->left = rightChild;
    else
        pivot->parent->right = rightChild;
    
    rightChild->left = pivot;
    pivot->parent = rightChild;
}

// Right rotation around pivot node
void rightRotate(RedBlackTree *tree, Node *pivot) {
    Node *leftChild = pivot->left;
    pivot->left = leftChild->right;
    
    if (leftChild->right != tree->NIL)
        leftChild->right->parent = pivot;
    
    leftChild->parent = pivot->parent;
    
    if (pivot->parent == tree->NIL)
        tree->root = leftChild;
    else if (pivot == pivot->parent->left)
        pivot->parent->left = leftChild;
    else
        pivot->parent->right = leftChild;
    
    leftChild->right = pivot;
    pivot->parent = leftChild;
}

// Fix red-black tree violations after insertion
void fixInsertViolations(RedBlackTree *tree, Node *current) {
    while (current->parent->color == RED) {
        if (current->parent == current->parent->parent->left) {
            Node *uncle = current->parent->parent->right;

            // Case 1: Uncle is red - recolor
            if (uncle->color == RED) {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            } else {
                // Case 2: Current is right child - left rotate
                if (current == current->parent->right) {
                    current = current->parent;
                    leftRotate(tree, current);
                }

                // Case 3: Current is left child - recolor and right rotate
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                rightRotate(tree, current->parent->parent);
            }
        } else {
            Node *uncle = current->parent->parent->left;
            
            // Case 1: Uncle is red - recolor
            if (uncle->color == RED) {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            } else {
                // Case 2: Current is left child - right rotate
                if (current == current->parent->left) {
                    current = current->parent;
                    rightRotate(tree, current);
                }
                
                // Case 3: Current is right child - recolor and left rotate
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                leftRotate(tree, current->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Insert a node with given data
void insert(RedBlackTree *tree, int data) {
    Node *newNode = createNode(tree, data);
    Node *parentNode = tree->NIL;
    Node *currentNode = tree->root;
    
    // Find the correct position for insertion
    while (currentNode != tree->NIL) {
        parentNode = currentNode;

        if (newNode->data < currentNode->data)
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;
    }
    
    newNode->parent = parentNode;
    
    // Insert the node
    if (parentNode == tree->NIL)
        tree->root = newNode;
    else if (newNode->data < parentNode->data)
        parentNode->left = newNode;
    else
        parentNode->right = newNode;
    
    fixInsertViolations(tree, newNode);
}

// Replace old node with new node in the tree structure
void transplant(RedBlackTree *tree, Node *oldNode, Node *newNode) {
    if (oldNode->parent == tree->NIL)
        tree->root = newNode;
    else if (oldNode == oldNode->parent->left)
        oldNode->parent->left = newNode;
    else
        oldNode->parent->right = newNode;

    newNode->parent = oldNode->parent;
}

// Find the node with minimum value in subtree
Node* findMinimum(RedBlackTree *tree, Node *subtreeRoot) {
    while (subtreeRoot->left != tree->NIL)
        subtreeRoot = subtreeRoot->left;

    return subtreeRoot;
}

// Find the node with maximum value in subtree
Node* findMaximum(RedBlackTree *tree, Node *subtreeRoot) {
    while (subtreeRoot->right != tree->NIL)
        subtreeRoot = subtreeRoot->right;

    return subtreeRoot;
}

// Fix red-black tree violations after deletion
void fixDeleteViolations(RedBlackTree *tree, Node *current) {
    while (current != tree->root && current->color == BLACK) {
        if (current == current->parent->left) {
            Node *sibling = current->parent->right;

            // Case 1: Sibling is red
            if (sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                leftRotate(tree, current->parent);
                sibling = current->parent->right;
            }
            
            // Case 2: Sibling is black with two black children
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                current = current->parent;
            } else {
                // Case 3: Sibling is black with red left child and black right child
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(tree, sibling);
                    sibling = current->parent->right;
                }

                // Case 4: Sibling is black with red right child
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(tree, current->parent);
                current = tree->root;
            }
        } else {
            Node *sibling = current->parent->left;

            // Case 1: Sibling is red
            if (sibling->color == RED) {
                sibling->color = BLACK;
                current->parent->color = RED;
                rightRotate(tree, current->parent);
                sibling = current->parent->left;
            }
            
            // Case 2: Sibling is black with two black children
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                current = current->parent;
            } else {
                // Case 3: Sibling is black with red right child and black left child
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(tree, sibling);
                    sibling = current->parent->left;
                }

                // Case 4: Sibling is black with red left child
                sibling->color = current->parent->color;
                current->parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(tree, current->parent);
                current = tree->root;
            }
        }
    }
    current->color = BLACK;
}

// Delete a node
void deleteNode(RedBlackTree *tree, Node *nodeToDelete) {
    Node *nodeToRemove = nodeToDelete;
    Node *replacementNode;
    Color originalColor = nodeToRemove->color;
    
    // Case 1: Node has no left child
    if (nodeToDelete->left == tree->NIL) {
        replacementNode = nodeToDelete->right;
        transplant(tree, nodeToDelete, nodeToDelete->right);
    } 
    // Case 2: Node has no right child
    else if (nodeToDelete->right == tree->NIL) {
        replacementNode = nodeToDelete->left;
        transplant(tree, nodeToDelete, nodeToDelete->left);
    } 
    // Case 3: Node has two children
    else {
        nodeToRemove = findMinimum(tree, nodeToDelete->right);
        originalColor = nodeToRemove->color;
        replacementNode = nodeToRemove->right;
        
        if (nodeToRemove->parent == nodeToDelete) {
            replacementNode->parent = nodeToRemove;
        } else {
            transplant(tree, nodeToRemove, nodeToRemove->right);
            nodeToRemove->right = nodeToDelete->right;
            nodeToRemove->right->parent = nodeToRemove;
        }
        
        transplant(tree, nodeToDelete, nodeToRemove);
        nodeToRemove->left = nodeToDelete->left;
        nodeToRemove->left->parent = nodeToRemove;
        nodeToRemove->color = nodeToDelete->color;
    }
    
    // Fix violations if a black node was removed
    if (originalColor == BLACK)
        fixDeleteViolations(tree, replacementNode);
    
    free(nodeToDelete);
}

// Search for a node with given data
Node* search(RedBlackTree *tree, int data) {
    Node *current = tree->root;
    while (current != tree->NIL && current->data != data) {
        if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    return (current != tree->NIL) ? current : NULL;
}

// Find the successor (next larger node) of given node
Node* successor(RedBlackTree *tree, Node *node) {
    if (node->right != tree->NIL)
        return findMinimum(tree, node->right);
    
    Node *parent = node->parent;
    while (parent != tree->NIL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }

    return (parent != tree->NIL) ? parent : NULL;
}

// Find the predecessor (next smaller node) of given node
Node* predecessor(RedBlackTree *tree, Node *node) {
    if (node->left != tree->NIL)
        return findMaximum(tree, node->left);
    
    Node *parent = node->parent;
    while (parent != tree->NIL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }

    return (parent != tree->NIL) ? parent : NULL;
}

// Find the minimum value in the entire tree
Node* findMin(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;

    return findMinimum(tree, tree->root);
}

// Find the maximum value in the entire tree
Node* findMax(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;
    
    return findMaximum(tree, tree->root);
}

// Perform in-order traversal and print nodes
void inorder(RedBlackTree *tree, Node *node) {
    if (node != tree->NIL) {
        inorder(tree, node->left);
        printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        inorder(tree, node->right);
    }
}

// Example usage demonstrating all operations
int main() {
    RedBlackTree *tree = createTree();
    
    // Insert elements
    int values[] = {10, 20, 30, 15, 25, 5, 1};
    for (int i = 0; i < 7; i++) {
        insert(tree, values[i]);
        printf("After inserting %d: ", values[i]);
        inorder(tree, tree->root);
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
    Node *toDelete = search(tree, 20);
    if (toDelete) {
        deleteNode(tree, toDelete);
        printf("\nAfter deleting 20: ");
        inorder(tree, tree->root);
        printf("\n");
    }
    
    return 0;
}
