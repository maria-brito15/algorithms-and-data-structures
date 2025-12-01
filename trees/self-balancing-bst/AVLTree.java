class Node {
    int data;
    int height;
    Node left, right, parent;
    
    public Node(int data) {
        this.data = data;
        this.height = 1;
        this.left = null;
        this.right = null;
        this.parent = null;
    }
}

public class AVLTree {
    private Node root;
    
    public AVLTree() {
        root = null;
    }
    
    // Get height of a node
    private int getHeight(Node node) {
        return node != null ? node.height : 0;
    }
    
    // Get balance factor of a node
    private int getBalance(Node node) {
        return node != null ? getHeight(node.left) - getHeight(node.right) : 0;
    }
    
    // Update height of a node
    private void updateHeight(Node node) {
        if (node != null) {
            int leftHeight = getHeight(node.left);
            int rightHeight = getHeight(node.right);
            node.height = Math.max(leftHeight, rightHeight) + 1;
        }
    }
    
    // Left rotation around pivot node
    private Node leftRotate(Node pivot) {
        Node rightChild = pivot.right;
        Node leftSubtree = rightChild.left;
        
        // Perform rotation
        rightChild.left = pivot;
        pivot.right = leftSubtree;
        
        // Update parent pointers
        rightChild.parent = pivot.parent;
        pivot.parent = rightChild;
        if (leftSubtree != null)
            leftSubtree.parent = pivot;
        
        // Update heights
        updateHeight(pivot);
        updateHeight(rightChild);
        
        return rightChild;
    }
    
    // Right rotation around pivot node
    private Node rightRotate(Node pivot) {
        Node leftChild = pivot.left;
        Node rightSubtree = leftChild.right;
        
        // Perform rotation
        leftChild.right = pivot;
        pivot.left = rightSubtree;
        
        // Update parent pointers
        leftChild.parent = pivot.parent;
        pivot.parent = leftChild;
        if (rightSubtree != null)
            rightSubtree.parent = pivot;
        
        // Update heights
        updateHeight(pivot);
        updateHeight(leftChild);
        
        return leftChild;
    }
    
    // Balance the node after insertion or deletion
    private Node balance(Node node) {
        updateHeight(node);
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
    private Node insertNode(Node node, int data, Node parent) {
        // Perform standard BST insertion
        if (node == null) {
            Node newNode = new Node(data);
            newNode.parent = parent;
            return newNode;
        }
        
        if (data < node.data)
            node.left = insertNode(node.left, data, node);
        else if (data > node.data)
            node.right = insertNode(node.right, data, node);
        else
            return node; // Duplicate values not allowed
        
        // Balance the node
        return balance(node);
    }
    
    // Insert a node with given data
    public void insert(int data) {
        root = insertNode(root, data, null);
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
        
        // Perform standard BST deletion
        if (data < node.data) {
            node.left = deleteNodeHelper(node.left, data);
        } else if (data > node.data) {
            node.right = deleteNodeHelper(node.right, data);
        } else {
            // Node with one child or no child
            if (node.left == null) {
                Node temp = node.right;
                if (temp != null)
                    temp.parent = node.parent;
                return temp;
            } else if (node.right == null) {
                Node temp = node.left;
                if (temp != null)
                    temp.parent = node.parent;
                return temp;
            }
            
            // Node with two children
            Node temp = findMinimum(node.right);
            node.data = temp.data;
            node.right = deleteNodeHelper(node.right, temp.data);
        }
        
        // Balance the node
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
    
    // Find the successor (next larger node) of given node
    public Node successor(Node node) {
        if (node.right != null)
            return findMinimum(node.right);
        
        Node parent = node.parent;
        while (parent != null && node == parent.right) {
            node = parent;
            parent = parent.parent;
        }
        return parent;
    }
    
    // Find the predecessor (next smaller node) of given node
    public Node predecessor(Node node) {
        if (node.left != null)
            return findMaximum(node.left);
        
        Node parent = node.parent;
        while (parent != null && node == parent.left) {
            node = parent;
            parent = parent.parent;
        }
        return parent;
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
            System.out.print(node.data + "(h=" + node.height + ") ");
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
