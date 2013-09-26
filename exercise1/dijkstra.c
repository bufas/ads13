#include <stdlib.h>
#include <time.h>
#include "fib-heap.h"

#define INFINITY -1

typedef struct graphnode;
struct graphnode {
    int tentative_dist;
    int visited;
};

void dijkstra(int source, graphnode **nodes, int **edges, int testsize) {
    // 1. Assign all nodes a distance (zero for source, and INF for all other)
    nodes[source]->tentative_dist = 0;
    // 2. Mark all nodes as unvisited
    // 3. Set source as current
    int current = source;
    // 4. Create a set of all unvisited nodes
    heap *unvisited = make_heap();
    node **heap_nodes = malloc(testsize * sizeof(node *));
    for (int i = 0; i < testsize; i++) {
        node *n = insert(unvisited, nodes[i]->tentative_dist);
        heap_nodes[i] = n;
    }

    current = delete_min(unvisited);
    while (nodes[current]->tentative_dist != INFINITY) {
        // 5. For all unvisited neighbors of current, update their tentative distance
        for (int neighbor = 0; neighbor < testsize; neighbor++) {
            if (edges[current][neighbor] != 0) {
                // There is an edge to this neighbor
                int dist_from_current = nodes[current]->tentative_dist + edges[current][neighbor];
                if (nodes[neighbor]->tentative_dist > dist_from_current) {
                    // update the neighbor dist
                    nodes[neighbor]->tentative_dist = dist_from_current;
                    int delta_dist = nodes[neighbor]->tentative_dist - dist_from_current;
                    decrease_key(unvisited, heap_nodes[neighbor], delta_dist);
                }
            }
        }

        // 6. Mark current as visited
        nodes[current]->cisited = 1;

        if (unvisited->size == 0) return;

        current = delete_min(unvisited);
    }
}

int main() {
    // The number of nodes
    int testsize = 10;

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
            // Do stuff
            int dist = rand() % 10 > 0 ? 0 : rand() % 10 + 1;
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
    }

    // Elect source
    node *source = nodes[rand() % testsize];

    // Call Dijkstras algorithm
    dijkstra(source, nodes, edges);

    // Free memory
    for (int i = 0; i < testsize; i++) {
        free(edges[i]);
        free(nodes[i]);
    }
    free(edges);
    free(nodes);
}
