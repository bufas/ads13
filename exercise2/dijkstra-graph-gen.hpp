#ifndef DIJKSTRA_GRAPH_GEN_HPP_
#define DIJKSTRA_GRAPH_GEN_HPP_

#include <utility>          // std::pair
#include <algorithm>        // std::remove
#include <fstream>          // file streams
#include <iostream>
#include <string>
#include "dijkstra-graph.hpp"
#include "int-list.hpp"

using std::pair;

struct Available {
    Available(int size) {
        lists = new std::vector<IntList*>;
        nonEmpty = new IntList(-1, size);

        for (int i = 0; i < (size-1); i++) {
            IntList *list = new IntList(i, (size-1)-i);
            lists->push_back(list);
            nonEmpty->push(i);

            for (int j = (i+1); j < size; j++) {
                list->push(j);
            }
        }
        lists->push_back(new IntList((size - 1), 0));
    }

    ~Available() {
        delete lists;
        delete nonEmpty;
    }

    pair<int,int> nextRandom() {
        int rindex = rand() % nonEmpty->size();
        int index = nonEmpty->at(rindex);

        IntList *list = lists->at(index);
        int i = list->id();
        int j = list->removeAt(rand() % list->size());

        // If the list is now empty, "remove" it
        if (list->isEmpty()) {
            nonEmpty->removeValue(list->id());
        }
        return pair<int,int>(i,j);
    }

    void remove(int i, int j) {
        // Swap if necessary
        if (i > j) {
            int t = i;
            i = j;
            j = t;
        }

        // Grab the list of available edges
        IntList *list = lists->at(i);
        if (list->isEmpty()) {
            throw 20;
        }

        // Remove j from the list
        list->removeValue(j);

        // If the list is now empty, "remove" it
        if (list->isEmpty()) {
            nonEmpty->removeValue(list->id());
        }
    }

    std::vector<IntList*> *lists;
    IntList *nonEmpty;
};

/**
 * Auxiliary function for removing pairs from vectors
 */
void removeFrom(vector< pair<int,int> > *v, int p, int c) {
    using std::remove;
    v->erase(remove(v->begin(), v->end(), pair<int,int>(p,c)), v->end());
    v->erase(remove(v->begin(), v->end(), pair<int,int>(c,p)), v->end());
}

/**
 * Auxiliary function for connecting two edges
 */
void connectNodes(Graph *g, int i, int j, int dist) {
    g->nodes_.at(i).edges_.push_back(GraphEdge(j,dist));
    g->nodes_.at(j).edges_.push_back(GraphEdge(i,dist));
}

int getRandomDistance(int MAX_DIST) {
    return 1 + (rand() % MAX_DIST);
}

Graph buildRandomMatrix(int n, long d, int MAX_DIST) {
    int m = n - 1;
    int unvisited[m];
    for (int i = 0; i < m; i++) {
        unvisited[i] = i + 1;
    }

    Available edges(n);
    Graph graph(n);

    // Random walk
    int prev = 0;
    int dist;
    for (int i = 0; i < (n-1); i++) {
        int index = rand() % m;
        int curr  = unvisited[index];

        dist = getRandomDistance(MAX_DIST);
        graph.addEdge(prev, curr, dist);
        edges.remove(prev, curr);

        m--;
        unvisited[index] = unvisited[m];

        prev = curr;
    }

    // Connect back to 0
    dist = getRandomDistance(MAX_DIST);
    graph.addEdge(0, prev, dist);
    edges.remove(0, prev);

    // Add random shortcuts
    for (long l = 0; l < d; l++) {
        // Grab a random pair
        pair<int,int> p = edges.nextRandom();

        dist = getRandomDistance(MAX_DIST);
        graph.addEdge(p.first, p.second, dist);
        //edges.remove(pair[0], pair[1]);
    }

    return graph;
}

/**
 * Generation of random matrices
 */
Graph buildRandomMatrix2(int n, long d, int max_dist) {
    // Seed random
    srand(time(0));

    // Create an array of unvisited nodes
    int m = n - 1;
    int unvisited[m];
    for (int i = 0; i < m; ++i) {
        unvisited[i] = i + 1;
    }

    // Create a list of available edges
    vector< pair<int,int> > available;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            available.push_back(pair<int,int>(i,j)); 
        }
    }

    // Create the graph
    Graph g(n);

    // Do a random walk
    int prev = 0;
    for (int i = 0; i < n - 1; i++) {
        // Pick a new node at random
        int index = rand() % m;
        int curr = unvisited[index];

        // Generate a random distance from previous node
        int dist = 1 + (rand() % max_dist);
        connectNodes(&g, prev, curr, dist);

        // Remove the node pairs from the available edges list
        removeFrom(&available, prev, curr);

        // Remember the current node
        prev = curr;

        // Shift the array backwards
        unvisited[index] = unvisited[--m];
    }

    // Connect back to 0
    int dist = 1 + (rand() % max_dist);
    connectNodes(&g, prev, 0, dist);
    removeFrom(&available, prev, 0);

    // Add random shortcuts
    for (long k = 0; k < d; k++) {
        // Extract a random pair
        int index = rand() % available.size();
        pair<int,int> p = available.at(index);

        // Generate a random distance
        int dist = 1 + (rand() % max_dist);
        connectNodes(&g, p.first, p.second, dist);

        // Remove the pair from the list
        removeFrom(&available, p.first, p.second);
    }
    return g;
}

/*
void serialize(Graph g, string filename) {
    using namespace std;

    // Create the file stream
    ofstream file(filename.c_str());

    // Write header
    file << "# node_index: (neighbour_1,dist_1) (neighbour_2,dist_2) ..." << endl;

    // Consider each graph node
    for (int i = 0; i < g.nodes_.size(); i++) {
        GraphNode n = g.nodes_[i];

        // Write index and edges
        file << n.index_ << ": ";
        for (int j = 0; j < n.edges_.size(); j++) {
            GraphEdge e = n.edges_[j];

            // Write (node,dist) pair
            file << "(" << e.node_ << "," << e.dist_ << ") ";
        }
        file << endl;
    }
    file.close();
}
*/

void serialize(Graph g, string filename) {
    using namespace std;

    // Create the file stream
    ofstream file(filename.c_str());

    // Write header
    file << "# node_index neighbour_1,dist_1 neighbour_2,dist_2 ..." << endl;

    // Consider each graph node
    for (int i = 0; i < g.nodes_.size(); i++) {
        GraphNode n = g.nodes_[i];

        // Write index and edges
        file << n.index_ << " ";
        for (int j = 0; j < n.edges_.size(); j++) {
            GraphEdge e = n.edges_[j];

            // Write (node,dist) pair
            file << e.node_ << "," << e.dist_ << " ";
        }
        file << endl;
    }
    file.close();
}

Graph deserialize(string filename) {
    using namespace std;

    // Prepare the vector of GraphNodes
    vector<GraphNode> nodes;

    // Create the file stream
    ifstream file(filename.c_str());

    // Read line by line
    string line;

    // First line is the header
    getline(file, line);

    // Consider each line
    while (getline(file, line)) {
        // Grab the index
        int pos   = line.find(" ");
        string s  = line.substr(0, pos);
        int index = atoi(s.c_str());

        // Create the node u
        GraphNode u(index);

        // Consider all of the edges
        while (pos < line.length()) {
            // Find the next tokens
            int lparen = line.find(" ", pos);
            int comma  = line.find(",", lparen);
            int rparen = line.find(" ", comma);

            if (lparen == string::npos) break;

            // Extract neighbour v...
            s = line.substr(lparen + 1, comma - (lparen + 1));
            int v = atoi(s.c_str());

            // ... and the distance
            s = line.substr(comma + 1, rparen - (comma + 1));
            int d = atoi(s.c_str());

            // And push the edge
            u.edges_.push_back(GraphEdge(v,d));

            // Advance the position
            pos = rparen;
        }

        // Push the node on the list
        nodes.push_back(u);
    }

    // Return the resulting graph
    return Graph(nodes);
}

/*
Graph deserialize(string filename) {
    using namespace std;

    // Prepare the vector of GraphNodes
    vector<GraphNode> nodes;

    // Create the file stream
    ifstream file(filename.c_str());

    // Read line by line
    string line;

    // First line is the header
    getline(file, line);

    // Consider each line
    while (getline(file, line)) {
        // Grab the index
        int pos   = line.find(":");
        string s  = line.substr(0, pos);
        int index = atoi(s.c_str());

        // Create the node u
        GraphNode u(index);

        // Consider all of the edges
        while (pos < line.length()) {
            // Find the next tokens
            int lparen = line.find("(", pos);
            int comma  = line.find(",", lparen);
            int rparen = line.find(")", comma);

            if (lparen == string::npos) break;

            // Extract neighbour v...
            s = line.substr(lparen + 1, comma - (lparen + 1));
            int v = atoi(s.c_str());

            // ... and the distance
            s = line.substr(comma + 1, rparen - (comma + 1));
            int d = atoi(s.c_str());

            // And push the edge
            u.edges_.push_back(GraphEdge(v,d));

            // Advance the position
            pos = rparen;
        }

        // Push the node on the list
        nodes.push_back(u);
    }

    // Return the resulting graph
    return Graph(nodes);
}
*/

#endif