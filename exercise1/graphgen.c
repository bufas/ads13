//graphgen -n # -d #  ->  n [0,0] [0,1] [0,2] ... [0,n-1] [1,0] [1,1] ... [1,n-1] ... [n-1,n-1] 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>

#define INFINITY INT_MAX

long one_percent;

void dot() {
    static long i = 0;
    if (++i % one_percent == 0l) {
        printf(".");
        fflush(stdout);
    }
}

int remove_value(int *a, int *n, int v) {
    for (int i = 0; i < (*n); i++) {
        if (a[i] == v) {
            a[i] = a[--(*n)];
            return 1;
        }
    }
    return -1;
}

int remove_random(int *a, int *n) {
    // Get a random index
    int i = rand() % (*n);

    // Extract the value from the array
    int r = a[i];

    // Overwrite the index with the last element
    a[i] = a[--(*n)];

    // Return the value
    return r;
}

void random_walk(int **matrix, int n, int max_dist, int available[], int *available_n) {
    // Create an array of all the unvisited vertices
    // int *unvisited = (int*) malloc(n * sizeof(int));
    int *unvisited = (int*) malloc(n * sizeof(int));
    // int unvisited[n];
    for (int i = 1; i < n; i++) {
        unvisited[i-1] = i;
    }

    // Do a random walk around the unvisited nodes
    int unvisited_n = (n-1);
    int i = 0;
    while (unvisited_n > 0) {
        // Grab a random vertex to visit
        int j = remove_random(unvisited, &unvisited_n);

        // Generate a random distance and insert it in the matrix
        int dist = 1 + (rand() % (max_dist - 1));
        matrix[i][j] = dist;
        matrix[j][i] = dist;

        // The coordinates denote two indices...
        int fst = (i * n) + j;
        int snd = (j * n) + i;
        if (fst < snd) {
            int tmp = fst;
            fst = snd;
            snd = tmp;
        }

        // Which can no longer be available, so remove them
        remove_value(available, available_n, fst);
        remove_value(available, available_n, snd);

        // Remember which node we jumped to
        i = j;
    }

    // Finally, connect back to 0
    int dist = 1 + (rand() % (max_dist - 1));
    matrix[i][0] = dist;
    matrix[0][i] = dist;
    remove_value(available, available_n, i * n);
    remove_value(available, available_n, i);

    // Free Willy!
    free(unvisited);
}

void densify(int **matrix, int n, int max_dist, int *available, int *available_n, long d) {
    one_percent = d / 80;
    if (one_percent == 0) {
        one_percent = 1;
    }

    for (long k = 0; k < d; k++) {
        // Grab a random index
        int fst = remove_random(available, available_n);

        // Convert to coordinates
        int i = (fst / n);
        int j = (fst % n);

        // Generate a random distance and insert it in the matrix
        int dist = 1 + (rand() % (max_dist - 1));
        matrix[i][j] = dist;
        matrix[j][i] = dist;

        // Convert back to the "inverse" index
        int snd = (i * n) + j;
        if (snd == fst) {
            snd = (j * n) + i;
        }

        // Remove this index from the array as well
        remove_value(available, available_n, snd);
        dot();
    }
    printf("\n");
}

int** build_random_matrix(int n, long d) {
    // The maximum possible distance is bound by n
    int max_dist = (2 * n);

    // Allocate memory for the actual adjacency matrix
    int **matrix = (int**) malloc(n * sizeof(int*));

    // Initialize all distances to INFINITY...
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            matrix[i][j] = INFINITY;
            //available[(i * n) + j] = (i * n) + j;
        }
    }

    // ... except for the diagonal entries, which are all zeroes
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 0;
        //available[(i * n) + i] = available[--available_n];
    }

    // Allocate memory for the 'available' entries, leave out the diagonal
    int available_n = n * (n - 1);
    int *available = (int*) malloc(available_n * sizeof(int));

    // Initialize the available entries, skip the diagonal
    for (int i = 0, k = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                available[k++] = (i * n) + j;
            }
        }
    }

    // printf("After diagonal - available_n: %d\n", available_n);
    // for (int i = 0; i < available_n; i++) printf("index: %d - (%d,%d)\n", available[i], (available[i] / n), (available[i] % n));
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         if (matrix[i][j] != INFINITY) {
    //             printf("%d ", matrix[i][j]);
    //         } else {
    //             printf("- ");
    //         }
    //     }
    //     printf("\n");
    // }

    // Do a random walk
    random_walk(matrix, n, max_dist, available, &available_n);

    // printf("After walk     - available_n: %d\n", available_n);
    // for (int i = 0; i < available_n; i++) printf("index: %d - (%d,%d)\n", available[i], (available[i] / n), (available[i] % n));
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         if (matrix[i][j] != INFINITY) {
    //             printf("%d ", matrix[i][j]);
    //         } else {
    //             printf("- ");
    //         }
    //     }
    //     printf("\n");
    // }

    // Fill in the blanks, depending on the density
    densify(matrix, n, max_dist, available, &available_n, d);

    // printf("After densify  - available_n: %d\n", available_n);
    // for (int i = 0; i < available_n; i++) printf("index: %d - (%d,%d)\n", available[i], (available[i] / n), (available[i] % n));
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         if (matrix[i][j] != INFINITY) {
    //             printf("%d ", matrix[i][j]);
    //         } else {
    //             printf("- ");
    //         }
    //     }
    //     printf("\n");
    // }
    return matrix;
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

int main(int argc, char **argv) {
    srand(time(0));

    // File
    char *filename = get_flag_value(argc, argv, "-f", "--file");
    if (filename != NULL) {
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Couldn't open file '%s'...\n", filename);
            return -1;
        }
        fclose(file);
    } else {
        printf("No filename found, using default file name 'graphgen.out'...\n");
        filename = "graphgen.out";
    }

    // Size flag, e.g. --size 50
    char *size = get_flag_value(argc, argv, "-n", "--size");
    int n = (size != NULL) ? atoi(size) : 100;

    // Density flag, e.g. --density 50
    char *dense = get_flag_value(argc, argv, "-d", "--density");
    long density = n;
    long max_density =  (((long) n * (n - 1l)) / 2l) - n;
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
        density = n;
    }
    printf("n: %d, d: %ld\n", n, density);
    int **matrix = build_random_matrix(n, density);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Couldn't open file '%s'...\n", filename);
        return -1;
    }
    printf("Writing to file '%s'...\n", filename);
    fprintf(file, "%d %ld ", n, density);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != INFINITY) {
                fprintf(file, "%d ", matrix[i][j]);
            } else {
                fprintf(file, "- ");
            }
        }
    }

    fclose(file);
    return 0;
}