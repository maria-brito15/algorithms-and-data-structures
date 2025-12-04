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
    private final Node NIL;
    
    public RedBlackTree() {
        NIL = new Node(0);
        NIL.color = Color.BLACK;
        NIL.left = NIL.right = null;
        root = NIL;
    }
    
    // ==================== UTILITY METHODS ====================
    
    private boolean isRed(Node node) {
        return node != NIL && node.color == Color.RED;
    }
    
    private boolean isBlack(Node node) {
        return node == NIL || node.color == Color.BLACK;
    }
    
    // ==================== ROTATION METHODS ====================
    
    private Node leftRotate(Node pivot) {
        Node rightChild = pivot.right;
        pivot.right = rightChild.left;
        rightChild.left = pivot;
        return rightChild;
    }
    
    private Node rightRotate(Node pivot) {
        Node leftChild = pivot.left;
        pivot.left = leftChild.right;
        leftChild.right = pivot;
        return leftChild;
    }
    
    // ==================== INSERTION ====================
    
    public void insert(int data) {
        boolean[] inserted = {false};
        root = insertRecursive(root, data, inserted);
        root.color = Color.BLACK;
    }
    
    private Node insertRecursive(Node node, int data, boolean[] inserted) {
        if (node == NIL) {
            inserted[0] = true;
            return createNode(data);
        }
        
        if (data < node.data) {
            node.left = insertRecursive(node.left, data, inserted);
        } else if (data > node.data) {
            node.right = insertRecursive(node.right, data, inserted);
        } else {
            return node; // Duplicate
        }
        
        if (inserted[0]) {
            node = fixInsertViolations(node);
        }
        
        return node;
    }
    
    private Node createNode(int data) {
        Node newNode = new Node(data);
        newNode.left = NIL;
        newNode.right = NIL;
        return newNode;
    }
    
    private Node fixInsertViolations(Node node) {
        // Fix left side violations
        if (isRed(node.left) && (isRed(node.left.left) || isRed(node.left.right))) {
            if (isRed(node.right)) {
                recolorNode(node);
            } else {
                node = fixLeftInsertViolation(node);
            }
        }
        
        // Fix right side violations
        if (isRed(node.right) && (isRed(node.right.right) || isRed(node.right.left))) {
            if (isRed(node.left)) {
                recolorNode(node);
            } else {
                node = fixRightInsertViolation(node);
            }
        }
        
        return node;
    }
    
    private Node fixLeftInsertViolation(Node node) {
        if (isRed(node.left.left)) {
            // Left-Left case
            node = rightRotate(node);
        } else {
            // Left-Right case
            node.left = leftRotate(node.left);
            node = rightRotate(node);
        }
        node.color = Color.BLACK;
        node.right.color = Color.RED;
        return node;
    }
    
    private Node fixRightInsertViolation(Node node) {
        if (isRed(node.right.right)) {
            // Right-Right case
            node = leftRotate(node);
        } else {
            // Right-Left case
            node.right = rightRotate(node.right);
            node = leftRotate(node);
        }
        
        node.color = Color.BLACK;
        node.left.color = Color.RED;
        return node;
    }
    
    private void recolorNode(Node node) {
        node.color = Color.RED;
        node.left.color = Color.BLACK;
        node.right.color = Color.BLACK;
    }
    
    // ==================== DELETION ====================
    
    public void delete(int data) {
        boolean[] done = {false};
        root = deleteRecursive(root, data, done);
        if (root != NIL) {
            root.color = Color.BLACK;
        }
    }
    
    private Node deleteRecursive(Node node, int data, boolean[] done) {
        if (node == NIL) {
            return node;
        }
        
        if (data < node.data) {
            node.left = deleteRecursive(node.left, data, done);
            if (!done[0]) {
                node = fixDeleteLeftCase(node);
            }
        } else if (data > node.data) {
            node.right = deleteRecursive(node.right, data, done);
            if (!done[0]) {
                node = fixDeleteRightCase(node);
            }
        } else {
            node = deleteNode(node, done);
        }
        
        return node;
    }
    
    private Node deleteNode(Node node, boolean[] done) {
        if (node.left == NIL || node.right == NIL) {
            // Node has at most one child
            Node child = (node.left != NIL) ? node.left : node.right;
            done[0] = isRed(node);
            return child;
        }
        
        // Node has two children - replace with successor
        Node successor = findMinimum(node.right);
        node.data = successor.data;
        node.right = deleteRecursive(node.right, successor.data, done);
        
        if (!done[0]) {
            node = fixDeleteRightCase(node);
        }
        
        return node;
    }
    
    private Node fixDeleteLeftCase(Node node) {
        Node sibling = node.right;
        
        // Case 1: Red sibling
        if (isRed(sibling)) {
            node = leftRotate(node);
            node.color = Color.BLACK;
            node.left.color = Color.RED;
            sibling = node.left.right;
        }
        
        // Case 2: Black sibling with two black children
        if (isBlack(sibling.left) && isBlack(sibling.right)) {
            sibling.color = Color.RED;
            return node;
        }
        
        // Case 3: Black sibling with red left child and black right child
        if (isBlack(sibling.right)) {
            sibling = rightRotate(sibling);
            sibling.color = Color.BLACK;
            sibling.right.color = Color.RED;
            node.right = sibling;
        }
        
        // Case 4: Black sibling with red right child
        node = leftRotate(node);
        node.color = node.left.color;
        node.left.color = Color.BLACK;
        node.right.color = Color.BLACK;
        
        return node;
    }
    
    private Node fixDeleteRightCase(Node node) {
        Node sibling = node.left;
        
        // Case 1: Red sibling
        if (isRed(sibling)) {
            node = rightRotate(node);
            node.color = Color.BLACK;
            node.right.color = Color.RED;
            sibling = node.right.left;
        }
        
        // Case 2: Black sibling with two black children
        if (isBlack(sibling.left) && isBlack(sibling.right)) {
            sibling.color = Color.RED;
            return node;
        }
        
        // Case 3: Black sibling with red right child and black left child
        if (isBlack(sibling.left)) {
            sibling = leftRotate(sibling);
            sibling.color = Color.BLACK;
            sibling.left.color = Color.RED;
            node.left = sibling;
        }
        
        // Case 4: Black sibling with red left child
        node = rightRotate(node);
        node.color = node.right.color;
        node.right.color = Color.BLACK;
        node.left.color = Color.BLACK;
        
        return node;
    }
    
    // ==================== SEARCH OPERATIONS ====================
    
    public Node search(int data) {
        Node current = root;
        while (current != NIL && current.data != data) {
            current = (data < current.data) ? current.left : current.right;
        }
        return (current != NIL) ? current : null;
    }
    
    public Node findMin() {
        return (root == NIL) ? null : findMinimum(root);
    }
    
    public Node findMax() {
        return (root == NIL) ? null : findMaximum(root);
    }
    
    private Node findMinimum(Node node) {
        while (node.left != NIL) {
            node = node.left;
        }
        return node;
    }
    
    private Node findMaximum(Node node) {
        while (node.right != NIL) {
            node = node.right;
        }
        return node;
    }
    
    // ==================== SUCCESSOR/PREDECESSOR ====================
    
    public Node successor(Node node) {
        if (node == null) return null;
        Node result = findSuccessor(root, node.data, NIL);
        return (result != NIL) ? result : null;
    }
    
    private Node findSuccessor(Node node, int data, Node successor) {
        if (node == NIL) {
            return successor;
        }
        
        if (data < node.data) {
            return findSuccessor(node.left, data, node);
        } else if (data > node.data) {
            return findSuccessor(node.right, data, successor);
        } else {
            return (node.right != NIL) ? findMinimum(node.right) : successor;
        }
    }
    
    public Node predecessor(Node node) {
        if (node == null) return null;
        Node result = findPredecessor(root, node.data, NIL);
        return (result != NIL) ? result : null;
    }
    
    private Node findPredecessor(Node node, int data, Node predecessor) {
        if (node == NIL) {
            return predecessor;
        }
        
        if (data < node.data) {
            return findPredecessor(node.left, data, predecessor);
        } else if (data > node.data) {
            return findPredecessor(node.right, data, node);
        } else {
            return (node.left != NIL) ? findMaximum(node.left) : predecessor;
        }
    }
    
    // ==================== TRAVERSAL ====================
    
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
    
    // ==================== MAIN METHOD ====================
    
    public static void main(String[] args) {
        RedBlackTree tree = new RedBlackTree();
        
        int[] values = {10, 20, 30, 15, 25, 5, 1};
        System.out.println("=== INSERTION ===");
        for (int value : values) {
            tree.insert(value);
            System.out.print("After inserting " + value + ": ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        System.out.println("\n=== SEARCH ===");
        Node found = tree.search(15);
        if (found != null) {
            System.out.println("Found: " + found.data);
        }
        
        System.out.println("\n=== MIN/MAX ===");
        Node minNode = tree.findMin();
        Node maxNode = tree.findMax();
        System.out.println("Min: " + minNode.data + ", Max: " + maxNode.data);
        
        System.out.println("\n=== SUCCESSOR/PREDECESSOR ===");
        Node node = tree.search(15);
        if (node != null) {
            Node succ = tree.successor(node);
            Node pred = tree.predecessor(node);
            System.out.println("Successor of 15: " + (succ != null ? succ.data : "null"));
            System.out.println("Predecessor of 15: " + (pred != null ? pred.data : "null"));
        }
        
        System.out.println("\n=== DELETION ===");
        tree.delete(20);
        System.out.print("After deleting 20: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}