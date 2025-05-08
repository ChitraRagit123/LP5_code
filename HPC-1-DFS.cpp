#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

void dfs(int node) {
    // Mark the node as visited
    visited[node] = true;
    cout << node << " ";

    // Parallelizing the recursive DFS calls for each unvisited neighbor using OpenMP tasks
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < graph[node].size(); i++) {
                int adj_node = graph[node][i];
                // If the neighbor hasn't been visited, create a task to explore it
                if (!visited[adj_node]) {
                    #pragma omp task
                    dfs(adj_node);
                }
            }
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter number of nodes, edges, and start node: ";
    cin >> n >> m >> start_node;

    cout << "Enter pairs of edges:" << endl;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // undirected graph
    }

    // Initialize visited array
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    cout << "DFS Traversal: ";
    dfs(start_node);
    cout << endl;

    return 0;
}
