enum Color {
    RED, BLACK
}

class Node {
    int data;
    Color color;
    Node left, right, parent;
    
    public Node(int data) {
        this.data = data;
        this.color = Color.RED;
    }
}

public class RedBlackTree {
    private Node root;
    private Node NIL;
    
    public RedBlackTree() {
        NIL = new Node(0);
        NIL.color = Color.BLACK;
        NIL.left = NIL.right = NIL.parent = null;
        root = NIL;
    }
    
    // Left rotation around pivot node
    private void leftRotate(Node pivot) {
        Node rightChild = pivot.right;
        pivot.right = rightChild.left;
        
        if (rightChild.left != NIL)
            rightChild.left.parent = pivot;
        
        rightChild.parent = pivot.parent;
        
        if (pivot.parent == NIL)
            root = rightChild;
        else if (pivot == pivot.parent.left)
            pivot.parent.left = rightChild;
        else
            pivot.parent.right = rightChild;
        
        rightChild.left = pivot;
        pivot.parent = rightChild;
    }
    
    // Right rotation around pivot node
    private void rightRotate(Node pivot) {
        Node leftChild = pivot.left;
        pivot.left = leftChild.right;
        
        if (leftChild.right != NIL)
            leftChild.right.parent = pivot;
        
        leftChild.parent = pivot.parent;
        
        if (pivot.parent == NIL)
            root = leftChild;
        else if (pivot == pivot.parent.left)
            pivot.parent.left = leftChild;
        else
            pivot.parent.right = leftChild;
        
        leftChild.right = pivot;
        pivot.parent = leftChild;
    }
    
    // Fix red-black tree violations after insertion
    private void fixInsertViolations(Node current) {
        while (current.parent.color == Color.RED) {
            if (current.parent == current.parent.parent.left) {
                Node uncle = current.parent.parent.right;

                // Case 1: Uncle is red - recolor
                if (uncle.color == Color.RED) {
                    current.parent.color = Color.BLACK;
                    uncle.color = Color.BLACK;
                    current.parent.parent.color = Color.RED;
                    current = current.parent.parent;
                } else {
                    // Case 2: Current is right child - left rotate
                    if (current == current.parent.right) {
                        current = current.parent;
                        leftRotate(current);
                    }

                    // Case 3: Current is left child - recolor and right rotate
                    current.parent.color = Color.BLACK;
                    current.parent.parent.color = Color.RED;
                    rightRotate(current.parent.parent);
                }
            } else {
                Node uncle = current.parent.parent.left;
                
                // Case 1: Uncle is red - recolor
                if (uncle.color == Color.RED) {
                    current.parent.color = Color.BLACK;
                    uncle.color = Color.BLACK;
                    current.parent.parent.color = Color.RED;
                    current = current.parent.parent;
                } else {
                    // Case 2: Current is left child - right rotate
                    if (current == current.parent.left) {
                        current = current.parent;
                        rightRotate(current);
                    }
                    
                    // Case 3: Current is right child - recolor and left rotate
                    current.parent.color = Color.BLACK;
                    current.parent.parent.color = Color.RED;
                    leftRotate(current.parent.parent);
                }
            }
        }
        root.color = Color.BLACK;
    }
    
    // Insert a node with given data
    public void insert(int data) {
        Node newNode = new Node(data);
        newNode.left = NIL;
        newNode.right = NIL;
        
        Node parentNode = NIL;
        Node currentNode = root;
        
        // Find the correct position for insertion
        while (currentNode != NIL) {
            parentNode = currentNode;
            if (newNode.data < currentNode.data)
                currentNode = currentNode.left;
            else
                currentNode = currentNode.right;
        }
        
        newNode.parent = parentNode;
        
        // Insert the node
        if (parentNode == NIL)
            root = newNode;
        else if (newNode.data < parentNode.data)
            parentNode.left = newNode;
        else
            parentNode.right = newNode;
        
        fixInsertViolations(newNode);
    }
    
    // Replace old node with new node in the tree structure
    private void transplant(Node oldNode, Node newNode) {
        if (oldNode.parent == NIL)
            root = newNode;
        else if (oldNode == oldNode.parent.left)
            oldNode.parent.left = newNode;
        else
            oldNode.parent.right = newNode;

        newNode.parent = oldNode.parent;
    }
    
    // Find the node with minimum value in subtree
    private Node findMinimum(Node subtreeRoot) {
        while (subtreeRoot.left != NIL)
            subtreeRoot = subtreeRoot.left;

        return subtreeRoot;
    }
    
    // Find the node with maximum value in subtree
    private Node findMaximum(Node subtreeRoot) {
        while (subtreeRoot.right != NIL)
            subtreeRoot = subtreeRoot.right;

        return subtreeRoot;
    }
    
    // Fix red-black tree violations after deletion
    private void fixDeleteViolations(Node current) {
        while (current != root && current.color == Color.BLACK) {
            if (current == current.parent.left) {
                Node sibling = current.parent.right;
                
                // Case 1: Sibling is red
                if (sibling.color == Color.RED) {
                    sibling.color = Color.BLACK;
                    current.parent.color = Color.RED;
                    leftRotate(current.parent);
                    sibling = current.parent.right;
                }
                
                // Case 2: Sibling is black with two black children
                if (sibling.left.color == Color.BLACK && sibling.right.color == Color.BLACK) {
                    sibling.color = Color.RED;
                    current = current.parent;
                } else {
                    // Case 3: Sibling is black with red left child and black right child
                    if (sibling.right.color == Color.BLACK) {
                        sibling.left.color = Color.BLACK;
                        sibling.color = Color.RED;
                        rightRotate(sibling);
                        sibling = current.parent.right;
                    }
                    
                    // Case 4: Sibling is black with red right child
                    sibling.color = current.parent.color;
                    current.parent.color = Color.BLACK;
                    sibling.right.color = Color.BLACK;
                    leftRotate(current.parent);
                    current = root;
                }
            } else {
                Node sibling = current.parent.left;
                
                // Case 1: Sibling is red
                if (sibling.color == Color.RED) {
                    sibling.color = Color.BLACK;
                    current.parent.color = Color.RED;
                    rightRotate(current.parent);
                    sibling = current.parent.left;
                }
                
                // Case 2: Sibling is black with two black children
                if (sibling.right.color == Color.BLACK && sibling.left.color == Color.BLACK) {
                    sibling.color = Color.RED;
                    current = current.parent;
                } else {
                    // Case 3: Sibling is black with red right child and black left child
                    if (sibling.left.color == Color.BLACK) {
                        sibling.right.color = Color.BLACK;
                        sibling.color = Color.RED;
                        leftRotate(sibling);
                        sibling = current.parent.left;
                    }
                    
                    // Case 4: Sibling is black with red left child
                    sibling.color = current.parent.color;
                    current.parent.color = Color.BLACK;
                    sibling.left.color = Color.BLACK;
                    rightRotate(current.parent);
                    current = root;
                }
            }
        }
        current.color = Color.BLACK;
    }
    
    // Delete a node with given data
    public void delete(int data) {
        Node nodeToDelete = search(data);
        if (nodeToDelete == null) return;
        
        Node nodeToRemove = nodeToDelete;
        Node replacementNode;
        Color originalColor = nodeToRemove.color;
        
        // Case 1: Node has no left child
        if (nodeToDelete.left == NIL) {
            replacementNode = nodeToDelete.right;
            transplant(nodeToDelete, nodeToDelete.right);
        } 
        // Case 2: Node has no right child
        else if (nodeToDelete.right == NIL) {
            replacementNode = nodeToDelete.left;
            transplant(nodeToDelete, nodeToDelete.left);
        } 
        // Case 3: Node has two children
        else {
            nodeToRemove = findMinimum(nodeToDelete.right);
            originalColor = nodeToRemove.color;
            replacementNode = nodeToRemove.right;
            
            if (nodeToRemove.parent == nodeToDelete) {
                replacementNode.parent = nodeToRemove;
            } else {
                transplant(nodeToRemove, nodeToRemove.right);
                nodeToRemove.right = nodeToDelete.right;
                nodeToRemove.right.parent = nodeToRemove;
            }
            
            transplant(nodeToDelete, nodeToRemove);
            nodeToRemove.left = nodeToDelete.left;
            nodeToRemove.left.parent = nodeToRemove;
            nodeToRemove.color = nodeToDelete.color;
        }
        
        // Fix violations if a black node was removed
        if (originalColor == Color.BLACK)
            fixDeleteViolations(replacementNode);
    }
    
    // Search for a node with given data
    public Node search(int data) {
        Node current = root;
        while (current != NIL && current.data != data) {
            if (data < current.data)
                current = current.left;
            else
                current = current.right;
        }

        return (current != NIL) ? current : null;
    }
    
    // Find the successor (next larger node) of given node
    public Node successor(Node node) {
        if (node.right != NIL)
            return findMinimum(node.right);
        
        Node parent = node.parent;
        while (parent != NIL && node == parent.right) {
            node = parent;
            parent = parent.parent;
        }

        return (parent != NIL) ? parent : null;
    }
    
    // Find the predecessor (next smaller node) of given node
    public Node predecessor(Node node) {
        if (node.left != NIL)
            return findMaximum(node.left);
        
        Node parent = node.parent;
        while (parent != NIL && node == parent.left) {
            node = parent;
            parent = parent.parent;
        }

        return (parent != NIL) ? parent : null;
    }
    
    // Find the minimum value in the entire tree
    public Node findMin() {
        if (root == NIL)
            return null;

        return findMinimum(root);
    }
    
    // Find the maximum value in the entire tree
    public Node findMax() {
        if (root == NIL)
            return null;

        return findMaximum(root);
    }
    
    // Perform in-order traversal and print nodes
    public void inorder(Node node) {
        if (node != NIL) {
            inorder(node.left);

            System.out.print(node.data + "(" + 
                (node.color == Color.RED ? "R" : "B") + ") ");
                
            inorder(node.right);
        }
    }
    
    public Node getRoot() {
        return root;
    }
    
    // Example usage demonstrating all operations
    public static void main(String[] args) {
        RedBlackTree tree = new RedBlackTree();
        
        // Insert elements
        int[] values = {10, 20, 30, 15, 25, 5, 1};
        for (int value : values) {
            tree.insert(value);
            System.out.print("After inserting " + value + ": ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        // Search for a node
        Node found = tree.search(15);
        if (found != null)
            System.out.println("\nFound: " + found.data);
        
        // Find minimum and maximum
        Node minNode = tree.findMin();
        Node maxNode = tree.findMax();
        System.out.println("Min: " + minNode.data + ", Max: " + maxNode.data);
        
        // Find successor and predecessor
        Node node = tree.search(15);
        if (node != null) {
            Node succ = tree.successor(node);
            Node pred = tree.predecessor(node);
            System.out.println("Successor of 15: " + (succ != null ? succ.data : "null"));
            System.out.println("Predecessor of 15: " + (pred != null ? pred.data : "null"));
        }
        
        // Delete a node
        tree.delete(20);
        System.out.print("\nAfter deleting 20: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}
