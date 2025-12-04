class Node {
    int data;
    Node left, right;
    
    public Node(int data) {
        this.data = data;
        this.left = null;
        this.right = null;
    }
}

public class AVLTree {
    private Node root;
    
    public AVLTree() {
        root = null;
    }
    
    // Get height of a node
    private int getHeight(Node node) {
        if (node == null) return 0;
        int lh = getHeight(node.left);
        int rh = getHeight(node.right);
        return Math.max(lh, rh) + 1;
    }
    
    // Get balance factor of a node
    private int getBalance(Node node) {
        return node != null ? getHeight(node.left) - getHeight(node.right) : 0;
    }
    
    // Left rotation around pivot node
    private Node leftRotate(Node pivot) {
        Node rightChild = pivot.right;
        Node leftSubtree = rightChild.left;
        
        rightChild.left = pivot;
        pivot.right = leftSubtree;
        
        return rightChild;
    }
    
    // Right rotation around pivot node
    private Node rightRotate(Node pivot) {
        Node leftChild = pivot.left;
        Node rightSubtree = leftChild.right;
        
        leftChild.right = pivot;
        pivot.left = rightSubtree;
        
        return leftChild;
    }
    
    // Balance the node after insertion or deletion
    private Node balance(Node node) {
        int balanceFactor = getBalance(node);
        
        // Left-Left Case
        if (balanceFactor > 1 && getBalance(node.left) >= 0)
            return rightRotate(node);
        
        // Left-Right Case
        if (balanceFactor > 1 && getBalance(node.left) < 0) {
            node.left = leftRotate(node.left);
            return rightRotate(node);
        }
        
        // Right-Right Case
        if (balanceFactor < -1 && getBalance(node.right) <= 0)
            return leftRotate(node);
        
        // Right-Left Case
        if (balanceFactor < -1 && getBalance(node.right) > 0) {
            node.right = rightRotate(node.right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    // Insert helper function
    private Node insertNode(Node node, int data) {
        if (node == null)
            return new Node(data);
        
        if (data < node.data)
            node.left = insertNode(node.left, data);
        else if (data > node.data)
            node.right = insertNode(node.right, data);
        else
            return node; // Duplicate values not allowed
        
        return balance(node);
    }
    
    // Insert a node with given data
    public void insert(int data) {
        root = insertNode(root, data);
    }
    
    // Find the node with minimum value in subtree
    private Node findMinimum(Node subtreeRoot) {
        while (subtreeRoot != null && subtreeRoot.left != null)
            subtreeRoot = subtreeRoot.left;
        return subtreeRoot;
    }
    
    // Find the node with maximum value in subtree
    private Node findMaximum(Node subtreeRoot) {
        while (subtreeRoot != null && subtreeRoot.right != null)
            subtreeRoot = subtreeRoot.right;
        return subtreeRoot;
    }
    
    // Delete helper function
    private Node deleteNodeHelper(Node node, int data) {
        if (node == null)
            return node;
        
        if (data < node.data) {
            node.left = deleteNodeHelper(node.left, data);
        } else if (data > node.data) {
            node.right = deleteNodeHelper(node.right, data);
        } else {
            if (node.left == null)
                return node.right;
            else if (node.right == null)
                return node.left;
            
            Node temp = findMinimum(node.right);
            node.data = temp.data;
            node.right = deleteNodeHelper(node.right, temp.data);
        }
        
        return balance(node);
    }
    
    // Delete a node with given data
    public void delete(int data) {
        root = deleteNodeHelper(root, data);
    }
    
    // Search for a node with given data
    public Node search(int data) {
        Node current = root;
        while (current != null && current.data != data) {
            if (data < current.data)
                current = current.left;
            else
                current = current.right;
        }
        return current;
    }
    
    // Find successor using in-order traversal approach
    private Node successorHelper(Node rootNode, Node target, Node succ) {
        if (rootNode == null)
            return succ;
        
        if (target.data < rootNode.data) {
            return successorHelper(rootNode.left, target, rootNode);
        } else if (target.data > rootNode.data) {
            return successorHelper(rootNode.right, target, succ);
        } else {
            if (rootNode.right != null)
                return findMinimum(rootNode.right);
            return succ;
        }
    }
    
    public Node successor(Node node) {
        return successorHelper(root, node, null);
    }
    
    // Find predecessor using in-order traversal approach
    private Node predecessorHelper(Node rootNode, Node target, Node pred) {
        if (rootNode == null)
            return pred;
        
        if (target.data < rootNode.data) {
            return predecessorHelper(rootNode.left, target, pred);
        } else if (target.data > rootNode.data) {
            return predecessorHelper(rootNode.right, target, rootNode);
        } else {
            if (rootNode.left != null)
                return findMaximum(rootNode.left);
            return pred;
        }
    }
    
    public Node predecessor(Node node) {
        return predecessorHelper(root, node, null);
    }
    
    // Find the minimum value in the entire tree
    public Node findMin() {
        if (root == null)
            return null;
        return findMinimum(root);
    }
    
    // Find the maximum value in the entire tree
    public Node findMax() {
        if (root == null)
            return null;
        return findMaximum(root);
    }
    
    // Perform in-order traversal and print nodes
    public void inorder(Node node) {
        if (node != null) {
            inorder(node.left);
            System.out.print(node.data + " ");
            inorder(node.right);
        }
    }
    
    public Node getRoot() {
        return root;
    }
    
    // Example usage demonstrating all operations
    public static void main(String[] args) {
        AVLTree tree = new AVLTree();
        
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
