#include <stdio.h>
#include <stdlib.h>
#include "fib-heap.h"

void node_definition(node *n, FILE *f) {
    if (n->parent == NULL) {
        if (n->marked)
            fprintf(f, "\t\"%p\" [color=\"red\", label=\"%d\", shape=\"box\"]\n", n, n->key);
        else
            fprintf(f, "\t\"%p\" [label=\"%d\", shape=\"box\"]\n", n, n->key);
    } else {
        if (n->marked)
            fprintf(f, "\t\"%p\" [color=\"red\", label=\"%d\"]\n", n, n->key);
        else
            fprintf(f, "\t\"%p\" [label=\"%d\", shape=\"box\"]\n", n, n->key);
    }
}

void node_relations(node *n, FILE *f) {
    if (n->parent != NULL) fprintf(f, "\t\"%p\" -> \"%p\" [constraint=\"false\"];\n", n, n->parent);
    fprintf(f, "\t\"%p\" -> \"%p\" [color=\"blue\", constraint=\"false\"];\n", n, n->right_sibling);
    fprintf(f, "\t\"%p\" -> \"%p\" [color=\"blue\", constraint=\"false\"];\n", n, n->left_sibling);
    if (n->child != NULL) fprintf(f, "\t\"%p\" -> \"%p\" [color=\"green\"];\n", n, n->child);
}

void visit(node *n, FILE *f, void (*fun)(node *n, FILE *f)) {
    if (n == NULL) return;
    node *current = n;
    do {
        (*fun)(current, f);
        visit(current->child, f, fun);
        current = current->right_sibling;
    } while (current != n);
}

int other_visit(node *n, node **nodes, int index) {
    if (n == NULL) return index;
    node *current = n;
    do {
        nodes[index++] = current;
        index = other_visit(current->child, nodes, index);
        current = current->right_sibling;
    } while (current != n);
    return index;
}

void build_dot(heap *h, char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "digraph heapgraph {\n");
    visit(h->root, f, node_definition);
    fprintf(f, "\n");
    visit(h->root, f, node_relations);
    fprintf(f, "\troot [shape=point];\n");
    fprintf(f, "\troot -> \"%p\";\n", h->root);
    // Set the ranks
    node *narr[h->size];
    other_visit(h->root, narr, 0);
    int thisRank = 0;
    int keepGoing = 1;
    while (keepGoing) {
        keepGoing = 0;
        fprintf(f, "\t{rank=same; ");
        for (int i = 0; i < h->size; i++) {
            if (narr[i] != NULL) {
                // Calculate narr_rank
                int narrRank = 0;
                node *noko = narr[i];
                while (noko->parent != NULL) {
                    narrRank++;
                    noko=noko->parent;
                }
                if (narrRank == thisRank) {
                    keepGoing = 1;
                    fprintf(f, "\"%p\" ", narr[i]);
                    narr[i] = NULL;
                }
            }
        }
        fprintf(f, "}\n");
        thisRank++;
    }

    fprintf(f, "}\n");

    fclose(f);
}
