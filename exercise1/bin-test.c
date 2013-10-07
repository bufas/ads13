#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "bin-heap.h"

#define TESTSIZE 7

void draw_relations(node *n, FILE *f) {
    if (n->left_child != NULL)  fprintf(f, "\t\"%p\" -> \"%p\" [color=\"red\"]\n", n, n->left_child);
    if (n->right_child != NULL) fprintf(f, "\t\"%p\" -> \"%p\" [color=\"blue\"]\n", n, n->right_child);
    if (n->parent != NULL) fprintf(f, "\t\"%p\" -> \"%p\" [color=\"black\", constraint=\"false\"]\n", n, n->parent);
}

void define_node(node *n, FILE *f) {
    fprintf(f, "\t\"%p\" [label=\"%d\"]\n", n, n->key);
}

void make_dot(node **nodes, char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "digraph grafical {\n");
    fprintf(f, "\tordering=\"out\"\n");
    for (int i = 0; i < TESTSIZE; i++) {
        define_node(nodes[i], f);
    }

    for (int i = 0; i < TESTSIZE; i++) {
        draw_relations(nodes[i], f);
    }

    fprintf(f, "}\n");
    fclose(f);
}

int main() {
    //srand(time(0));
    srand(50);

    heap *h = make_heap();
    node **nodes = malloc(TESTSIZE * sizeof(node *));
    for(int i = 0; i < TESTSIZE; i++) {
        nodes[i] = insert(h, rand() % 100);
    }

    while (h->size > 0) {
        make_dot(nodes, "after_inserts.dot");
        getchar();

        if (rand() % 2) {
            printf("decrease_key\n");
            decrease_key(h, nodes[rand() % TESTSIZE], rand() % 100);
        } else {
            printf("delete_min\n");
            delete_min(h);
        }

        if (h->size > 0) 
            printf("heap size = %d and root key = %d\n", h->size, h->root->key);
    }

    free(nodes);
    return 1;
}
