#ifndef SEARCH_H
#define SEARCH_H

#include <algorithm>
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

struct WeightedGraph {
    std::vector<WeightedGraphNode*> nodes;
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

// GBFS search
float computeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b) {
    return 0.0f;
}

struct GBFSScratch {
    const WeightedEdge* parentEdge = nullptr;
    float heuristic = 0.0f;
    bool inOpenSet = false;
    bool inClosedSet = false;
};

using GBFSMap = std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

bool GBFS(const WeightedGraphNode* start, const WeightedGraphNode* goal, GBFSMap& outMap) {
    std::vector<const WeightedGraphNode*> openSet;

    // Set current node to start, and mark in closed set
    const WeightedGraphNode* current = start;
    outMap[current].inClosedSet = true;

    do {
        // Add adjacent nodes to open set
        for(const WeightedEdge* edge: current->edges) {
            // Get scratch data for this node
            GBFSScratch& data = outMap[edge->to];

            // Add it only if it's not in closed set
            if(!data.inClosedSet) {
                // Set the adjacent node's parent edge
                data.parentEdge = edge;
                if(!data.inOpenSet) {
                    // Compute the heuristic for this node, and add to open set
                    data.heuristic = computeHeuristic(edge->to, goal);
                    data.inOpenSet = true;
                    openSet.emplace_back(edge->to);
                }
            }
        }

        if(openSet.empty()) {
            break; // Break out of outer loop
        }

        // Find lowest cost node in open set
        auto iter = std::min_element(openSet.begin(),
              openSet.end(),
              [&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
                  return outMap[a].heuristic < outMap[b].heuristic;
              });

        // Set to current and move from open to closed
        current = *iter;
        openSet.erase(iter);
        outMap[current].inOpenSet = false;
        outMap[current].inClosedSet = true;
    } while(current != goal);

    return (current == goal) ? true : false;
}

#endif