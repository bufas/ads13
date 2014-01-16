package garbagemule.retro;

import java.util.Set;
import java.util.TreeSet;

public class Node {
    /**
     * In all nodes, this is the parent node, which is only
     * null for the root node
     */
    Node parent;
    /**
     * In a composite/parent, this array holds the child nodes
     */
    Node[] child;
    /**
     * The size of the sub-tree held in this node; always 1 for
     * leaves, and always 2 or 3 for composites/parents
     */
    int size;
    /**
     * The interval that this node represents
     */
    int start, end;
    /**
     * The time at which
     */
    int time;
    /**
     * The set of keys that exist in the time interval that this
     * node represents
     */
    Set<Integer> tree;

    /**
     * Create a leaf node with the given key
     */
    public Node(int t, int key) {
        time = t;
        size = 1;
        tree = new TreeSet<Integer>();
        tree.add(key);
    }

    /**
     * Create a parent node, which will act as the parent for
     * the given nodes.
     */
    public Node(Node n0, Node n1) {
        parent = null;
        child  = new Node[3];
        tree   = new TreeSet<Integer>();
        adopt(n0, n1);
    }

    private void adopt(Node n0, Node n1) {
        // Establish healthy relationship
        child[0] = n0;  n0.parent = this;
        child[1] = n1;  n1.parent = this;

        // Then establish range
        start = (n0.child == null) ? n0.time : n0.start;
        end   = (n1.child == null) ? n1.time : n1.end;

        // Deal with keys
        for (int key : n0.tree) {
            if (n1.tree.contains(key)) {
                tree.add(key);
            }
        }
        for (int key : tree) {
            n0.tree.remove(key);
            n1.tree.remove(key);
        }

        // Size is sum of sizes
        size = n0.size + n1.size;
    }

    /**
     * Shift around the leaves to make room for the given leaf,
     * and return the newly created sibling node, containing the
     * fallout and the third child.
     *
     * @precondition this node is a final parent
     * @precondition this node has three children
     */
    private Node insertLeafSplit(int t, Node leaf) {
        leaf.parent = this;

        Node fallout;
        if (t < child[0].time) {
            fallout  = child[2];
            child[2] = child[1];
            child[1] = child[0];
            child[0] = leaf;
        }
        else if (t < child[1].time) {
            fallout = child[2];
            child[2] = child[1];
            child[1] = leaf;
        }
        else if (t < child[2].time) {
            fallout = child[2];
            child[2] = leaf;
        }
        else {
            fallout = leaf;
        }
        Node sibling = new Node(child[2], fallout);
        child[2] = null;
        return sibling;
    }

    /**
     * Safely insert a leaf among its two siblings.
     *
     * @precondition this node has two children
     */
    private void insertLeafSafe(int t, Node leaf) {
        leaf.parent = this;

        if (t < child[0].time) {
            child[2] = child[1];
            child[1] = child[0];
            child[0] = leaf;
        }
        else if (t < child[1].time) {
            child[2] = child[1];
            child[1] = leaf;
        }
        else {
            child[2] = leaf;
        }
    }

    /**
     * Shift around the children to make room for the given node,
     * and return the newly created sibling node, containing the
     * fallout and the third child.
     *
     * @precondition this node is NOT a final parent
     * @precondition this node has three children
     */
    private Node insertAfterNodeSplit(Node left, Node right) {
        Node fallout;
        if (child[0] == left) {
            fallout  = child[2];
            child[2] = child[1];
            child[1] = right;
            right.parent = this;
        }
        else if (child[1] == left) {
            fallout  = child[2];
            child[2] = right;
        }
        else if (child[2] == left) {
            fallout = right;
        }
        else {
            System.out.println("STOP HAPPENIONG THIS!");
            fallout = null;
        }
        Node sibling = new Node(child[2], fallout);
        child[2] = null;
        return sibling;
    }

    /**
     * Safely insert a parent node among its two siblings.
     *
     * @precondition this node has two children
     */
    private void insertNodeAfterSafe(Node left, Node right) {
        right.parent = this;

        if (child[0] == left) {
            child[2] = child[1];
            child[1] = right;
            return;
        }
        if (child[1] == left) {
            child[2] = right;
            return;
        }
        System.out.println("Should never happeeeeeeeeen!!!");
    }

    public Node insertNodeAfter(Node left, Node right) {
        // No third child? Safe to insert
        if (child[2] == null) {
            insertNodeAfterSafe(left, right);
            fixToRoot();
            return null;
        }

        // Otherwise, insert the node and split
        Node sibling = insertAfterNodeSplit(left, right);
        fixMe(); sibling.fixMe();

        // No parent means new root
        if (parent == null) {
            return new Node(this, sibling);
        }

        // Otherwise, recursively insert into the parent
        return parent.insertNodeAfter(this, sibling);
    }

    public Node insertNode(int t, Node node) {
        // If we are not a final parent, we insert recursively
        if (child[0].child != null) {
            if (t < child[0].end) return child[0].insertNode(t, node);
            if (t < child[1].end) return child[1].insertNode(t, node);
            if (child[2] == null) return child[1].insertNode(t, node);
            return child[2].insertNode(t, node);
        }

        // No third child? Safe to insert
        if (child[2] == null) {
            insertLeafSafe(t, node);
            fixToRoot();
            return null;
        }

        // Otherwise, insert the leaf and split
        Node sibling = insertLeafSplit(t, node);
        fixMe(); sibling.fixMe();

        // If we have no parent, we return a new root
        if (parent == null) {
            return new Node(this, sibling);
        }

        // Otherwise, we recursively insert in the parent
        return parent.insertNodeAfter(this, sibling);
    }

    private void fixToRoot() {
        fixMe();
        if (parent != null) {
            parent.fixToRoot();
        }
    }

    private void fixMe() {
        // Final parents look at times
        if (child[0].child == null) {
            start = child[0].time;
            end   = (child[2] == null) ? child[1].time : child[2].time;
            size  = (child[2] == null) ? 2 : 3;
        }
        // Non-final parents look at ranges
        else {
            start = child[0].start;
            end   = (child[2] == null) ? child[1].end : child[2].end;
            size  = child[0].size + child[1].size + (child[2] == null ? 0 : child[2].size);
        }
    }

    /**
     * Insert the given segment
     */
    public void insertSegment(int start, int end, int key) {
        // Is this a leaf?
        if (child == null) {
            if (start <= time || time < end) {
                tree.add(key);
            }
            return;
        }
        /*
        child[0].insertSegment(start, end, key);
        child[1].insertSegment(start, end, key);
        if (child[2] != null) {
            child[2].insertSegment(start, end, key);
        }
         */
        if (start <= this.start && this.end <= end) {
            removeKey(key);
            tree.add(key);
        } else {
            // Overlap on the left?
            if (start <= this.start) {
                child[0].insertSegment(start, end, key);
                child[1].insertSegment(start, end, key);
            }
            // On the right?
            else if (this.end < end) {
                child[1].insertSegment(start, end, key);
                if (child[2] != null) {
                    child[2].insertSegment(start, end, key);
                }
            }
        }
/*
        // Does this nodes interval fit in the segment?
        if (start <= this.start && this.end <= end) {
            // Remove key from children, then add to node (order is important!)
            removeKey(key);
            tree.add(key);
        } else {
            // Overlap on the left?
            if (start <= this.start) {
                child[0].insertSegment(start, end, key);
                child[1].insertSegment(start, end, key);
            }
            // On the right?
            else if (this.end <= end) {
                child[1].insertSegment(start, end, key);
                if (child[2] != null) {
                    child[2].insertSegment(start, end, key);
                }
            }
        }
*/
    }

    private void removeKey(int key) {
        tree.remove(key);
        if (child != null) {
            child[0].removeKey(key);
            child[1].removeKey(key);
            if (child[2] != null) {
                child[2].removeKey(key);
            }
        }
    }

    /**
     * Make room for inserting at the given time
     */
    public void makeRoom(int t) {
        // Are we a leaf?
        if (child == null) {
            if (time >= t) {
                time++;
            }
            return;
        }

        child[0].makeRoom(t);
        child[1].makeRoom(t);
        if (child[2] != null) {
            child[2].makeRoom(t);
        }
/*
        // Are we a final parent?
        if (child[0].child == null) {
            start = child[0].time;
            end = (child[2] == null) ? child[1].time : child[2].time;
        } else {
            // Or in the middle of the tree?
            start = child[0].start;
            end = (child[2] == null) ? child[1].end : child[2].end;
        }
*/
    }

    /**
     * Take room for inserting at the given time
     */
    public void takeRoom(int t) {
        if (child != null) {
            child[0].takeRoom(t);
            child[1].takeRoom(t);
            if (child[2] != null) {
                child[2].takeRoom(t);
            }
        } else if (time >= t) {
            time--;
        }
    }

    public void print() {
        print_aux("");
    }

    private void print_aux(String indent) {
        if (child == null) {
            System.out.println(indent + "Leaf[t:" + time + "] " + tree);
            return;
        }
        System.out.println(indent + "Node[r={" + start + ".." + end + "} s=" + size + "] " + tree);
        child[0].print_aux(indent + "    ");
        child[1].print_aux(indent + "    ");
        if (child[2] != null) {
            child[2].print_aux(indent + "    ");
        }
    }
}
