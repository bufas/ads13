#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <limits.h>
#ifdef FIB
    #include "fib-heap.h"
#else
    #include "bin-heap.h"
#endif

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

    while (unvisited->size > 0) {
        node *current = delete_min(unvisited);
        graphnode *this_node = (graphnode *) current->data;

        if (this_node->tentative_dist == INFINITY) {
            break;
        }

        for (int neighbor = 0; neighbor < graphsize; neighbor++) {
            graphnode *neighbor_node = (graphnode *) heap_nodes[neighbor]->data;
            int this_edge = edges[this_node->index][neighbor];
            if (this_edge < INFINITY) {
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
    }

    // Free memory
    free(heap_nodes);
}

void make_dot(int **matrix, int n, char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "graph graphternoondelight {\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "\t\"%d\"\n", i);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (i != j && matrix[i][j] < INFINITY) {
                fprintf(f, "\t\"%d\" -- \"%d\" [label=\"%d\"]\n", i, j, matrix[i][j]);
            }
        }
    }
    fprintf(f, "}\n");
    fclose(f);
}

/**
 * Build a random adjacency matrix by doing a random walk through
 * all the nodes of the assumed graph, generating random distances
 * on every visit. By never generating INFINITY, the random walk
 * will produce a fully connected graph without shortcuts.
 */
int** build_random_matrix(int n, long d) {
    // Allocate the memory for the adjacency matrix
    int** matrix = (int**) malloc(n * sizeof(int*));

    // Initialize all distances to INFINITY
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*) malloc(n * sizeof(int));

        for (int j = 0; j < i; j++) {
            matrix[i][j] = INFINITY;
            matrix[j][i] = INFINITY;
        }
    }

    // Except for the diagonal, which is all zeroes
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
    }

    // Generate an array of unvisited nodes
    int *unvisited = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        unvisited[i] = i;
    }

    // Do a random walk.
    int prev = 0;
    int m = n;
    for (int k = 0; k < n; k++) {
        // Pick a new node at random
        int index = rand() % m;
        int curr = unvisited[index];

        // Generate a random distance from previous node
        int dist = 1 + (rand() % 9);
        matrix[prev][curr] = (dist == 0) ? INFINITY : dist;
        matrix[curr][prev] = (dist == 0) ? INFINITY : dist;

        // Remember the current node
        prev = curr;

        // Shift the array backwards
        unvisited[index] = unvisited[m-1];
        m--;
    }

    // Add some random shortcuts
    for (int k = 0; k < d; k++) {
        int i = rand() % n;
        int j = rand() % n;
        if (i != j && matrix[i][j] == INFINITY) {
            int dist = 1 + (rand() % 9);
            matrix[i][j] = dist;
            matrix[j][i] = dist;
        } else {
            k--;
        }
    }
    return matrix;
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

char* get_flag_value(int argc, char **argv, char *flag_short, char *flag_long) {
    for (int i = 1; i < (argc - 1); i++) {
        if (strcmp(argv[i], flag_short) == 0 || strcmp(argv[i], flag_long) == 0) {
            return argv[i+1];
        }
    }
    return NULL;
}

int has_flag(int argc, char **argv, char *flag_short, char *flag_long) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag_short) == 0 || strcmp(argv[i], flag_long) == 0) {
            return 1;
        }
    }
    return 0;
}

int get_pct(char *dense) {
    for (int i = 0; dense[i] != '\0'; i++) {
        if (dense[i] == '%') {
            char pct[i];
            for (int j = 0; j < i; j++) {
                pct[j] = dense[j];
            }
            return atoi(pct);
        }
    }
    return -1;
}

void print_time(char *pre, struct rusage before, struct rusage after) {
    long utime = ((after.ru_utime.tv_sec - before.ru_utime.tv_sec) * 1000000) + after.ru_utime.tv_usec - before.ru_utime.tv_usec;
    long stime = ((after.ru_stime.tv_sec - before.ru_stime.tv_sec) * 1000000) + after.ru_stime.tv_usec - before.ru_stime.tv_usec;
    printf("%s%ld\n", pre, (utime + stime));
}

int main(int argc, char **argv) {
    // Seed flag, e.g. --seed 50
    char *seed = get_flag_value(argc, argv, "-s", "--seed");
    srand((seed != NULL) ? atoi(seed) : time(0));

    // Size flag, e.g. --size 50
    char *size = get_flag_value(argc, argv, "-n", "--size");
    int testsize = (size != NULL) ? atoi(size) : 100;

    // Density flag, e.g. --density 50
    char *dense = get_flag_value(argc, argv, "-d", "--density");
    long density = testsize;
    long max_density = (((long) testsize * testsize) - (3l * testsize)) / 2l;
    if (dense != NULL) {
        int pct = get_pct(dense);
        if (pct > 0) {
            density = (max_density * pct) / 100l;
        } else {
            density = atol(dense);
            if (density > max_density) {
                density = max_density;
            }
        }
    } else {
        density = testsize;
    }
    //printf("n: %d, d: %ld\n", testsize, density);

    // Print flag, e.g. --print
    int print = has_flag(argc, argv, "-p", "--print");

    // Set up the stopwatch, yo yo
    struct rusage before_gen;
    struct rusage after_gen;

    getrusage(RUSAGE_SELF, &before_gen);
    int **edges = build_random_matrix(testsize, density);

    // Create the nodes
    graphnode **nodes = malloc(testsize * sizeof(graphnode *));
    for (int i = 0; i < testsize; i++) {
        graphnode *node = malloc(sizeof(graphnode));
        node->tentative_dist = INFINITY;
        node->visited = 0;
        node->index = i;
        nodes[i] = node;
    }
    getrusage(RUSAGE_SELF, &after_gen);
    print_time("generation: ", before_gen, after_gen);

    // Elect source
    int source = rand() % testsize;

    // Print the configuration
    if (print) {
        printf("Source node: %d\n", source);
        printf("    ");
        for (int col = 0; col < testsize; col++) printf("%d ", col);
        printf("\n   ");
        for (int col = 0; col < testsize; col++) printf("--");
        printf("\n");
        for (int row = 0; row < testsize; row++) {
            printf("%d | ", row);
            for (int col = 0; col < testsize; col++) {
                if (edges[row][col] == INFINITY) {
                    printf("- ");
                } else {
                    printf("%d ", edges[row][col]);
                }
            }
            printf("\n");
        }
    }

    struct rusage before_dijk;
    struct rusage after_dijk;

    // Call Dijkstras algorithm
    getrusage(RUSAGE_SELF, &before_dijk);
    dijkstra(source, nodes, edges, testsize);
    getrusage(RUSAGE_SELF, &after_dijk);
    print_time("dijkstra: ", before_dijk, after_dijk);

    print_time("total: ", before_gen, after_dijk);

    // Print the result
    if (print) {
        for (int i = 0; i < testsize; i++) {
            printf("Node %d: %d meter\n", i, nodes[i]->tentative_dist);
        }
    }

    // Free memory
    for (int i = 0; i < testsize; i++) {
        free(edges[i]);
        free(nodes[i]);
    }
    free(edges);
    free(nodes);
}
