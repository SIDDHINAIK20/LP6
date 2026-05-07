#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;



// =====================================================
//                    GRAPH CLASS
// =====================================================

class Graph
{
    int vertices;

    vector<vector<int>> adj;

public:

    Graph(int v)
    {
        vertices = v;

        adj.resize(v);
    }



    // ---------------- ADD EDGE ----------------

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);

        adj[v].push_back(u);
    }



    // =====================================================
    //                  SEQUENTIAL BFS
    // =====================================================

    void sequential_BFS(int start)
    {
        vector<bool> visited(vertices, false);

        queue<int> q;

        visited[start] = true;

        q.push(start);



        while(!q.empty())
        {
            int node = q.front();

            q.pop();

            cout << node << " ";



            for(int i = 0; i < adj[node].size(); i++)
            {
                int next = adj[node][i];

                if(!visited[next])
                {
                    visited[next] = true;

                    q.push(next);
                }
            }
        }
    }



    // =====================================================
    //                  PARALLEL BFS
    // =====================================================

    void parallel_BFS(int start)
    {
        vector<bool> visited(vertices, false);

        queue<int> q;

        visited[start] = true;

        q.push(start);



        while(!q.empty())
        {
            int node = q.front();

            q.pop();

            cout << node << " ";



            #pragma omp parallel for
            for(int i = 0; i < adj[node].size(); i++)
            {
                int next = adj[node][i];

                if(!visited[next])
                {
                    #pragma omp critical
                    {
                        if(!visited[next])
                        {
                            visited[next] = true;

                            q.push(next);
                        }
                    }
                }
            }
        }
    }



    // =====================================================
    //                  SEQUENTIAL DFS
    // =====================================================

    void sequential_DFS_util(int node, vector<bool> &visited)
    {
        visited[node] = true;

        cout << node << " ";



        for(int i = 0; i < adj[node].size(); i++)
        {
            int next = adj[node][i];

            if(!visited[next])
            {
                sequential_DFS_util(next, visited);
            }
        }
    }



    void sequential_DFS(int start)
    {
        vector<bool> visited(vertices, false);

        sequential_DFS_util(start, visited);
    }



    // =====================================================
    //                  PARALLEL DFS
    // =====================================================

    void parallel_DFS_util(int node, vector<bool> &visited)
    {
        visited[node] = true;

        cout << node << " ";



        #pragma omp parallel for
        for(int i = 0; i < adj[node].size(); i++)
        {
            int next = adj[node][i];

            if(!visited[next])
            {
                #pragma omp critical
                {
                    if(!visited[next])
                    {
                        parallel_DFS_util(next, visited);
                    }
                }
            }
        }
    }



    void parallel_DFS(int start)
    {
        vector<bool> visited(vertices, false);

        parallel_DFS_util(start, visited);
    }
};



// =====================================================
//                        MAIN
// =====================================================

int main()
{
    int vertices;
    int edges;

    cout << "Enter number of vertices: ";
    cin >> vertices;

    cout << "Enter number of edges: ";
    cin >> edges;



    Graph g(vertices);



    // Input edges
    cout << "\nEnter edges:\n";

    for(int i = 0; i < edges; i++)
    {
        int u, v;

        cin >> u >> v;

        g.addEdge(u, v);
    }



    int start;

    cout << "\nEnter starting vertex: ";
    cin >> start;



    // =====================================================
    //                  SEQUENTIAL BFS
    // =====================================================

    cout << "\nSequential BFS Traversal:\n";

    auto bfs_seq_start = high_resolution_clock::now();

    g.sequential_BFS(start);

    auto bfs_seq_end = high_resolution_clock::now();



    duration<double> bfs_seq_time =
        bfs_seq_end - bfs_seq_start;



    // =====================================================
    //                  PARALLEL BFS
    // =====================================================

    cout << "\n\nParallel BFS Traversal:\n";

    auto bfs_par_start = high_resolution_clock::now();

    g.parallel_BFS(start);

    auto bfs_par_end = high_resolution_clock::now();



    duration<double> bfs_par_time =
        bfs_par_end - bfs_par_start;



    // =====================================================
    //                  BFS RESULTS
    // =====================================================

    cout << "\n\nSequential BFS Time : "
         << bfs_seq_time.count() << " seconds";



    cout << "\nParallel BFS Time   : "
         << bfs_par_time.count() << " seconds";



    cout << "\nBFS Speedup         : "
         << bfs_seq_time.count() /
            bfs_par_time.count();



    // =====================================================
    //                  SEQUENTIAL DFS
    // =====================================================

    cout << "\n\n\nSequential DFS Traversal:\n";

    auto dfs_seq_start = high_resolution_clock::now();

    g.sequential_DFS(start);

    auto dfs_seq_end = high_resolution_clock::now();



    duration<double> dfs_seq_time =
        dfs_seq_end - dfs_seq_start;



    // =====================================================
    //                  PARALLEL DFS
    // =====================================================

    cout << "\n\nParallel DFS Traversal:\n";

    auto dfs_par_start = high_resolution_clock::now();

    g.parallel_DFS(start);

    auto dfs_par_end = high_resolution_clock::now();



    duration<double> dfs_par_time =
        dfs_par_end - dfs_par_start;



    // =====================================================
    //                  DFS RESULTS
    // =====================================================

    cout << "\n\nSequential DFS Time : "
         << dfs_seq_time.count() << " seconds";



    cout << "\nParallel DFS Time   : "
         << dfs_par_time.count() << " seconds";



    cout << "\nDFS Speedup         : "
         << dfs_seq_time.count() /
            dfs_par_time.count();



    return 0;
}
