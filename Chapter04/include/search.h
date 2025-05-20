#ifndef SEARCH_H
#define SEARCH_H

#include <vector>

struct GraphNode {
    // Each node has pointers to adjacent nodes
    std::vector<GraphNode*> adjacent;
};

struct Graph {
    // A graph contains nodes
    std::vector<GraphNode*> nodes;
};

struct WeightedEdge {
    // Which nodes are connected by this edge?
    struct WeightedGraphNode* from;
    struct WeightedGraphNode* to;

    // Weight of this edge
    float weight;
};

struct WeightedGraphNode {
    // Stores outgoing edges
    std::vector<WeightedEdge*> edges;
};

#endif