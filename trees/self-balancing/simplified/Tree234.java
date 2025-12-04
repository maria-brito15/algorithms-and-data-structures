class Node {
    private static final int MAX_KEYS = 3;
    private static final int MAX_CHILDREN = 4;
    
    int[] keys;
    Node[] children;
    int numKeys;
    boolean isLeaf;
    
    public Node(boolean isLeaf) {
        this.keys = new int[MAX_KEYS];
        this.children = new Node[MAX_CHILDREN];
        this.numKeys = 0;
        this.isLeaf = isLeaf;
    }
}

class SplitResult {
    Node left;
    int middleKey;
    Node right;
    
    public SplitResult(Node left, int middleKey, Node right) {
        this.left = left;
        this.middleKey = middleKey;
        this.right = right;
    }
}

public class Tree234 {
    private Node root;
    
    public Tree234() {
        root = new Node(true);
    }
    
    // Check if node is full
    private boolean isFull(Node node) {
        return node.numKeys == 3;
    }
    
    // Search for a key
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
    
    // Split a full node
    private SplitResult splitNode(Node node) {
        Node newNode = new Node(node.isLeaf);
        
        int middleKey = node.keys[1];
        
        // Right key goes to new node
        newNode.keys[0] = node.keys[2];
        newNode.numKeys = 1;
        
        // Left key stays in original node
        node.numKeys = 1;
        
        // Move children if not leaf
        if (!node.isLeaf) {
            newNode.children[0] = node.children[2];
            newNode.children[1] = node.children[3];
            node.children[2] = null;
            node.children[3] = null;
        }
        
        return new SplitResult(node, middleKey, newNode);
    }
    
    // Insert into a non-full node
    private void insertNonFull(Node node, int key) {
        int i = node.numKeys - 1;
        
        if (node.isLeaf) {
            while (i >= 0 && key < node.keys[i]) {
                node.keys[i + 1] = node.keys[i];
                i--;
            }
            node.keys[i + 1] = key;
            node.numKeys++;
        } else {
            while (i >= 0 && key < node.keys[i])
                i--;
            i++;
            
            if (isFull(node.children[i])) {
                SplitResult split = splitNode(node.children[i]);
                
                // Insert middle key into parent
                for (int j = node.numKeys; j > i; j--) {
                    node.keys[j] = node.keys[j - 1];
                    node.children[j + 1] = node.children[j];
                }
                node.keys[i] = split.middleKey;
                node.children[i] = split.left;
                node.children[i + 1] = split.right;
                node.numKeys++;
                
                if (key > node.keys[i])
                    i++;
            }
            
            insertNonFull(node.children[i], key);
        }
    }
    
    // Insert a key
    public void insert(int key) {
        if (isFull(root)) {
            Node newRoot = new Node(false);
            SplitResult split = splitNode(root);
            
            newRoot.keys[0] = split.middleKey;
            newRoot.children[0] = split.left;
            newRoot.children[1] = split.right;
            newRoot.numKeys = 1;
            
            root = newRoot;
            insertNonFull(newRoot, key);
        } else {
            insertNonFull(root, key);
        }
    }
    
    // Find minimum key in subtree
    private int findMinimum(Node node) {
        while (!node.isLeaf)
            node = node.children[0];
        return node.keys[0];
    }
    
    // Find maximum key in subtree
    private int findMaximum(Node node) {
        while (!node.isLeaf)
            node = node.children[node.numKeys];
        return node.keys[node.numKeys - 1];
    }
    
    // Merge nodes
    private void merge(Node parent, int idx) {
        Node left = parent.children[idx];
        Node right = parent.children[idx + 1];
        
        left.keys[left.numKeys] = parent.keys[idx];
        left.numKeys++;
        
        for (int i = 0; i < right.numKeys; i++) {
            left.keys[left.numKeys] = right.keys[i];
            left.numKeys++;
        }
        
        if (!left.isLeaf) {
            for (int i = 0; i <= right.numKeys; i++)
                left.children[left.numKeys - right.numKeys + i] = right.children[i];
        }
        
        for (int i = idx; i < parent.numKeys - 1; i++) {
            parent.keys[i] = parent.keys[i + 1];
            parent.children[i + 1] = parent.children[i + 2];
        }
        parent.numKeys--;
    }
    
    // Borrow from left sibling
    private void borrowFromLeft(Node parent, int idx) {
        Node child = parent.children[idx];
        Node sibling = parent.children[idx - 1];
        
        for (int i = child.numKeys; i > 0; i--)
            child.keys[i] = child.keys[i - 1];
        
        if (!child.isLeaf) {
            for (int i = child.numKeys + 1; i > 0; i--)
                child.children[i] = child.children[i - 1];
        }
        
        child.keys[0] = parent.keys[idx - 1];
        parent.keys[idx - 1] = sibling.keys[sibling.numKeys - 1];
        
        if (!child.isLeaf) {
            child.children[0] = sibling.children[sibling.numKeys];
            sibling.children[sibling.numKeys] = null;
        }
        
        child.numKeys++;
        sibling.numKeys--;
    }
    
    // Borrow from right sibling
    private void borrowFromRight(Node parent, int idx) {
        Node child = parent.children[idx];
        Node sibling = parent.children[idx + 1];
        
        child.keys[child.numKeys] = parent.keys[idx];
        parent.keys[idx] = sibling.keys[0];
        
        if (!child.isLeaf)
            child.children[child.numKeys + 1] = sibling.children[0];
        
        for (int i = 0; i < sibling.numKeys - 1; i++)
            sibling.keys[i] = sibling.keys[i + 1];
        
        if (!sibling.isLeaf) {
            for (int i = 0; i < sibling.numKeys; i++)
                sibling.children[i] = sibling.children[i + 1];
        }
        
        child.numKeys++;
        sibling.numKeys--;
    }
    
    // Fill child
    private void fill(Node parent, int idx) {
        if (idx != 0 && parent.children[idx - 1].numKeys > 1)
            borrowFromLeft(parent, idx);
        else if (idx != parent.numKeys && parent.children[idx + 1].numKeys > 1)
            borrowFromRight(parent, idx);
        else if (idx != 0)
            merge(parent, idx - 1);
        else
            merge(parent, idx);
    }
    
    // Delete from leaf
    private void deleteFromLeaf(Node node, int idx) {
        for (int i = idx; i < node.numKeys - 1; i++)
            node.keys[i] = node.keys[i + 1];
        node.numKeys--;
    }
    
    // Delete from internal node
    private void deleteFromInternal(Node node, int idx) {
        int key = node.keys[idx];
        
        if (node.children[idx].numKeys > 1) {
            int pred = findMaximum(node.children[idx]);
            node.keys[idx] = pred;
            deleteHelper(node.children[idx], pred);
        } else if (node.children[idx + 1].numKeys > 1) {
            int succ = findMinimum(node.children[idx + 1]);
            node.keys[idx] = succ;
            deleteHelper(node.children[idx + 1], succ);
        } else {
            merge(node, idx);
            deleteHelper(node.children[idx], key);
        }
    }
    
    // Delete helper
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
    
    // Delete a key
    public void delete(int key) {
        if (root == null)
            return;
        
        deleteHelper(root, key);
        
        if (root.numKeys == 0) {
            if (root.isLeaf)
                root = null;
            else
                root = root.children[0];
        }
    }
    
    // Find minimum
    public int findMin() {
        if (root == null)
            return -1;
        return findMinimum(root);
    }
    
    // Find maximum
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
    
    // Print tree structure
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
        
        int[] values = {10, 20, 30, 15, 25, 5, 1, 35, 40, 50};
        for (int value : values) {
            tree.insert(value);
            System.out.println("\nAfter inserting " + value + ":");
            tree.printTree();
            System.out.print("In-order: ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        Node found = tree.search(15);
        System.out.println("\n" + (found != null ? "Found:" : "Not found:") + " 15");
        
        System.out.println("Min: " + tree.findMin() + ", Max: " + tree.findMax());
        
        tree.delete(20);
        System.out.println("\nAfter deleting 20:");
        tree.printTree();
        System.out.print("In-order: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}
