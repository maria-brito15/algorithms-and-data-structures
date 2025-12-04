#include <stdio.h>
#include <stdlib.h>

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

// Create a new node
Node* createNode(RedBlackTree *tree, int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    return node;
}

// Initialize tree
RedBlackTree* createTree() {
    RedBlackTree *tree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    tree->NIL = (Node*)malloc(sizeof(Node));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = NULL;
    tree->root = tree->NIL;
    return tree;
}

// Left rotation
Node* leftRotate(Node *pivot, Node *NIL) {
    Node *rightChild = pivot->right;
    pivot->right = rightChild->left;
    rightChild->left = pivot;
    return rightChild;
}

// Right rotation
Node* rightRotate(Node *pivot, Node *NIL) {
    Node *leftChild = pivot->left;
    pivot->left = leftChild->right;
    leftChild->right = pivot;
    return leftChild;
}

// Insert and fix violations
Node* insertFix(RedBlackTree *tree, Node *root, int data, int *inserted) {
    if (root == tree->NIL) {
        *inserted = 1;
        return createNode(tree, data);
    }
    
    if (data < root->data) {
        root->left = insertFix(tree, root->left, data, inserted);
    } else if (data > root->data) {
        root->right = insertFix(tree, root->right, data, inserted);
    } else {
        return root; // Duplicate
    }
    
    if (!*inserted)
        return root;
    
    // Fix violations
    if (root->left != tree->NIL && root->left->color == RED &&
        root->left->left != tree->NIL && root->left->left->color == RED) {
        
        if (root->right != tree->NIL && root->right->color == RED) {
            root->color = RED;
            root->left->color = BLACK;
            root->right->color = BLACK;
        } else {
            root = rightRotate(root, tree->NIL);
            root->color = BLACK;
            root->right->color = RED;
        }
    } else if (root->left != tree->NIL && root->left->color == RED &&
               root->left->right != tree->NIL && root->left->right->color == RED) {
        
        if (root->right != tree->NIL && root->right->color == RED) {
            root->color = RED;
            root->left->color = BLACK;
            root->right->color = BLACK;
        } else {
            root->left = leftRotate(root->left, tree->NIL);
            root = rightRotate(root, tree->NIL);
            root->color = BLACK;
            root->right->color = RED;
        }
    } else if (root->right != tree->NIL && root->right->color == RED &&
               root->right->right != tree->NIL && root->right->right->color == RED) {
        
        if (root->left != tree->NIL && root->left->color == RED) {
            root->color = RED;
            root->left->color = BLACK;
            root->right->color = BLACK;
        } else {
            root = leftRotate(root, tree->NIL);
            root->color = BLACK;
            root->left->color = RED;
        }
    } else if (root->right != tree->NIL && root->right->color == RED &&
               root->right->left != tree->NIL && root->right->left->color == RED) {
        
        if (root->left != tree->NIL && root->left->color == RED) {
            root->color = RED;
            root->left->color = BLACK;
            root->right->color = BLACK;
        } else {
            root->right = rightRotate(root->right, tree->NIL);
            root = leftRotate(root, tree->NIL);
            root->color = BLACK;
            root->left->color = RED;
        }
    }
    
    return root;
}

// Insert a node
void insert(RedBlackTree *tree, int data) {
    int inserted = 0;
    tree->root = insertFix(tree, tree->root, data, &inserted);
    tree->root->color = BLACK;
}

// Find minimum node
Node* findMinimum(RedBlackTree *tree, Node *subtreeRoot) {
    while (subtreeRoot->left != tree->NIL)
        subtreeRoot = subtreeRoot->left;
    return subtreeRoot;
}

// Find maximum node
Node* findMaximum(RedBlackTree *tree, Node *subtreeRoot) {
    while (subtreeRoot->right != tree->NIL)
        subtreeRoot = subtreeRoot->right;
    return subtreeRoot;
}

// Helper for deletion fixup
Node* deleteFixLeft(RedBlackTree *tree, Node *root) {
    Node *sibling = root->right;
    
    if (sibling->color == RED) {
        root = leftRotate(root, tree->NIL);
        root->color = BLACK;
        root->left->color = RED;
        sibling = root->left->right;
    }
    
    if ((sibling->left == tree->NIL || sibling->left->color == BLACK) &&
        (sibling->right == tree->NIL || sibling->right->color == BLACK)) {
        sibling->color = RED;
        return root;
    }
    
    if (sibling->right == tree->NIL || sibling->right->color == BLACK) {
        sibling = rightRotate(sibling, tree->NIL);
        sibling->color = BLACK;
        sibling->right->color = RED;
        root->right = sibling;
    }
    
    root = leftRotate(root, tree->NIL);
    root->color = root->left->color;
    root->left->color = BLACK;
    root->right->color = BLACK;
    
    return root;
}

Node* deleteFixRight(RedBlackTree *tree, Node *root) {
    Node *sibling = root->left;
    
    if (sibling->color == RED) {
        root = rightRotate(root, tree->NIL);
        root->color = BLACK;
        root->right->color = RED;
        sibling = root->right->left;
    }
    
    if ((sibling->left == tree->NIL || sibling->left->color == BLACK) &&
        (sibling->right == tree->NIL || sibling->right->color == BLACK)) {
        sibling->color = RED;
        return root;
    }
    
    if (sibling->left == tree->NIL || sibling->left->color == BLACK) {
        sibling = leftRotate(sibling, tree->NIL);
        sibling->color = BLACK;
        sibling->left->color = RED;
        root->left = sibling;
    }
    
    root = rightRotate(root, tree->NIL);
    root->color = root->right->color;
    root->right->color = BLACK;
    root->left->color = BLACK;
    
    return root;
}

// Delete helper
Node* deleteHelper(RedBlackTree *tree, Node *root, int data, int *done) {
    if (root == tree->NIL)
        return root;
    
    if (data < root->data) {
        root->left = deleteHelper(tree, root->left, data, done);
        if (!*done)
            root = deleteFixLeft(tree, root);
    } else if (data > root->data) {
        root->right = deleteHelper(tree, root->right, data, done);
        if (!*done)
            root = deleteFixRight(tree, root);
    } else {
        if (root->left == tree->NIL || root->right == tree->NIL) {
            Node *temp = (root->left != tree->NIL) ? root->left : root->right;
            *done = (root->color == RED);
            free(root);
            return temp;
        }
        
        Node *successor = findMinimum(tree, root->right);
        root->data = successor->data;
        root->right = deleteHelper(tree, root->right, successor->data, done);
        if (!*done)
            root = deleteFixRight(tree, root);
    }
    
    return root;
}

// Delete a node
void deleteNode(RedBlackTree *tree, int data) {
    int done = 0;
    tree->root = deleteHelper(tree, tree->root, data, &done);
    if (tree->root != tree->NIL)
        tree->root->color = BLACK;
}

// Search for a node
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

// Find successor
Node* successorHelper(RedBlackTree *tree, Node *root, int data, Node *succ) {
    if (root == tree->NIL)
        return succ;
    
    if (data < root->data)
        return successorHelper(tree, root->left, data, root);
    else if (data > root->data)
        return successorHelper(tree, root->right, data, succ);
    else {
        if (root->right != tree->NIL)
            return findMinimum(tree, root->right);
        return succ;
    }
}

Node* successor(RedBlackTree *tree, Node *node) {
    Node *result = successorHelper(tree, tree->root, node->data, tree->NIL);
    return (result != tree->NIL) ? result : NULL;
}

// Find predecessor
Node* predecessorHelper(RedBlackTree *tree, Node *root, int data, Node *pred) {
    if (root == tree->NIL)
        return pred;
    
    if (data < root->data)
        return predecessorHelper(tree, root->left, data, pred);
    else if (data > root->data)
        return predecessorHelper(tree, root->right, data, root);
    else {
        if (root->left != tree->NIL)
            return findMaximum(tree, root->left);
        return pred;
    }
}

Node* predecessor(RedBlackTree *tree, Node *node) {
    Node *result = predecessorHelper(tree, tree->root, node->data, tree->NIL);
    return (result != tree->NIL) ? result : NULL;
}

// Find minimum in tree
Node* findMin(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;
    return findMinimum(tree, tree->root);
}

// Find maximum in tree
Node* findMax(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;
    return findMaximum(tree, tree->root);
}

// In-order traversal
void inorder(RedBlackTree *tree, Node *node) {
    if (node != tree->NIL) {
        inorder(tree, node->left);
        printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        inorder(tree, node->right);
    }
}

// Example usage
int main() {
    RedBlackTree *tree = createTree();
    
    int values[] = {10, 20, 30, 15, 25, 5, 1};
    for (int i = 0; i < 7; i++) {
        insert(tree, values[i]);
        printf("After inserting %d: ", values[i]);
        inorder(tree, tree->root);
        printf("\n");
    }
    
    Node *found = search(tree, 15);
    if (found)
        printf("\nFound: %d\n", found->data);
    
    Node *minNode = findMin(tree);
    Node *maxNode = findMax(tree);
    printf("Min: %d, Max: %d\n", minNode->data, maxNode->data);
    
    Node *node = search(tree, 15);
    if (node) {
        Node *succ = successor(tree, node);
        Node *pred = predecessor(tree, node);
        printf("Successor of 15: %d\n", succ ? succ->data : -1);
        printf("Predecessor of 15: %d\n", pred ? pred->data : -1);
    }
    
    deleteNode(tree, 20);
    printf("\nAfter deleting 20: ");
    inorder(tree, tree->root);
    printf("\n");
    
    return 0;
}
