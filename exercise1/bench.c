#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#ifdef FIB
    #include "fib-heap.h"

    void validate(node *n) {
        if(n->parent != NULL) {
            if (n->parent->child != n) {
                node *this = n;
                int seenchild = 0;
                do {
                    if (this == n->parent->child) seenchild = 1;
                    this = this->right_sibling;
                } while (this != n);
                if (!seenchild) {
                    printf("PARENTS CHILD-POINTER IS WRONG");
                    exit(-1);
                }
            }
        }
        node *this = n;
        do {
            if (this->parent != n->parent) {
                printf("PARENTS ARE NOT EQUAL");
                exit(-1);
            }
            this = this->right_sibling;
        } while (this != n);
        if (n->right_sibling->left_sibling != n) {
            printf("RIGHT SIB IS WRONG");
            exit(-1);
        }
        if (n->left_sibling->right_sibling != n) {
            printf("LEFT SIB IS WRONG");
            exit(-1);
        }
        if (n->child != NULL) {
            if (n->child->parent != n) {
                printf("CHILD IS WRONG");
                exit(-1);
            }
        }
    }

    void visit_nodes(node *n, void (*fun)(node *n)) {
        if (n == NULL) return;
        node *current = n;
        do {
            (*fun)(current);
            visit_nodes(n->child, fun);
            current = current->right_sibling;
        } while (current != n);
    }

#else
    #include "bin-heap.h"
#endif


//=============================================================================
//  Test functions
//=============================================================================

void print_time(char *pre, struct rusage before, struct rusage after) {
    long utime = ((after.ru_utime.tv_sec - before.ru_utime.tv_sec) * 1000000) + after.ru_utime.tv_usec - before.ru_utime.tv_usec;
    long stime = ((after.ru_stime.tv_sec - before.ru_stime.tv_sec) * 1000000) + after.ru_stime.tv_usec - before.ru_stime.tv_usec;
    printf("%s%ld\n", pre, (utime + stime));
}

void delete_min_stress_test(int pow) {
    heap *h = make_heap();

    struct rusage before_ins;
    struct rusage after_ins;

    getrusage(RUSAGE_SELF, &before_ins);

    // Insertion
    int n = (1 << pow);
    for (int i = 0; i < n; i++) {
        insert(h, i);
        insert(h, -i);
        insert(h, n - i);
    }

    getrusage(RUSAGE_SELF, &after_ins);
    print_time("ins: ", before_ins, after_ins);

    struct rusage before_del;
    struct rusage after_del;

    getrusage(RUSAGE_SELF, &before_del);

    // Deletion
    int max = -1;
    int prev_size;
    while (h->size > 0) {
        prev_size = h->size;

        node *min = delete_min(h);

        max = min->key;
    }

    getrusage(RUSAGE_SELF, &after_del);
    print_time("del: ", before_del, after_del);

    free(h);
}

void decrease_key_stress_test(int pow) {
    heap *h = make_heap();

    int n = (1 << pow);

    // Make an array to contain all the nodes
    node **nodes = (node**) malloc(3 * n * sizeof(node*));

    // Insertion
    int count = 0;
    for (int i = 0; i < n; i++) {
        nodes[count++] = insert(h, i);
        nodes[count++] = insert(h, -i);
        nodes[count++] = insert(h, n - i);
    }

    insert(h, INT_MIN);
    delete_min(h);

    struct rusage before_dec;
    struct rusage after_dec;

    getrusage(RUSAGE_SELF, &before_dec);

    // Decrease
    for (int i = 0; i < (1<<pow); i++) {
        decrease_key(h, nodes[(i * 29) % (n * 3)], (i * 13) % 500);
    }

    getrusage(RUSAGE_SELF, &after_dec);
    print_time("dec: ", before_dec, after_dec);

    // Free Willy!
    free(nodes);
    free(h);
}

int main() {
    for (int pow = 15; pow < 25; pow++) {
        printf("%d\n", 1 << pow);
        delete_min_stress_test(pow);
        decrease_key_stress_test(pow);
    }
    return 0;
}
