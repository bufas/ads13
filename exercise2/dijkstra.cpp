#include <limits>           // INT_MAX
#include <cstdlib>          // strcmp
#include <ctime>            // clock
#include <sstream>          // string streams
#include <fstream>          // file streams
#include <string>
#include <vector>
#include "dijkstra-graph.hpp"
#include "dijkstra-graph-gen.hpp"
#include "heap.hpp"
#include "node.hpp"
#include "bin-heap.cpp"
#include "fib-heap.cpp"
#include "veb-heap.cpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::ostringstream;

const int MAX_DIST = 9;
bool verbose;

std::ostream& operator<<(std::ostream &out, const GraphNode &n) {
    //string s("GraphNode[");
    //s += "]";
    //return out << s;
    // Fucking C++ >_>
    if (n.tent_ == INFINITY) {
        return out << "GraphNode[index: " << n.index_ << ", tent: ∞, visited: " << n.visited_ << "]";
    } else {
        return out << "GraphNode[index: " << n.index_ << ", tent: " << n.tent_ << ", visited: " << n.visited_ << "]";
    }
}

std::ostream& operator<<(std::ostream &out, const Graph &g) {
    ostringstream s;
    s << "Graph (size: " << g.size_ << ")";
    for (int i = 0; i < g.nodes_.size(); i++) {
        GraphNode n = g.nodes_.at(i);
        s << endl << "  " << n.index_ << "[tent: " << n.tent_ << ", edges: ";
        for (int j = 0; j < n.edges_.size(); j++) {
            GraphEdge e = n.edges_.at(j);
            s <<  "(" << e.node_ << "," << e.dist_ << ") ";
        }
        s << "]";
    }
    return out << s.str();
}

/**
 * Dijkstra's Algorithm
 */
void dijkstra(Graph *g, int source, Heap<GraphNode*> *h) {
    // The ns array keeps track of all the heap-node objects that
    // are returned upon insertion into the given heap. The array
    // serves as a means of accessing neighbours for DECREASE KEY
    // operations.
    Node<GraphNode*> *ns[g->size_];

    // Source distance is 0, insert into queue to start off
    g->nodes_[source].tent_ = 0;
    ns[0] = h->insert(0, &(g->nodes_[source]));

    // The main loop
    while (h->size() > 0) {
        // The FIND MIN operation gives us the next node
        Node<GraphNode*> *hu = h->findMin();
        GraphNode *u = hu->getValue();

        // Mark it as visited
        u->visited_ = true;

        // Consider all neighbours
        for (int i = 0; i < u->edges_.size(); i++) {
            // Grab the graph node v
            GraphEdge e = u->edges_[i];
            GraphNode *v = &(g->nodes_[e.node_]);

            // Calculate the alternative distance from u to v
            int alt = u->tent_ + e.dist_;

            // If the alternative distance is shorter, save it!
            if (alt < v->tent_) {
                v->tent_ = alt;

                // If v has been visited, DECREASE KEY, otherwise, INSERT
                if (v->visited_) {
                    Node<GraphNode*> *hv = ns[v->index_];
                    int delta = hv->getKey() - alt;
                    h->decreaseKey(hv, delta);
                } else {
                    ns[v->index_] = h->insert(v->tent_, v);
                }
            }
        }

        // Finally, delete u from the queue
        h->deleteMin();
    }
}

/**
 * Auxiliary function for command line argument parsing
 */
const char* getOption(int argc, const char *argv[], string fs, string fl) {
    for (int i = 0; i < (argc - 1); i++) {
        if (strcmp(argv[i], fs.c_str()) == 0 || strcmp(argv[i], fl.c_str()) == 0) {
            return argv[i+1];
        }
    }
    return NULL;
}

/**
 * Auxiliary function for command line argument parsing
 */
const bool hasOption(int argc, const char *argv[], string fl) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], fl.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

Graph getGraph(int argc, const char *argv[]) {
    // File flag, e.g. --file dijkstra.graph
    const char *opt_file = getOption(argc, argv, "-f", "--file");

    if (opt_file != NULL) {
        if (verbose) cout << "Loading graph from file " << opt_file << "... " << endl;

        // Load the graph
        clock_t begin = clock();
        Graph g = deserialize(string(opt_file));
        clock_t end = clock();
        if (verbose) cout << "Load time: " << double(end-begin)/CLOCKS_PER_SEC << endl;
        return g;
    } else {
        // Seed flag, e.g. --seed 50
        const char *opt_seed = getOption(argc, argv, "-s", "--seed");
        long seed = (opt_seed != NULL) ? atol(opt_seed) : time(0);
        srand(seed);

        // Size flag, e.g. --size 50
        const char *opt_size = getOption(argc, argv, "-n", "--size");
        int size = (opt_size != NULL) ? atoi(opt_size) : 10;

        // Density flag, e.g. --density 50
        const char *opt_density = getOption(argc, argv, "-d", "--density");
        long max_density = (((long) size * size) - (3l * size)) / 2l;
        long density;
        if (opt_density != NULL) {
            density = (max_density * atoi(opt_density)) / 100l;
        } else {
            density = size;
        }

        // Info!
        if (verbose) cout << "Generating graph; size: " << size << ", density: " << ((density * 100l) / max_density) << "%, seed: " << seed << endl;

        // Build the graph
        clock_t begin = clock();
        Graph g = buildRandomMatrix(size, density, MAX_DIST);
        clock_t end = clock();
        if (verbose) cout << "Build time: " << double(end-begin)/CLOCKS_PER_SEC << endl;

        // Output flag, e.g. --output bob.graph
        const char *opt_out = getOption(argc, argv, "-o", "--output");
        if (opt_out != NULL) {
            if (verbose) cout << "Saving graph to file " << opt_out << "..." << endl;
            serialize(g, string(opt_out));
        }
        return g;
    }
}

void showHelp() {
    cout << "The program runs in one of two modes:" << endl;
    cout << "  AUTO MODE: Generates a graph from a set of arguments." << endl;
    cout << "  FILE MODE: Loads a graph from a given graph file." << endl;
    cout << endl;

    cout << "AUTO MODE arguments:" << endl;
    cout << "  -n [int]    number of nodes in the graph, defaults to 10" << endl;
    cout << "  -d [int]    graph density (in whole percentages), defaults to -n" << endl;
    cout << "  -s [int]    seed for rand(), defaults to time(0)" << endl;
    cout << "  -o [file]   file to write to, defaults to no output file" << endl;
    cout << endl;

    cout << "FILE MODE arguments:" << endl;
    cout << "  -f [file]   file to parse a graph from" << endl;
    cout << endl;

    cout << "Common arguments for both modes:" << endl;
    cout << "  --impl [bin|fib|veb] (defaults to bin)" << endl;
    cout << "  --dot       create a graph.dot file of the graph" << endl;
    cout << "  --print     print the graph to stdout after running Dijkstra" << endl;
    cout << endl;

    cout << "Note: FILE MODE is chosen if using -f, otherwise defaults to AUTO MODE" << endl;
}

int main(int argc, const char *argv[]) {
    // Show help
    if (hasOption(argc, argv, "--help") ||
        hasOption(argc, argv, "-h")     ||
        hasOption(argc, argv, "?")) {
        showHelp();
        return 0;
    }

    // Verbose flag, --verbose
    verbose = hasOption(argc, argv, "--verbose");

    // The graph object
    Graph g = getGraph(argc, argv);

    // Dot flag, --dot
    const bool dot = hasOption(argc, argv, "--dot");

    // Print flag, --print
    const bool print = hasOption(argc, argv, "--print");

    // Heap implementation
    const char *impl = getOption(argc, argv, "-impl", "--heap");
    Heap<GraphNode*> *h;
    if (impl != NULL && strcmp(impl, "fib") == 0) {
        h = new FibHeap<GraphNode*>;
        if (verbose) cout << "FibHeap" << endl;
    } else if (impl != NULL && strcmp(impl, "veb") == 0) {
        h = new VebHeap<GraphNode*>(g.size_ * 10);
        if (verbose) cout << "VebHeap" << endl;
    } else {
        h = new BinHeap<GraphNode*>;
        if (verbose) cout << "BinHeap" << endl;
    }

    // If we want it dotted
    if (dot) {
        ofstream file("graph.dot");
        file << g.dot();
        file.close();
    }

    // Create a heap and execute Dijkstra's Algorithm
    // Heap<GraphNode*> *h = new BinHeap<GraphNode*>;
    // Heap<GraphNode*> *h = new FibHeap<GraphNode*>;
    // Heap<GraphNode*> *h = new VebHeap<GraphNode*>(g.size_ * 10);
    clock_t begin = clock();
    dijkstra(&g, 0, h);
    clock_t end = clock();
    if (verbose) {
        cout << "Dijkstra time: " << double(end-begin)/CLOCKS_PER_SEC << endl; 
    } else {
        cout << double(end-begin) / CLOCKS_PER_SEC * 1000 << endl;
    }
    // If we want the end result printed
    if (print) {
        cout << g << endl;
    }

    // Clean up
    delete h;
    return 0;
}
