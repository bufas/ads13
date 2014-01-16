package garbagemule.retro;

import java.util.Map;
import java.util.TreeMap;

public class Tree {
    Node root;
    Map<Integer,Integer> current;

    public Tree() {
        root = null;
        current = new TreeMap<Integer, Integer>();
    }

    public void insertOperation(int t, Operation o, int key) {
        switch (o) {
            case INSERT: insertInsert(t, key); break;
            case DELETE: insertDelete(t, key); break;
        }
    }

    public void deleteOperation(int t) {

    }

    /**
     * Retroactively insert a non-retroactive insertion into the
     * data structure.
     *
     * @precondition the key can be inserted into the present,
     * because a pre-existing deletion would break consistency
     */
    void insertInsert(int t, int key) {
        assert t == 0 || (root != null);

        // No root means we just have to make one
        if (root == null) {
            root = new Node(t, key);
        }

        // If the root is a leaf, add a new parent node
        else if (root.child == null) {
            Node n0, n1;

            // Inserting before first node?
            if (t == 0) {
                // Update time in root
                root.time = 1;

                // Remap in current
                int k = root.tree.iterator().next();
                current.put(k, 1);

                n0 = new Node(t, key);
                n1 = root;
            } else {
                n0 = root;
                n1 = new Node(t, key);
            }
            root = new Node(n0, n1);
        }

        // Otherwise, the root is a parent, and the tree is consistent
        else {
            makeRoom(t);
            root.makeRoom(t);

            Node r = root.insertNode(t, new Node(t, key));
            if (r != null) {
                root = r;
            }
        }

        // Because we are not allowed to break consistency, we can
        // safely insert into the present.
        current.put(key, t);
    }

    /**
     * Retroactively insert a non-retroactive deletion into the
     * data structure.
     *
     * @precondition the key exists in the present, since deleting
     * a key that has not been inserted would break consistency
     */
    void insertDelete(int t, int key) {
        // Grab the start time so we know which segment to insert
        int start = current.remove(key);
        assert start < t;

        // Make room for the node
        root.makeRoom(t);

        // Insert it, then insert the segment
        Node r = root.insertNode(t, new Node(t, key));
        if (r != null) {
            root = r;
        }
        root.insertSegment(start, t, key);
    }

    /**
     * Retroactively delete a non-retroactive insertion into the
     * data structure.
     *
     * @precondition the key exists in the present, since deleting
     * an insert with an associated delete would break consistency
     */
    void deleteInsert(int t, int key) {

    }

    /**
     * Retroactively delete a non-retroactive deletion into the
     * data structure.
     *
     * @precondition the key can be re-inserted into the present,
     * since deleting a delete with no associated insert would
     * break consistency
     */
    void deleteDelete(int t, int key) {

    }

    /**
     * Make room for inserting a new element at the given time
     * @param t
     */
    void makeRoom(int t) {
        for (Map.Entry<Integer,Integer> entry : current.entrySet()) {
            if (entry.getValue() >= t) {
                entry.setValue(entry.getValue() + 1);
            }
        }
    }

    void print() {
        if (root != null) {
            root.print();
        }
    }
}
