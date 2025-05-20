#ifndef SEARCH_H
#define SEARCH_H

#include <queue>
#include <unordered_map>
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

using NodeToParentMap = std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(
      const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap) {
    // Whether we found a path
    bool pathFound = false;

    // Nodes to consider
    std::queue<const GraphNode*> q;

    // Enqueue the first node
    q.emplace(start);
    while(!q.empty()) {
        // Dequeue a node
        const GraphNode* current = q.front();
        q.pop();
        if(current == goal) {
            pathFound = true;
            break;
        }

        // Enqueue adjacent nodes that aren't already on map
        for(const GraphNode* node: current->adjacent) {
            // If the parent is null, it hasn't been enqueued
            // (except for the start node)
            const GraphNode* parent = outMap[node];
            if(parent == nullptr && node != start) {
                // Enqueue this node, setting its parent
                outMap[node] = current;
                q.emplace(node);
            }
        }
    }

    return pathFound;
}

#endif