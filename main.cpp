/*
 * INSTRUCTION:
 *     This is a C++ starting code for Homework Group1.
 *     When you finish the development, download this file and submit to Canvas
 *     according to the submission instructions.
 *
 *     Please DO NOT change the name of the main procecure "main()"
 */

// Finish the head comment with Abstract, Name, and Date.
/*
 * Abstract: This program simulates a Smart Route Planner that determines the most efficient path 
 *            between two locations using Dijkstra’s shortest path algorithm. It models a real-world 
 *            transportation network as a weighted graph, where nodes represent destinations and edges 
 *            represent travel distances or costs. The implementation demonstrates the use of adjacency 
 *            lists, priority queues, and graph traversal to compute and display the fastest route 
 *            along with the total travel cost.
 /*
 * AI Tools Utilized: ChatGPT was used to assist in refining the problem concept, 
 * identifying the real-world application of Dijkstra’s algorithm in route optimization, and improving 
 * the structure and documentation of the C++ implementation. Prompts focused on clarifying algorithm 
 * logic, verifying graph traversal behavior, and ensuring the program met formatting and autograder 
 * requirements. While ChatGPT provided guidance and sample phrasing, all logic and testing were 
 * independently implemented, verified, and debugged by the developer.
 */
/*
 * Name: Kayla Perez and Mathew Johann
 * Date: 10/05/2025
 */

#include <bits/stdc++.h>
using namespace std;

// ---------- Graph Structures ----------

// Represents a single directed edge from one vertex to another with a given weight.
struct Edge {
    string v;   // destination vertex
    double w;   // weight or cost of the edge
};

// Graph class uses an adjacency list to store all vertices and edges.
class Graph {
private:
    unordered_map<string, vector<Edge>> adj;  // adjacency list representation

public:
    // Adds a new vertex to the graph if it doesn’t already exist.
    void addNode(const string& u) {
        if (!adj.count(u)) adj[u] = {};
    }

    // Adds a directed edge (u -> v) with weight w to the graph.
    // Dijkstra’s algorithm requires non-negative edge weights.
    void addEdge(const string& u, const string& v, double w) {
        if (w < 0)
            throw invalid_argument("Edge weights must be non-negative for Dijkstra.");
        addNode(u);
        addNode(v);
        adj[u].push_back({v, w});
    }

    // Returns all neighbors (adjacent vertices) of a given vertex u.
    const vector<Edge>& neighbors(const string& u) const {
        static const vector<Edge> empty;
        auto it = adj.find(u);
        return (it != adj.end()) ? it->second : empty;
    }

    // Returns a list of all vertices currently stored in the graph.
    vector<string> nodes() const {
        vector<string> keys;
        for (const auto& kv : adj) keys.push_back(kv.first);
        return keys;
    }
};

// ---------- Dijkstra’s Algorithm ----------

// Stores shortest distance and parent (previous node) for each vertex.
struct DijkstraResult {
    unordered_map<string, double> dist;   // shortest distance from source
    unordered_map<string, string> parent; // path reconstruction data
};

// Dijkstra’s algorithm: computes shortest paths from a single source vertex to all others.
DijkstraResult dijkstra(const Graph& g, const string& src) {
    DijkstraResult res;

    // Initialize all distances to infinity
    for (const auto& n : g.nodes()) 
        res.dist[n] = numeric_limits<double>::infinity();
    res.dist[src] = 0.0; // source has distance 0

    // Min-priority queue storing (distance, vertex)
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    pq.push({0.0, src});
    unordered_set<string> visited;

    // Main loop: extract the vertex with the smallest tentative distance
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (visited.count(u)) continue; // skip if already processed
        visited.insert(u);

        // Relax edges (update neighbors if shorter path found)
        for (auto& e : g.neighbors(u)) {
            double nd = d + e.w;
            if (nd < res.dist[e.v]) {
                res.dist[e.v] = nd;
                res.parent[e.v] = u;
                pq.push({nd, e.v});
            }
        }
    }
    return res;
}

// ---------- Path Reconstruction ----------

// Builds the shortest path sequence from source to target using the parent map.
vector<string> reconstructPath(const unordered_map<string, string>& parent, const string& src, const string& tgt) {
    vector<string> path;
    string cur = tgt;

    // Follow parent links backward from target to source
    while (cur != src && parent.count(cur)) {
        path.push_back(cur);
        cur = parent.at(cur);
    }

    // If source was never reached, the target is unreachable
    if (cur != src) return {};
    
    // Add source and reverse the path for correct order
    path.push_back(src);
    reverse(path.begin(), path.end());
    return path;
}

// ---------- Output Formatting ----------

// Displays the loaded graph, computes shortest path, and prints the results.
void showRoute(const Graph& g, const string& src, const string& tgt, int nodes, int edges) {
    cout << "Graph loaded with " << nodes << " nodes and " << edges << " edges.\n";
    cout << "Source: " << src << ", Target: " << tgt << "\n";

    // Run Dijkstra’s algorithm
    DijkstraResult r = dijkstra(g, src);

    // Check if target is reachable
    if (!r.dist.count(tgt) || isinf(r.dist[tgt])) {
        cout << "No route from " << src << " to " << tgt << ".";
        return;
    }

    // Reconstruct and print the path
    auto path = reconstructPath(r.parent, src, tgt);
    cout << "Fastest route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << "->"; // arrow between nodes
    }
    // cout << "\nTotal cost: " << fixed << setprecision(2) << r.dist[tgt];
    // Mathew: An endline is added because it makes the code print cleaner.
    cout << "\nTotal cost: " << fixed << setprecision(2) << r.dist[tgt] << endl;
}

// ---------- Main ----------

// Entry point: reads input, builds graph, and displays the computed route.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int nodes, edges;

    // Read number of nodes and edges
    if (!(cin >> nodes >> edges)) {
        cerr << "Invalid input format.\n";
        return 1;
    }

    Graph g;

    // Read all edges and construct the graph
    for (int i = 0; i < edges; ++i) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        g.addEdge(to_string(u), to_string(v), w);
    }

    // Read source and target vertices
    string src, tgt;
    cin >> src >> tgt;

    // Display the computed route
    showRoute(g, src, tgt, nodes, edges);
    return 0;
}
