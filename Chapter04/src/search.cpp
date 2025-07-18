#include "search.h"

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

bool AStar(const WeightedGraphNode* start, const WeightedGraphNode* goal, AStarMap& outMap) {
    std::vector<const WeightedGraphNode*> openSet;

    // Set current node to start and mark in closed set
    const WeightedGraphNode* current = start;
    outMap[current].inClosedSet = true;

    do {
        // Add adjacent nodes to open set
        for(const WeightedEdge* edge: current->edges) {
            const WeightedGraphNode* neighbor = edge->to;

            // Get a scratch data for this node
            AStarScratch& data = outMap[neighbor];

            // Only check nodes that aren't in the closed set
            if(!data.inClosedSet) {
                if(!data.inOpenSet) {
                    // Not in the open set, so parent must by current
                    data.parentEdge = edge;
                    data.heuristic = computeHeuristic(neighbor, goal);

                    // Actual cost is the parent's plus cost of traversing edge
                    data.actualFromStart = outMap[current].actualFromStart + edge->weight;
                    data.inClosedSet = true;
                    openSet.emplace_back(neighbor);
                } else {
                    // Compute what new actual cost is if current becomes parent
                    float newG = outMap[current].actualFromStart + edge->weight;
                    if(newG < data.actualFromStart) {
                        // Current should adopt this node
                        data.parentEdge = edge;
                        data.actualFromStart = newG;
                    }
                }
            }
        }

        if(openSet.empty()) {
            break;
        }

        // Find lowest cost node in open set
        auto iter = std::min_element(openSet.begin(),
              openSet.end(),
              [&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
                  // Calculate f(x) for nodes a/b
                  float fOfA = outMap[a].heuristic + outMap[a].actualFromStart;
                  float fOfB = outMap[b].heuristic + outMap[b].actualFromStart;
                  return fOfA < fOfB;
              });

        // Set to current and move from open to closed
        current = *iter;
        openSet.erase(iter);
        outMap[current].inOpenSet = false;
        outMap[current].inClosedSet = true;
    } while(current != goal);

    return (current == goal) ? true : false;
}