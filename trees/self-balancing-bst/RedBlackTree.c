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

// Left rotation
void leftRotate(RedBlackTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    
    if (y->left != tree->NIL)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == tree->NIL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

// Right rotation
void rightRotate(RedBlackTree *tree, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    
    if (x->right != tree->NIL)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (y->parent == tree->NIL)
        tree->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    
    x->right = y;
    y->parent = x;
}

// Fix violations after insertion
void insertFixup(RedBlackTree *tree, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;

            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    
                    leftRotate(tree, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;

                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Insert a node
void insert(RedBlackTree *tree, int data) {
    Node *z = createNode(tree, data);
    Node *y = tree->NIL;
    Node *x = tree->root;
    
    while (x != tree->NIL) {
        y = x;

        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }
    
    z->parent = y;
    
    if (y == tree->NIL)
        tree->root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;
    
    insertFixup(tree, z);
}

// Transplant utility for deletion
void transplant(RedBlackTree *tree, Node *u, Node *v) {
    if (u->parent == tree->NIL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

// Find minimum in subtree
Node* minimum(RedBlackTree *tree, Node *node) {
    while (node->left != tree->NIL)
        node = node->left;

    return node;
}

// Find maximum in subtree
Node* maximum(RedBlackTree *tree, Node *node) {
    while (node->right != tree->NIL)
        node = node->right;

    return node;
}

// Fix violations after deletion
void deleteFixup(RedBlackTree *tree, Node *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;

                leftRotate(tree, x->parent);

                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;

                    rightRotate(tree, w);

                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;

                leftRotate(tree, x->parent);

                x = tree->root;
            }
        } else {
            Node *w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;

                rightRotate(tree, x->parent);

                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;

                    leftRotate(tree, w);

                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;

                rightRotate(tree, x->parent);
                
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Delete a node
void deleteNode(RedBlackTree *tree, Node *z) {
    Node *y = z;
    Node *x;
    Color yOriginalColor = y->color;
    
    if (z->left == tree->NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = minimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        transplant(tree, z, y);

        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if (yOriginalColor == BLACK)
        deleteFixup(tree, x);
    
    free(z);
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
Node* successor(RedBlackTree *tree, Node *x) {
    if (x->right != tree->NIL)
        return minimum(tree, x->right);
    
    Node *y = x->parent;
    while (y != tree->NIL && x == y->right) {
        x = y;
        y = y->parent;
    }

    return (y != tree->NIL) ? y : NULL;
}

// Find predecessor
Node* predecessor(RedBlackTree *tree, Node *x) {
    if (x->left != tree->NIL)
        return maximum(tree, x->left);
    
    Node *y = x->parent;
    while (y != tree->NIL && x == y->left) {
        x = y;
        y = y->parent;
    }

    return (y != tree->NIL) ? y : NULL;
}

// Find tree minimum
Node* findMin(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;

    return minimum(tree, tree->root);
}

// Find tree maximum
Node* findMax(RedBlackTree *tree) {
    if (tree->root == tree->NIL)
        return NULL;
    
    return maximum(tree, tree->root);
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
    
    // Insert elements
    int values[] = {10, 20, 30, 15, 25, 5, 1};
    for (int i = 0; i < 7; i++) {
        insert(tree, values[i]);
        printf("After inserting %d: ", values[i]);
        inorder(tree, tree->root);
        printf("\n");
    }
    
    // Search
    Node *found = search(tree, 15);
    if (found)
        printf("\nFound: %d\n", found->data);
    
    // Find min and max
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
    
    // Delete
    Node *toDelete = search(tree, 20);
    if (toDelete) {
        deleteNode(tree, toDelete);
        printf("\nAfter deleting 20: ");
        inorder(tree, tree->root);
        printf("\n");
    }
    
    return 0;
}