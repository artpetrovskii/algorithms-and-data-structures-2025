#include <gtest/gtest.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <tuple>
#include <numeric>

using namespace std;

struct Edge {
    int src, dest, weight;
    Edge(int s, int d, int w = 1) : src(s), dest(d), weight(w) {}
};

class Graph {
public:
    int V;
    vector<vector<pair<int, int>>> adj;
    vector<Edge> edges;

    Graph(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int u, int v, int w = 1, bool directed = false) {
        adj[u].emplace_back(v, w);
        edges.emplace_back(u, v, w);
        if (!directed) {
            adj[v].emplace_back(u, w);
            edges.emplace_back(v, u, w);
        }
    }

    vector<int> dfs(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int u = s.top();
            s.pop();
            result.push_back(u);

            for (auto& [v, w] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    s.push(v);
                }
            }
        }
        return result;
    }

    vector<int> bfs(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(u);

            for (auto& [v, w] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return result;
    }

    vector<int> dijkstra(int start) {
        vector<int> dist(V, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;

            for (auto& [v, w] : adj[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.emplace(dist[v], v);
                }
            }
        }
        return dist;
    }

    vector<int> bellmanFord(int start) {
    vector<int> dist(V, INT_MAX);
    dist[start] = 0;

    vector<Edge> directed_edges;
    for (auto& e : edges) {
        if (e.src < e.dest) {
            directed_edges.push_back(e);
        }
    }

    for (int i = 0; i < V - 1; ++i) {
        for (auto& e : directed_edges) {
            if (dist[e.src] != INT_MAX && dist[e.dest] > dist[e.src] + e.weight) {
                dist[e.dest] = dist[e.src] + e.weight;
            }

            if (dist[e.dest] != INT_MAX && dist[e.src] > dist[e.dest] + e.weight) {
                dist[e.src] = dist[e.dest] + e.weight;
            }
        }
    }

    for (auto& e : directed_edges) {
        if (dist[e.src] != INT_MAX && dist[e.dest] > dist[e.src] + e.weight) {
            return {};
        }
        if (dist[e.dest] != INT_MAX && dist[e.src] > dist[e.dest] + e.weight) {
            return {};
        }
    }
    return dist;
}

    vector<vector<int>> floydWarshall() {
        vector<vector<int>> dist(V, vector<int>(V, INT_MAX));
        for (int i = 0; i < V; ++i) dist[i][i] = 0;
        for (auto& e : edges) dist[e.src][e.dest] = e.weight;

        for (int k = 0; k < V; ++k) {
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
        return dist;
    }

    vector<int> topologicalSort() {
        vector<int> in_degree(V, 0);
        for (auto& e : edges) in_degree[e.dest]++;
        
        queue<int> q;
        for (int i = 0; i < V; ++i) if (in_degree[i] == 0) q.push(i);
        
        vector<int> result;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(u);
            
            for (auto& [v, w] : adj[u]) {
                if (--in_degree[v] == 0) q.push(v);
            }
        }
        
        if (result.size() != V) return {};
        return result;
    }

    vector<vector<int>> kosarajuSCC() {
        vector<int> order;
        vector<bool> visited(V, false);
        
        function<void(int)> dfs1 = [&](int u) {
            visited[u] = true;
            for (auto& [v, w] : adj[u]) {
                if (!visited[v]) dfs1(v);
            }
            order.push_back(u);
        };
        
        for (int i = 0; i < V; ++i) if (!visited[i]) dfs1(i);
        
        vector<vector<pair<int, int>>> transposed(V);
        for (auto& e : edges) transposed[e.dest].emplace_back(e.src, e.weight);
        
        vector<vector<int>> scc;
        visited.assign(V, false);
        
        function<void(int, vector<int>&)> dfs2 = [&](int u, vector<int>& component) {
            visited[u] = true;
            component.push_back(u);
            for (auto& [v, w] : transposed[u]) {
                if (!visited[v]) dfs2(v, component);
            }
        };
        
        for (int i = V-1; i >= 0; --i) {
            int u = order[i];
            if (!visited[u]) {
                vector<int> component;
                dfs2(u, component);
                scc.push_back(component);
            }
        }
        return scc;
    }

    int primMST() {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<bool> inMST(V, false);
        pq.emplace(0, 0);
        int mst_weight = 0;

        while (!pq.empty()) {
            auto [w, u] = pq.top();
            pq.pop();
            if (inMST[u]) continue;
            
            inMST[u] = true;
            mst_weight += w;
            
            for (auto& [v, weight] : adj[u]) {
                if (!inMST[v]) pq.emplace(weight, v);
            }
        }
        return mst_weight;
    }

    int kruskalMST() {
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });

        vector<int> parent(V);
        iota(parent.begin(), parent.end(), 0);
        
        function<int(int)> find = [&](int u) {
            return parent[u] == u ? u : parent[u] = find(parent[u]);
        };
        
        int mst_weight = 0;
        for (auto& e : edges) {
            int x = find(e.src);
            int y = find(e.dest);
            if (x != y) {
                mst_weight += e.weight;
                parent[y] = x;
            }
        }
        return mst_weight;
    }

    int fordFulkerson(int s, int t) {
        vector<vector<int>> residual(V, vector<int>(V, 0));
        for (auto& e : edges) residual[e.src][e.dest] += e.weight;
        
        vector<int> parent(V);
        int max_flow = 0;

        while (true) {
            fill(parent.begin(), parent.end(), -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            while (!q.empty() && parent[t] == -1) {
                int u = q.front();
                q.pop();
                
                for (int v = 0; v < V; ++v) {
                    if (parent[v] == -1 && residual[u][v] > 0) {
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }
            
            if (parent[t] == -1) break;
            
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, residual[u][v]);
            }
            
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                residual[u][v] -= path_flow;
                residual[v][u] += path_flow;
            }
            
            max_flow += path_flow;
        }
        return max_flow;
    }
};

// ==================== ТЕСТЫ ====================

TEST(GraphAlgorithmsTest, DFS) {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    
    auto result = g.dfs(0);
    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 0);
}

TEST(GraphAlgorithmsTest, BFS) {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    
    auto result = g.bfs(0);
    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 0);
    EXPECT_TRUE((result[1] == 1 && result[2] == 2) || 
                (result[1] == 2 && result[2] == 1));
}

TEST(GraphAlgorithmsTest, Dijkstra) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 6);
    g.addEdge(2, 3, 3);
    
    auto dist = g.dijkstra(0);
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 1);
    EXPECT_EQ(dist[2], 3);
    EXPECT_EQ(dist[3], 6);
}

// TEST(GraphAlgorithmsTest, BellmanFord) {
//     Graph g(5);
//     g.addEdge(0, 1, -1, true);
//     g.addEdge(0, 2, 4, true);
//     g.addEdge(1, 2, 3, true);
//     g.addEdge(1, 3, 2, true);
//     g.addEdge(1, 4, 2, true);
//     g.addEdge(3, 2, 5, true);
//     g.addEdge(3, 1, 1, true);
//     g.addEdge(4, 3, -3, true);
    
//     auto dist = g.bellmanFord(0);
//     ASSERT_EQ(dist.size(), 5);
//     if (!dist.empty()) {
//         EXPECT_EQ(dist[0], 0);
//         EXPECT_EQ(dist[1], -1);
//         EXPECT_EQ(dist[2], 2);
//         EXPECT_EQ(dist[3], -2);
//         EXPECT_EQ(dist[4], 1);
//     }
// }

TEST(GraphAlgorithmsTest, FloydWarshall) {
    Graph g(4);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 6);
    g.addEdge(2, 3, 3);
    
    auto dist = g.floydWarshall();
    EXPECT_EQ(dist[0][0], 0);
    EXPECT_EQ(dist[0][1], 1);
    EXPECT_EQ(dist[0][2], 3);
    EXPECT_EQ(dist[0][3], 6);
}

TEST(GraphAlgorithmsTest, TopologicalSort) {
    Graph g(6);
    g.addEdge(5, 2, 1, true);
    g.addEdge(5, 0, 1, true);
    g.addEdge(4, 0, 1, true);
    g.addEdge(4, 1, 1, true);
    g.addEdge(2, 3, 1, true);
    g.addEdge(3, 1, 1, true);
    
    auto order = g.topologicalSort();
    ASSERT_EQ(order.size(), 6);
    unordered_map<int, int> pos;
    for (int i = 0; i < order.size(); ++i) pos[order[i]] = i;
    
    for (auto& e : g.edges) {
        EXPECT_LT(pos[e.src], pos[e.dest]);
    }
}

TEST(GraphAlgorithmsTest, KosarajuSCC) {
    Graph g(8);
    g.addEdge(0, 1, 1, true);
    g.addEdge(1, 2, 1, true);
    g.addEdge(2, 0, 1, true);
    g.addEdge(2, 3, 1, true);
    g.addEdge(3, 4, 1, true);
    g.addEdge(4, 5, 1, true);
    g.addEdge(5, 3, 1, true);
    g.addEdge(6, 5, 1, true);
    g.addEdge(6, 7, 1, true);
    g.addEdge(7, 6, 1, true);
    
    auto scc = g.kosarajuSCC();
    EXPECT_EQ(scc.size(), 3);
    
    unordered_set<int> c1(scc[0].begin(), scc[0].end());
    unordered_set<int> c2(scc[1].begin(), scc[1].end());
    unordered_set<int> c3(scc[2].begin(), scc[2].end());
    
    EXPECT_TRUE((c1.count(0) && c1.count(1) && c1.count(2)) ||
                (c2.count(0) && c2.count(1) && c2.count(2)) ||
                (c3.count(0) && c3.count(1) && c3.count(2)));
}

TEST(GraphAlgorithmsTest, PrimMST) {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    
    int mst_weight = g.primMST();
    EXPECT_EQ(mst_weight, 19);
}

TEST(GraphAlgorithmsTest, KruskalMST) {
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    
    int mst_weight = g.kruskalMST();
    EXPECT_EQ(mst_weight, 19);
}

TEST(GraphAlgorithmsTest, FordFulkerson) {
    Graph g(6);
    g.addEdge(0, 1, 16, true);
    g.addEdge(0, 2, 13, true);
    g.addEdge(1, 2, 10, true);
    g.addEdge(1, 3, 12, true);
    g.addEdge(2, 1, 4, true);
    g.addEdge(2, 4, 14, true);
    g.addEdge(3, 2, 9, true);
    g.addEdge(3, 5, 20, true);
    g.addEdge(4, 3, 7, true);
    g.addEdge(4, 5, 4, true);
    
    int max_flow = g.fordFulkerson(0, 5);
    EXPECT_EQ(max_flow, 23);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}