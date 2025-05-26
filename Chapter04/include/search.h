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
      const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap);

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

bool GBFS(const WeightedGraphNode* start, const WeightedGraphNode* goal, GBFSMap& outMap);

// A* search
struct AStarScratch {
    const WeightedEdge* parentEdge = nullptr;
    float heuristic = 0.0f;
    bool inOpenSet = false;
    bool inClosedSet = false;
    float actualFromStart = 0.0f;
};

using AStarMap = std::unordered_map<const WeightedGraphNode*, AStarScratch>;

bool AStar(const WeightedGraphNode* start, const WeightedGraphNode* goal, AStarMap& outMap);

#endif