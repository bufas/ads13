#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "fib-heap-reroll.h"

#define INFINITY INT_MAX

typedef struct graphnode graphnode;
struct graphnode {
    int index;
    int tentative_dist;
    int visited;
};

void dijkstra(int source, graphnode **nodes, int **edges, int graphsize) {
    nodes[source]->tentative_dist = 0;

    heap *unvisited = make_heap();
    node **heap_nodes = malloc(graphsize * sizeof(node *));
    for (int i = 0; i < graphsize; i++) {
        node *n = insert(unvisited, nodes[i]->tentative_dist);
        n->data = nodes[i];
        heap_nodes[i] = n;
    }

    do {
        node *current = delete_min(unvisited);
        graphnode *this_node = (graphnode *) current->data;
        for (int neighbor = 0; neighbor < graphsize; neighbor++) {
            graphnode *neighbor_node = (graphnode *) heap_nodes[neighbor]->data;
            int this_edge = edges[this_node->index][neighbor];
            if (this_edge != 0) {
                // There is an edge to this neighbor
                int dist_from_current = current->key + this_edge;
                if (heap_nodes[neighbor]->key > dist_from_current) {
                    // update the neighbor dist
                    int delta_dist = heap_nodes[neighbor]->key - dist_from_current;
                    neighbor_node->tentative_dist = dist_from_current;
                    decrease_key(unvisited, heap_nodes[neighbor], delta_dist);
                }
            }
        }

        this_node->visited = 1;

    } while (unvisited->size > 1 && find_min(unvisited).key != INFINITY);

    // Free memory
    free(heap_nodes);
}

/**
 * Build an adjacency matrix as a two-dimensional int array from
 * the given size and distances array.
 */
int** build_matrix(int size, int *distances) {
    int **matrix = (int**) malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*) malloc(size * sizeof(int));

        for (int j = 0; j < size; j++) {
            // Flattened index
            int index = (i * size) + j;
            matrix[i][j] = distances[index];
        }
    }
    return matrix;
}

/**
 * Build a distances array from an argument array by converting
 * the argument strings to integers. Note that the args pointer
 * must point to what will become the top-left (0,0) entry in
 * what will be the resulting adjacency matrix.
 *
 * @param length the length of the argument array
 * @param args the 0-indexed argument array
 */
int* args_to_distances(int length, char **args) {
    int *distances = (int*) malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        if (strlen(args[i]) == 1 && args[i][0] == '-') {
            distances[i] = INFINITY;
        } else {
            distances[i] = atoi(args[i]);
        }
    }
    return distances;
}

int main() {
    // The number of nodes
    int testsize = 4;

    // Initialize the ajacency matrix
    int **edges = malloc(testsize * sizeof(int *));
    for (int nodes = 0; nodes < testsize; nodes++) {
        int *tmp = malloc(testsize * sizeof(int));
        for (int i = 0; i < testsize; i++) {
            tmp[i] = 0;
        }
        edges[nodes] = tmp;
    }

    // Fill the ajacency matrix
    for (int row = 0; row < testsize; row++) {
        for (int col = 0; col < row; col++) {
            int dist = rand() % 10;
            edges[row][col] = dist;
            edges[col][row] = dist;
        }
    }

    // Create the nodes
    graphnode **nodes = malloc(testsize * sizeof(graphnode *));
    for (int i = 0; i < testsize; i++) {
        graphnode *node = malloc(sizeof(graphnode));
        node->tentative_dist = INFINITY;
        node->visited = 0;
        node->index = i;
        nodes[i] = node;
    }

    // Elect source
    int source = rand() % testsize;

    // Print the configuration
    printf("Source node: %d\n", source);
    printf("    ");
    for (int col = 0; col < testsize; col++) printf("%d ", col);
    printf("\n   ");
    for (int col = 0; col < testsize; col++) printf("--");
    printf("\n");
    for (int row = 0; row < testsize; row++) {
        printf("%d | ", row);
        for (int col = 0; col < testsize; col++) {
            printf("%d ", edges[row][col]);
        }
        printf("\n");
    }

    // Call Dijkstras algorithm
    dijkstra(source, nodes, edges, testsize);

    // Print the result
    for (int i = 0; i < testsize; i++) {
        printf("Node %d: %d meter\n", i, nodes[i]->tentative_dist);
    }

    // Free memory
    for (int i = 0; i < testsize; i++) {
        free(edges[i]);
        free(nodes[i]);
    }
    free(edges);
    free(nodes);
}
