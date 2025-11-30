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
    
    // Left rotation
    private void leftRotate(Node x) {
        Node y = x.right;
        x.right = y.left;
        
        if (y.left != NIL)
            y.left.parent = x;
        
        y.parent = x.parent;
        
        if (x.parent == NIL)
            root = y;
        else if (x == x.parent.left)
            x.parent.left = y;
        else
            x.parent.right = y;
        
        y.left = x;
        x.parent = y;
    }
    
    // Right rotation
    private void rightRotate(Node y) {
        Node x = y.left;
        y.left = x.right;
        
        if (x.right != NIL)
            x.right.parent = y;
        
        x.parent = y.parent;
        
        if (y.parent == NIL)
            root = x;
        else if (y == y.parent.left)
            y.parent.left = x;
        else
            y.parent.right = x;
        
        x.right = y;
        y.parent = x;
    }
    
    // Fix violations after insertion
    private void insertFixup(Node z) {
        while (z.parent.color == Color.RED) {
            if (z.parent == z.parent.parent.left) {
                Node y = z.parent.parent.right;

                if (y.color == Color.RED) {
                    z.parent.color = Color.BLACK;
                    y.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;

                    z = z.parent.parent;
                } else {
                    if (z == z.parent.right) {
                        z = z.parent;
                        leftRotate(z);
                    }

                    z.parent.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;

                    rightRotate(z.parent.parent);
                }
            } else {
                Node y = z.parent.parent.left;
                if (y.color == Color.RED) {
                    z.parent.color = Color.BLACK;
                    y.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;

                    z = z.parent.parent;
                } else {
                    if (z == z.parent.left) {
                        z = z.parent;

                        rightRotate(z);
                    }
                    z.parent.color = Color.BLACK;
                    z.parent.parent.color = Color.RED;

                    leftRotate(z.parent.parent);
                }
            }
        }
        root.color = Color.BLACK;
    }
    
    // Insert a node
    public void insert(int data) {
        Node z = new Node(data);
        z.left = NIL;
        z.right = NIL;
        
        Node y = NIL;
        Node x = root;
        
        while (x != NIL) {
            y = x;
            if (z.data < x.data)
                x = x.left;
            else
                x = x.right;
        }
        
        z.parent = y;
        
        if (y == NIL)
            root = z;
        else if (z.data < y.data)
            y.left = z;
        else
            y.right = z;
        
        insertFixup(z);
    }
    
    // Transplant utility for deletion
    private void transplant(Node u, Node v) {
        if (u.parent == NIL)
            root = v;
        else if (u == u.parent.left)
            u.parent.left = v;
        else
            u.parent.right = v;

        v.parent = u.parent;
    }
    
    // Find minimum in subtree
    private Node minimum(Node node) {
        while (node.left != NIL)
            node = node.left;

        return node;
    }
    
    // Find maximum in subtree
    private Node maximum(Node node) {
        while (node.right != NIL)
            node = node.right;

        return node;
    }
    
    // Fix violations after deletion
    private void deleteFixup(Node x) {
        while (x != root && x.color == Color.BLACK) {
            if (x == x.parent.left) {
                Node w = x.parent.right;
                if (w.color == Color.RED) {
                    w.color = Color.BLACK;
                    x.parent.color = Color.RED;

                    leftRotate(x.parent);
                    
                    w = x.parent.right;
                }
                if (w.left.color == Color.BLACK && w.right.color == Color.BLACK) {
                    w.color = Color.RED;
                    x = x.parent;
                } else {
                    if (w.right.color == Color.BLACK) {
                        w.left.color = Color.BLACK;
                        w.color = Color.RED;
                        rightRotate(w);
                        w = x.parent.right;
                    }
                    w.color = x.parent.color;
                    x.parent.color = Color.BLACK;
                    w.right.color = Color.BLACK;
                    leftRotate(x.parent);
                    x = root;
                }
            } else {
                Node w = x.parent.left;
                if (w.color == Color.RED) {
                    w.color = Color.BLACK;
                    x.parent.color = Color.RED;

                    rightRotate(x.parent);

                    w = x.parent.left;
                }
                if (w.right.color == Color.BLACK && w.left.color == Color.BLACK) {
                    w.color = Color.RED;
                    x = x.parent;
                } else {
                    if (w.left.color == Color.BLACK) {
                        w.right.color = Color.BLACK;
                        w.color = Color.RED;

                        leftRotate(w);

                        w = x.parent.left;
                    }
                    w.color = x.parent.color;
                    x.parent.color = Color.BLACK;
                    w.left.color = Color.BLACK;

                    rightRotate(x.parent);

                    x = root;
                }
            }
        }
        x.color = Color.BLACK;
    }
    
    // Delete a node
    public void delete(int data) {
        Node z = search(data);
        if (z == null) return;
        
        Node y = z;
        Node x;
        Color yOriginalColor = y.color;
        
        if (z.left == NIL) {
            x = z.right;
            transplant(z, z.right);
        } else if (z.right == NIL) {
            x = z.left;
            transplant(z, z.left);
        } else {
            y = minimum(z.right);
            yOriginalColor = y.color;
            x = y.right;
            
            if (y.parent == z) {
                x.parent = y;
            } else {
                transplant(y, y.right);

                y.right = z.right;
                y.right.parent = y;
            }
            
            transplant(z, y);

            y.left = z.left;
            y.left.parent = y;
            y.color = z.color;
        }
        
        if (yOriginalColor == Color.BLACK)
            deleteFixup(x);
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
    public Node successor(Node x) {
        if (x.right != NIL)
            return minimum(x.right);
        
        Node y = x.parent;
        while (y != NIL && x == y.right) {
            x = y;
            y = y.parent;
        }

        return (y != NIL) ? y : null;
    }
    
    // Find predecessor
    public Node predecessor(Node x) {
        if (x.left != NIL)
            return maximum(x.left);
        
        Node y = x.parent;
        while (y != NIL && x == y.left) {
            x = y;
            y = y.parent;
        }

        return (y != NIL) ? y : null;
    }
    
    // Find tree minimum
    public Node findMin() {
        if (root == NIL)
            return null;

        return minimum(root);
    }
    
    // Find tree maximum
    public Node findMax() {
        if (root == NIL)
            return null;

        return maximum(root);
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
        
        // Insert elements
        int[] values = {10, 20, 30, 15, 25, 5, 1};
        for (int value : values) {
            tree.insert(value);
            System.out.print("After inserting " + value + ": ");
            tree.inorder(tree.getRoot());
            System.out.println();
        }
        
        // Search
        Node found = tree.search(15);
        if (found != null)
            System.out.println("\nFound: " + found.data);
        
        // Find min and max
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
        
        // Delete
        tree.delete(20);
        System.out.print("\nAfter deleting 20: ");
        tree.inorder(tree.getRoot());
        System.out.println();
    }
}