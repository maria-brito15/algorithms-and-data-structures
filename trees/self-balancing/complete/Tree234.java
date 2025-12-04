class Node {
    private static final int MAX_KEYS = 3;
    private static final int MAX_CHILDREN = 4;
    
    int[] keys;
    Node[] children;
    Node parent;
    int numKeys;
    boolean isLeaf;
    
    public Node(boolean isLeaf) {
        this.keys = new int[MAX_KEYS];
        this.children = new Node[MAX_CHILDREN];
        this.numKeys = 0;
        this.isLeaf = isLeaf;
        this.parent = null;
    }
}

public class Tree234 {
    private Node root;
    
    public Tree234() {
        root = new Node(true);
    }
    
    // Check if node is full (has 3 keys)
    private boolean isFull(Node node) {
        return node.numKeys == 3;
    }
    
    // Search for a key in the tree
    public Node search(int key) {
        Node current = root;
        
        while (current != null) {
            int i = 0;
            while (i < current.numKeys && key > current.keys[i])
                i++;
            
            if (i < current.numKeys && key == current.keys[i])
                return current;
            
            if (current.isLeaf)
                return null;
            
            current = current.children[i];
        }
        
        return null;
    }
    
    // Split a full child node
    private void splitChild(Node parent, int index, Node child) {
        Node newNode = new Node(child.isLeaf);
        newNode.numKeys = 1;
        
        // Middle key goes to parent
        int middleKey = child.keys[1];
        
        // Right key goes to new node
        newNode.keys[0] = child.keys[2];
        
        // If not leaf, move children
        if (!child.isLeaf) {
            newNode.children[0] = child.children[2];
            newNode.children[1] = child.children[3];
            
            if (newNode.children[0] != null)
                newNode.children[0].parent = newNode;
            if (newNode.children[1] != null)
                newNode.children[1].parent = newNode;
            
            child.children[2] = null;
            child.children[3] = null;
        }
        
        // Left key stays in original node
        child.numKeys = 1;
        
        // Insert middle key into parent
        for (int i = parent.numKeys; i > index; i--) {
            parent.keys[i] = parent.keys[i - 1];
            parent.children[i + 1] = parent.children[i];
        }
        
        parent.keys[index] = middleKey;
        parent.children[index + 1] = newNode;
        parent.numKeys++;
        
        newNode.parent = parent;
    }
    
    // Insert a key into a non-full node
    private void insertNonFull(Node node, int key) {
        int i = node.numKeys - 1;
        
        if (node.isLeaf) {
            // Insert key in sorted order
            while (i >= 0 && key < node.keys[i]) {
                node.keys[i + 1] = node.keys[i];
                i--;
            }
            node.keys[i + 1] = key;
            node.numKeys++;
        } else {
            // Find child to insert into
            while (i >= 0 && key < node.keys[i])
                i--;
            i++;
            
            // Split child if full
            if (isFull(node.children[i])) {
                splitChild(node, i, node.children[i]);
                
                if (key > node.keys[i])
                    i++;
            }
            
            insertNonFull(node.children[i], key);
        }
    }
    
    // Insert a key into the tree
    public void insert(int key) {
        if (isFull(root)) {
            Node newRoot = new Node(false);
            newRoot.children[0] = root;
            root.parent = newRoot;
            
            splitChild(newRoot, 0, root);
            root = newRoot;
            
            insertNonFull(newRoot, key);
        } else {
            insertNonFull(root, key);
        }
    }
    
    // Find the minimum key in a subtree
    private int findMinimum(Node node) {
        while (!node.isLeaf)
            node = node.children[0];
        
        return node.keys[0];
    }
    
    // Find the maximum key in a subtree
    private int findMaximum(Node node) {
        while (!node.isLeaf)
            node = node.children[node.numKeys];
        
        return node.keys[node.numKeys - 1];
    }
    
    // Merge a key and two children
    private void merge(Node parent, int index) {
        Node left = parent.children[index];
        Node right = parent.children[index + 1];
        
        // Move key from parent to left child
        left.keys[left.numKeys] = parent.keys[index];
        left.numKeys++;
        
        // Copy keys from right to left
        for (int i = 0; i < right.numKeys; i++) {
            left.keys[left.numKeys] = right.keys[i];
            left.numKeys++;
        }
        
        // Copy children from right to left
        if (!left.isLeaf) {
            for (int i = 0; i <= right.numKeys; i++) {
                left.children[left.numKeys - right.numKeys + i] = right.children[i];
                if (right.children[i] != null)
                    right.children[i].parent = left;
            }
        }
        
        // Remove key from parent
        for (int i = index; i < parent.numKeys - 1; i++) {
            parent.keys[i] = parent.keys[i + 1];
            parent.children[i + 1] = parent.children[i + 2];
        }
        parent.numKeys--;
    }
    
    // Borrow a key from left sibling
    private void borrowFromLeft(Node parent, int index) {
        Node child = parent.children[index];
        Node sibling = parent.children[index - 1];
        
        // Move all keys in child one step ahead
        for (int i = child.numKeys; i > 0; i--)
            child.keys[i] = child.keys[i - 1];
        
        // Move children
        if (!child.isLeaf) {
            for (int i = child.numKeys + 1; i > 0; i--)
                child.children[i] = child.children[i - 1];
        }
        
        // Move key from parent to child
        child.keys[0] = parent.keys[index - 1];
        
        // Move key from sibling to parent
        parent.keys[index - 1] = sibling.keys[sibling.numKeys - 1];
        
        // Move child pointer
        if (!child.isLeaf) {
            child.children[0] = sibling.children[sibling.numKeys];
            if (child.children[0] != null)
                child.children[0].parent = child;
            sibling.children[sibling.numKeys] = null;
        }
        
        child.numKeys++;
        sibling.numKeys--;
    }
    
    // Borrow a key from right sibling
    private void borrowFromRight(Node parent, int index) {
        Node child = parent.children[index];
        Node sibling = parent.children[index + 1];
        
        // Move key from parent to child
        child.keys[child.numKeys] = parent.keys[index];
        
        // Move key from sibling to parent
        parent.keys[index] = sibling.keys[0];
        
        // Move child pointer
        if (!child.isLeaf) {
            child.children[child.numKeys + 1] = sibling.children[0];
            if (child.children[child.numKeys + 1] != null)
                child.children[child.numKeys + 1].parent = child;
        }
        
        // Shift keys in sibling
        for (int i = 0; i < sibling.numKeys - 1; i++)
            sibling.keys[i] = sibling.keys[i + 1];
        
        // Shift children in sibling
        if (!sibling.isLeaf) {
            for (int i = 0; i < sibling.numKeys; i++)
                sibling.children[i] = sibling.children[i + 1];
        }
        
        child.numKeys++;
        sibling.numKeys--;
    }
    
    // Fill child if it has minimum keys
    private void fill(Node parent, int index) {
        // Borrow from left sibling
        if (index != 0 && parent.children[index - 1].numKeys > 1)
            borrowFromLeft(parent, index);
        
        // Borrow from right sibling
        else if (index != parent.numKeys && parent.children[index + 1].numKeys > 1)
            borrowFromRight(parent, index);
        
        // Merge with left sibling
        else if (index != 0)
            merge(parent, index - 1);
        
        // Merge with right sibling
        else
            merge(parent, index);
    }
    
    // Delete key from a leaf node
    private void deleteFromLeaf(Node node, int index) {
        for (int i = index; i < node.numKeys - 1; i++)
            node.keys[i] = node.keys[i + 1];
        
        node.numKeys--;
    }
    
    // Delete key from an internal node
    private void deleteFromInternal(Node node, int index) {
        int key = node.keys[index];
        
        if (node.children[index].numKeys > 1) {
            int pred = findMaximum(node.children[index]);
            node.keys[index] = pred;
            deleteHelper(node.children[index], pred);
        } else if (node.children[index + 1].numKeys > 1) {
            int succ = findMinimum(node.children[index + 1]);
            node.keys[index] = succ;
            deleteHelper(node.children[index + 1], succ);
        } else {
            merge(node, index);
            deleteHelper(node.children[index], key);
        }
    }
    
    // Delete helper function
    private void deleteHelper(Node node, int key) {
        int i = 0;
        while (i < node.numKeys && key > node.keys[i])
            i++;
        
        if (i < node.numKeys && key == node.keys[i]) {
            if (node.isLeaf)
                deleteFromLeaf(node, i);
            else
                deleteFromInternal(node, i);
        } else if (!node.isLeaf) {
            boolean isInLastChild = (i == node.numKeys);
            
            if (node.children[i].numKeys < 2)
                fill(node, i);
            
            if (isInLastChild && i > node.numKeys)
                deleteHelper(node.children[i - 1], key);
            else
                deleteHelper(node.children[i], key);
        }
    }
    
    // Delete a key from the tree
    public void delete(int key) {
        if (root == null)
            return;
        
        deleteHelper(root, key);
        
        // If root is empty, make its only child the new root
        if (root.numKeys == 0) {
            if (root.isLeaf)
                root = null;
            else {
                root = root.children[0];
                if (root != null)
                    root.parent = null;
            }
        }
    }
    
    // Find minimum key in tree
    public int findMin() {
        if (root == null)
            return -1;
        
        return findMinimum(root);
    }
    
    // Find maximum key in tree
    public int findMax() {
        if (root == null)
            return -1;
        
        return findMaximum(root);
    }
    
    // In-order traversal
    public void inorder(Node node) {
        if (node != null) {
            for (int i = 0; i < node.numKeys; i++) {
                if (!node.isLeaf)
                    inorder(node.children[i]);
                System.out.print(node.keys[i] + " ");
            }
            if (!node.isLeaf)
                inorder(node.children[node.numKeys]);
        }
    }
    
    // Print tree structure (level by level)
    private void printLevel(Node node, int level, int currentLevel) {
        if (node == null)
            return;
        
        if (currentLevel == level) {
            System.out.print("[");
            for (int i = 0; i < node.numKeys; i++) {
                System.out.print(node.keys[i]);
                if (i < node.numKeys - 1)
                    System.out.print(",");
            }
            System.out.print("] ");
        } else if (currentLevel < level) {
            for (int i = 0; i <= node.numKeys; i++)
                printLevel(node.children[i], level, currentLevel + 1);
        }
    }
    
    public void printTree() {
        if (root == null) {
            System.out.println("Empty tree");
            return;
        }
        
        int height = 0;
        Node temp = root;
        while (!temp.isLeaf) {
            temp = temp.children[0];
            height++;
        }
        
        for (int i = 0; i <= height; i++) {
            System.out.print("Level " + i + ": ");
            printLevel(root, i, 0);
            System.out.println();
        }
    }
    
    public Node getRoot() {
        return root;
    }
    
    // Example usage
    public static void main(String[] args) {
        Tree234 tree = new Tree234();
        
        // Insert elements
        int[] values = {10, 20, 30, 15, 25, 5, 1, 35, 40, 50};
        for (int value : values) {
            tree.insert(value);
            System.out.println("\nAfter inserting " + value + ":");
            tree.printTree();
            System.out.print("In-order: ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        // Search for a key
        Node found = tree.search(15);
        System.out.println("\n" + (found != null ? "Found:" : "Not found:") + " 15");
        
        // Find minimum and maximum
        System.out.println("Min: " + tree.findMin() + ", Max: " + tree.findMax());
        
        // Delete a key
        tree.delete(20);
        System.out.println("\nAfter deleting 20:");
        tree.printTree();
        System.out.print("In-order: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}
