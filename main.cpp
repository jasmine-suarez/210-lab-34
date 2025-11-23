// COMSC-210 | Lab 34 | Jasmine Suarez
// IDE used: VS Code

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

const int SIZE = 12;

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
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,9},{0,2,7},{0,3,22},{2,3,5},{2,4,5},
        {1,7,5},{3,8,4},{0,9,29},{8,9,31},{9,10,13},{1,6,28},{6,7,24}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();

    // Prints DFS,BFS from node 0
    graph.DFS(0);
    graph.BFS(0);


    return 0;
}