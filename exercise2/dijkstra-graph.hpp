#ifndef DIJKSTRA_GRAPH_HPP_
#define DIJKSTRA_GRAPH_HPP_

#include <limits>           // INT_MAX
#include <sstream>          // string streams
#include <string>
#include <vector>

using std::endl;
using std::string;
using std::vector;
using std::ostringstream;

const int INFINITY = INT_MAX;

struct GraphEdge {
    GraphEdge(int node, int dist) : node_(node), dist_(dist) {
    }

    int node_;
    int dist_;
};

struct GraphNode {
    GraphNode(int index, int tent = INFINITY, bool visited = false) : index_(index), tent_(tent), visited_(visited), edges_() {
    }

    ~GraphNode() {
    }

    void addNeighbour(int index, int dist) {
        edges_.push_back(GraphEdge(index, dist));
    }

    string dot() {
        ostringstream s;
        for (int i = 0; i < edges_.size(); i++) {
            GraphEdge e = edges_.at(i);
            s << index_ << " -- " << e.node_;
            s << " [label=\"" << e.dist_ << "\"]" << endl;
        }
        return s.str();
    }

    int index_;
    int tent_;
    bool visited_;
    vector<GraphEdge> edges_;
};

struct Graph {
    Graph(vector<GraphNode> nodes) : size_(nodes.size()), nodes_(nodes) {
    }

    Graph(int size) : size_(size), nodes_() {
        for (int i = 0; i < size_; i++) {
            nodes_.push_back(GraphNode(i));
        }
    }

    ~Graph() {
        nodes_.clear();
    }

    void addEdge(int i, int j, int dist) {
        nodes_.at(i).addNeighbour(j, dist);
        nodes_.at(j).addNeighbour(i, dist);
    }

    string dot() {
        ostringstream s;
        s << "strict graph {" << endl;
        s << "graph [remincross=true]" << endl;
        for (int i = 0; i < size_; i++) {
            s << nodes_.at(i).dot();
        }
        s << "}";
        return s.str();
    }

    const int size_;
    vector<GraphNode> nodes_;
};

#endif