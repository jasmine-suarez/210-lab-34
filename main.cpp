// COMSC-210 | Lab 34 | Jasmine Suarez
// IDE used: VS Code

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
using namespace std;

const int SIZE = 11;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    // Breadth-First Search starting from 'start'
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);
        cout << "BFS starting from vertex " << start << ":" << endl;
        // print nodes on the next line
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";

            // enqueue all unvisited neighbors
            for (auto &p : adjList[u]) {
                int v = p.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

    // Depth-First Search (iterative using a stack) starting from 'start'
    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        stack<int> st;

        st.push(start);

        cout << "DFS starting from vertex " << start << ":" << endl;
        // print nodes on the next line
        while (!st.empty()) {
            int u = st.top(); st.pop();

            if (visited[u])
                continue;

            visited[u] = true;
            cout << u << " ";

            // push neighbors onto stack in adjacency order so
            // the last neighbor is visited first
            for (auto &p : adjList[u]) {
                int v = p.first;
                if (!visited[v])
                    st.push(v);
            }
        }
        cout << endl;
    }

    // Return BFS visit order as a vector of indices
    vector<int> BFS_order(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;
        vector<int> order;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);

            for (auto &p : adjList[u]) {
                int v = p.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return order;
    }

    // Return DFS visit order (iterative) as a vector of indices
    vector<int> DFS_order(int start) {
        vector<bool> visited(SIZE, false);
        stack<int> st;
        vector<int> order;

        st.push(start);
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            order.push_back(u);

            for (auto &p : adjList[u]) {
                int v = p.first;
                if (!visited[v]) st.push(v);
            }
        }
        return order;
    }

    // Single-source shortest paths (Dijkstra). Returns vector of distances.
    vector<int> dijkstra(int start) {
        const int INF = numeric_limits<int>::max();
        vector<int> dist(SIZE, INF);
        // min-heap of (distance, vertex)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();
            int d = top.first;
            int u = top.second;
            if (d > dist[u]) continue;

            for (auto &p : adjList[u]) {
                int v = p.first;
                int w = p.second;
                if (dist[u] != INF && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return dist;
    }

    // Print adjacency list using names for nodes
    void printGraphWithNames(const vector<string> &names) {
        cout << "Graph's adjacency list (names):" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            string nodeName = (i < names.size() ? names[i] : to_string(i));
            cout << nodeName << " (" << i << ") --> ";
            for (Pair v : adjList[i]) {
                string nbr = (v.first < names.size() ? names[v.first] : to_string(v.first));
                cout << "(" << nbr << ", " << v.second << ") ";
            }
            cout << endl;
        }
    }
};

// Disjoint Set Union (Union-Find) for Kruskal's algorithm
class DSU {
    vector<int> parent, rankv;
public:
    DSU(int n) : parent(n), rankv(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rankv[a] < rankv[b]) parent[a] = b;
        else if (rankv[b] < rankv[a]) parent[b] = a;
        else { parent[b] = a; rankv[a]++; }
    }
};

// Kruskal's algorithm: return list of edges in the MST
vector<Edge> kruskalMST(int V, const vector<Edge> &edges) {
    vector<Edge> sorted = edges;
    sort(sorted.begin(), sorted.end(), [](const Edge &a, const Edge &b){
        return a.weight < b.weight;
    });

    DSU dsu(V);
    vector<Edge> mst;
    for (auto &e : sorted) {
        if (dsu.find(e.src) != dsu.find(e.dest)) {
            dsu.unite(e.src, e.dest);
            mst.push_back(e);
            if ((int)mst.size() == V - 1) break;
        }
    }
    return mst;
}

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,9},{0,2,7},{0,3,13},{2,3,5},{2,4,5},
        {1,7,5},{3,8,4},{3,5,29},{8,9,31},{9,10,13},{1,6,28},{6,7,24}
    };

    // Creates graph
    Graph graph(edges);

    // A real-world example: small water distribution network
    // Nodes are towns; edge weights are pipe lengths in kilometers.
    vector<string> towns = {
        "Central",   // 0
        "Riverside", // 1
        "Hilltown",  // 2
        "Lakeside",  // 3
        "Maple",     // 4
        "Easton",    // 5
        "Westfield", // 6
        "Northport", // 7
        "Southbay",  // 8
        "Greenwich", // 9
        "Oakridge"   //10
    };

    cout << "Water distribution network (nodes = towns, weights = pipe length km)\n" << endl;
    graph.printGraphWithNames(towns);

    // Show index mapping for interactive selection
    cout << "\nTowns (index):" << endl;
    for (int i = 0; i < towns.size(); ++i)
        cout << i << ": " << towns[i] << (i+1 == towns.size() ? "" : ", ");
    cout << "\n" << endl;

    // Ask user for a start town
    cout << "Enter start town index (0-" << (int)towns.size()-1 << ") [default 0]: ";
    int start = 0;
    if (!(cin >> start)) {
        // invalid input (e.g., EOF); default to 0
        cin.clear();
        start = 0;
    }
    if (start < 0 || start >= (int)towns.size()) {
        cout << "Invalid index, defaulting to 0." << endl;
        start = 0;
    }

    // Run traversals and show town names in visit order
    auto dfsOrder = graph.DFS_order(start);
    cout << "\nInspection route (DFS) starting from " << towns[start] << ":" << endl;
    for (int idx : dfsOrder) cout << towns[idx] << " ";
    cout << endl;

    auto bfsOrder = graph.BFS_order(start);
    cout << "\nContaminant spread (BFS) starting from " << towns[start] << ":" << endl;
    for (int idx : bfsOrder) cout << towns[idx] << " ";
    cout << endl << endl;

    // Compute and print shortest distances from start using Dijkstra
    auto distances = graph.dijkstra(start);
    cout << "Shortest pipe distances from town " << towns[start] << " (km):" << endl;
    for (int i = 0; i < (int)distances.size(); ++i) {
        cout << towns[start] << " -> " << (i < (int)towns.size() ? towns[i] : to_string(i)) << " : ";
        if (distances[i] == numeric_limits<int>::max())
            cout << "INF";
        else
            cout << distances[i];
        cout << endl;
    }

    cout << endl;
    // Compute and print Minimum Spanning Tree (Kruskal)
    auto mst = kruskalMST(SIZE, edges);
    cout << "Minimum Spanning Tree (water network backbone) edges:" << endl;
    int total = 0;
    for (auto &e : mst) {
        string a = (e.src < (int)towns.size() ? towns[e.src] : to_string(e.src));
        string b = (e.dest < (int)towns.size() ? towns[e.dest] : to_string(e.dest));
        cout << "Pipe from " << a << " to " << b << " with length: " << e.weight << " km" << endl;
        total += e.weight;
    }
    cout << "Total backbone pipe length: " << total << " km" << endl;

    return 0;
}