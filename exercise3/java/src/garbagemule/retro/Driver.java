package garbagemule.retro;

public class Driver {
    static void forced_splits() {
        Tree t = new Tree();
        for (int i = 0; i < 8; i++) {
            t.insertInsert(0, i+1);
            t.print();
            System.out.println();
        }
    }
    static void sequential_insert() {
        Tree t = new Tree();
        int[] keys = {5, 14, 1, 13, 3, 9, 0, 6, 8, 4, 11, 10, 15, 2, 7, 12};
        for (int i = 0; i < keys.length; i++) {
            t.insertInsert(i, keys[i]);
            t.print();
            System.out.println();
        }
    }

    static void interleaved_insert() {
        Tree t = new Tree();
        int[] times = {0, 1,  2, 0,  2, 1, 5, 4, 1, 2, 7,  0,  1,  5, 1, 10};
        int[] keys  = {5, 14, 1, 13, 3, 9, 0, 6, 8, 4, 11, 10, 15, 2, 7, 12};
        for (int i = 0; i < keys.length; i++) {
            t.insertInsert(times[i], keys[i]);
            t.print();
            System.out.println();
        }
    }

    static void random_insert() {
        Tree t = new Tree();
        java.util.Random r = new java.util.Random();
        for (int i = 0; i < 20; i++) {
            t.insertInsert(r.nextInt(i+1), r.nextInt());
            t.print();
            System.out.println();
        }
    }

    static void simple_ranges() {
        Tree t = new Tree();
        t.insertInsert(0, 5);
        t.insertInsert(1, 6);
        t.insertInsert(2, 3);
        t.insertDelete(3, 6);
        t.insertDelete(4, 3);
        t.insertInsert(5, 1);
        t.insertInsert(6, 2);
        t.insertDelete(7, 2);
        t.insertDelete(8, 5);
        t.insertDelete(9, 1);
        t.print();
    }

    static void stair_ranges() {
        Tree t = new Tree();
        int n = 6;
        for (int i = 0; i < n; i++) {
            t.insertInsert(i, i);
        }
        for (int i = 0; i < n; i++) {
            t.insertDelete(n + i, n - i - 1);
        }
        t.print();
    }

    public static void main(String[] args) {
        //forced_splits();
        //sequential_insert();
        //interleaved_insert();
        //random_insert();
        //simple_ranges();
        stair_ranges();
    }
}
