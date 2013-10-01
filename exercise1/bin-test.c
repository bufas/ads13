#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bin-heap.h"

#define TESTSIZE 10

int main() {
    heap *h = make_heap();
    node **nodes = malloc(TESTSIZE * sizeof(node *));
    for(int i = 0; i < TESTSIZE; i++) {
        nodes[i] = insert(h, rand() % 100);
    }

    while (h->size > 0) {
        if (rand() % 2) {
            printf("decrease_key");
            decrease_key(h, nodes[rand() % TESTSIZE], rand() % 15);
        } else {
            printf("delete_min");
            delete_min(h);
        }
    }

    free(nodes);
}
