#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

// Recursive parallel DFS
void dfs(int node) {
    #pragma omp critical
    {
        if (visited[node]) return;
        visited[node] = true;
        cout << node << " ";
    }

    // Parallelize the neighbor traversal
    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); i++) {
        int neighbor = graph[node][i];
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
}

int main() {
    int n, m, start_node;
    cout << "Enter number of nodes, edges and start node: ";
    cin >> n >> m >> start_node;

    cout << "Enter pairs of edges:\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected graph
    }

    // Initialize visited array in parallel
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    cout << "DFS Traversal: ";

    // Start DFS in parallel region
    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs(start_node);
        }
    }

    cout << endl;
    return 0;
}

