enum Color {
    RED, BLACK
}

class Node {
    int data;
    Color color;
    Node left, right;
    
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
        NIL.left = NIL.right = null;
        root = NIL;
    }
    
    // Left rotation
    private Node leftRotate(Node pivot) {
        Node rightChild = pivot.right;
        pivot.right = rightChild.left;
        rightChild.left = pivot;
        return rightChild;
    }
    
    // Right rotation
    private Node rightRotate(Node pivot) {
        Node leftChild = pivot.left;
        pivot.left = leftChild.right;
        leftChild.right = pivot;
        return leftChild;
    }
    
    // Insert and fix violations
    private Node insertFix(Node rootNode, int data, boolean[] inserted) {
        if (rootNode == NIL) {
            inserted[0] = true;
            Node newNode = new Node(data);
            newNode.left = NIL;
            newNode.right = NIL;
            return newNode;
        }
        
        if (data < rootNode.data) {
            rootNode.left = insertFix(rootNode.left, data, inserted);
        } else if (data > rootNode.data) {
            rootNode.right = insertFix(rootNode.right, data, inserted);
        } else {
            return rootNode; // Duplicate
        }
        
        if (!inserted[0])
            return rootNode;
        
        // Fix violations
        if (rootNode.left != NIL && rootNode.left.color == Color.RED &&
            rootNode.left.left != NIL && rootNode.left.left.color == Color.RED) {
            
            if (rootNode.right != NIL && rootNode.right.color == Color.RED) {
                rootNode.color = Color.RED;
                rootNode.left.color = Color.BLACK;
                rootNode.right.color = Color.BLACK;
            } else {
                rootNode = rightRotate(rootNode);
                rootNode.color = Color.BLACK;
                rootNode.right.color = Color.RED;
            }
        } else if (rootNode.left != NIL && rootNode.left.color == Color.RED &&
                   rootNode.left.right != NIL && rootNode.left.right.color == Color.RED) {
            
            if (rootNode.right != NIL && rootNode.right.color == Color.RED) {
                rootNode.color = Color.RED;
                rootNode.left.color = Color.BLACK;
                rootNode.right.color = Color.BLACK;
            } else {
                rootNode.left = leftRotate(rootNode.left);
                rootNode = rightRotate(rootNode);
                rootNode.color = Color.BLACK;
                rootNode.right.color = Color.RED;
            }
        } else if (rootNode.right != NIL && rootNode.right.color == Color.RED &&
                   rootNode.right.right != NIL && rootNode.right.right.color == Color.RED) {
            
            if (rootNode.left != NIL && rootNode.left.color == Color.RED) {
                rootNode.color = Color.RED;
                rootNode.left.color = Color.BLACK;
                rootNode.right.color = Color.BLACK;
            } else {
                rootNode = leftRotate(rootNode);
                rootNode.color = Color.BLACK;
                rootNode.left.color = Color.RED;
            }
        } else if (rootNode.right != NIL && rootNode.right.color == Color.RED &&
                   rootNode.right.left != NIL && rootNode.right.left.color == Color.RED) {
            
            if (rootNode.left != NIL && rootNode.left.color == Color.RED) {
                rootNode.color = Color.RED;
                rootNode.left.color = Color.BLACK;
                rootNode.right.color = Color.BLACK;
            } else {
                rootNode.right = rightRotate(rootNode.right);
                rootNode = leftRotate(rootNode);
                rootNode.color = Color.BLACK;
                rootNode.left.color = Color.RED;
            }
        }
        
        return rootNode;
    }
    
    // Insert a node
    public void insert(int data) {
        boolean[] inserted = {false};
        root = insertFix(root, data, inserted);
        root.color = Color.BLACK;
    }
    
    // Find minimum node
    private Node findMinimum(Node subtreeRoot) {
        while (subtreeRoot.left != NIL)
            subtreeRoot = subtreeRoot.left;
        return subtreeRoot;
    }
    
    // Find maximum node
    private Node findMaximum(Node subtreeRoot) {
        while (subtreeRoot.right != NIL)
            subtreeRoot = subtreeRoot.right;
        return subtreeRoot;
    }
    
    // Helper for deletion fixup - left case
    private Node deleteFixLeft(Node rootNode) {
        Node sibling = rootNode.right;
        
        if (sibling.color == Color.RED) {
            rootNode = leftRotate(rootNode);
            rootNode.color = Color.BLACK;
            rootNode.left.color = Color.RED;
            sibling = rootNode.left.right;
        }
        
        if ((sibling.left == NIL || sibling.left.color == Color.BLACK) &&
            (sibling.right == NIL || sibling.right.color == Color.BLACK)) {
            sibling.color = Color.RED;
            return rootNode;
        }
        
        if (sibling.right == NIL || sibling.right.color == Color.BLACK) {
            sibling = rightRotate(sibling);
            sibling.color = Color.BLACK;
            sibling.right.color = Color.RED;
            rootNode.right = sibling;
        }
        
        rootNode = leftRotate(rootNode);
        rootNode.color = rootNode.left.color;
        rootNode.left.color = Color.BLACK;
        rootNode.right.color = Color.BLACK;
        
        return rootNode;
    }
    
    // Helper for deletion fixup - right case
    private Node deleteFixRight(Node rootNode) {
        Node sibling = rootNode.left;
        
        if (sibling.color == Color.RED) {
            rootNode = rightRotate(rootNode);
            rootNode.color = Color.BLACK;
            rootNode.right.color = Color.RED;
            sibling = rootNode.right.left;
        }
        
        if ((sibling.left == NIL || sibling.left.color == Color.BLACK) &&
            (sibling.right == NIL || sibling.right.color == Color.BLACK)) {
            sibling.color = Color.RED;
            return rootNode;
        }
        
        if (sibling.left == NIL || sibling.left.color == Color.BLACK) {
            sibling = leftRotate(sibling);
            sibling.color = Color.BLACK;
            sibling.left.color = Color.RED;
            rootNode.left = sibling;
        }
        
        rootNode = rightRotate(rootNode);
        rootNode.color = rootNode.right.color;
        rootNode.right.color = Color.BLACK;
        rootNode.left.color = Color.BLACK;
        
        return rootNode;
    }
    
    // Delete helper
    private Node deleteHelper(Node rootNode, int data, boolean[] done) {
        if (rootNode == NIL)
            return rootNode;
        
        if (data < rootNode.data) {
            rootNode.left = deleteHelper(rootNode.left, data, done);
            if (!done[0])
                rootNode = deleteFixLeft(rootNode);
        } else if (data > rootNode.data) {
            rootNode.right = deleteHelper(rootNode.right, data, done);
            if (!done[0])
                rootNode = deleteFixRight(rootNode);
        } else {
            if (rootNode.left == NIL || rootNode.right == NIL) {
                Node temp = (rootNode.left != NIL) ? rootNode.left : rootNode.right;
                done[0] = (rootNode.color == Color.RED);
                return temp;
            }
            
            Node successor = findMinimum(rootNode.right);
            rootNode.data = successor.data;
            rootNode.right = deleteHelper(rootNode.right, successor.data, done);
            if (!done[0])
                rootNode = deleteFixRight(rootNode);
        }
        
        return rootNode;
    }
    
    // Delete a node
    public void delete(int data) {
        boolean[] done = {false};
        root = deleteHelper(root, data, done);
        if (root != NIL)
            root.color = Color.BLACK;
    }
    
    // Search for a node
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
    
    // Find successor
    private Node successorHelper(Node rootNode, int data, Node succ) {
        if (rootNode == NIL)
            return succ;
        
        if (data < rootNode.data)
            return successorHelper(rootNode.left, data, rootNode);
        else if (data > rootNode.data)
            return successorHelper(rootNode.right, data, succ);
        else {
            if (rootNode.right != NIL)
                return findMinimum(rootNode.right);
            return succ;
        }
    }
    
    public Node successor(Node node) {
        Node result = successorHelper(root, node.data, NIL);
        return (result != NIL) ? result : null;
    }
    
    // Find predecessor
    private Node predecessorHelper(Node rootNode, int data, Node pred) {
        if (rootNode == NIL)
            return pred;
        
        if (data < rootNode.data)
            return predecessorHelper(rootNode.left, data, pred);
        else if (data > rootNode.data)
            return predecessorHelper(rootNode.right, data, rootNode);
        else {
            if (rootNode.left != NIL)
                return findMaximum(rootNode.left);
            return pred;
        }
    }
    
    public Node predecessor(Node node) {
        Node result = predecessorHelper(root, node.data, NIL);
        return (result != NIL) ? result : null;
    }
    
    // Find minimum in tree
    public Node findMin() {
        if (root == NIL)
            return null;
        return findMinimum(root);
    }
    
    // Find maximum in tree
    public Node findMax() {
        if (root == NIL)
            return null;
        return findMaximum(root);
    }
    
    // In-order traversal
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
    
    // Example usage
    public static void main(String[] args) {
        RedBlackTree tree = new RedBlackTree();
        
        int[] values = {10, 20, 30, 15, 25, 5, 1};
        for (int value : values) {
            tree.insert(value);
            System.out.print("After inserting " + value + ": ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        Node found = tree.search(15);
        if (found != null)
            System.out.println("\nFound: " + found.data);
        
        Node minNode = tree.findMin();
        Node maxNode = tree.findMax();
        System.out.println("Min: " + minNode.data + ", Max: " + maxNode.data);
        
        Node node = tree.search(15);
        if (node != null) {
            Node succ = tree.successor(node);
            Node pred = tree.predecessor(node);
            System.out.println("Successor of 15: " + (succ != null ? succ.data : "null"));
            System.out.println("Predecessor of 15: " + (pred != null ? pred.data : "null"));
        }
        
        tree.delete(20);
        System.out.print("\nAfter deleting 20: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}
