#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>

using namespace std;

// Graph class to represent the graph in CSR format
class Graph {
public:
    int n; // Number of vertices
    vector<int> row_ptr; // row_ptr: vector containing starting index for each vertex in col_idx
    vector<int> col_idx; // col_idx: vector containing adjacent vertices for each vertex
    vector<int> weights; // Edge weights

    // Constructor to initialize the graph
    Graph(int n, const vector<int> &row_ptr, const vector<int> &col_idx, const vector<int> &weights)
        : n(n), row_ptr(row_ptr), col_idx(col_idx), weights(weights) {}

    // Function to get the weight of the edge between vertices u and v
    int weight(int u, int v) const {
        for (int i = row_ptr[u]; i < row_ptr[u + 1]; ++i) {
            if (col_idx[i] == v) {
                return weights[i];
            }
        }
        return 0;
    }
};

// Function to calculate the modularity of the current graph partition
double modularity(const Graph &graph, const vector<int> &communities, int m) {
    double q = 0.0;
    // Iterate through all pairs of vertices in the graph
    for (int u = 0; u < graph.n; ++u) {
        for (int v = 0; v < graph.n; ++v) {
            // Check if vertices u and v belong to the same community
            if (communities[u] == communities[v]) {
                int a_uv = graph.weight(u, v); // Weight of the edge between u and v
                int k_u = graph.row_ptr[u + 1] - graph.row_ptr[u]; // Degree of vertex u
                int k_v = graph.row_ptr[v + 1] - graph.row_ptr[v]; // Degree of vertex v
                q += a_uv - (static_cast<double>(k_u * k_v) / (2 * m));
            }
        }
    }
    return q / (2 * m);
}

// Louvain algorithm for community detection
vector<int> louvain(Graph &graph) {
    int m = 0; // Total weight of the graph
    for (int w : graph.weights) {
        m += w;
    }

    vector<int> communities(graph.n);
    for (int i = 0; i < graph.n; ++i) {
        communities[i] = i; // Initially, each vertex is in its own community
    }

    bool improvement = true;
    // Continue optimizing the modularity until no further improvement can be made
    while (improvement) {
        improvement = false;
        // Iterate through all vertices in the graph
        for (int u = 0; u < graph.n; ++u) {
            int original_community = communities[u];
            double max_delta_q = numeric_limits<double>::lowest();
            int best_community = original_community;

            // Calculate the sum of weights between vertex u and each neighboring community
               unordered_map<int, int> neighbor_communities;
            for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; ++i) {
                int v = graph.col_idx[i];
                int weight_uv = graph.weights[i];
                int community_v = communities[v];
                neighbor_communities[community_v] += weight_uv;
            }

            // Remove vertex u from its current community
            communities[u] = -1;
            neighbor_communities[original_community] -= graph.row_ptr[u + 1] - graph.row_ptr[u];

            // Evaluate the change in modularity for moving vertex u to each neighboring community
            for (const auto &nc : neighbor_communities) {
                int community = nc.first;
                int weight_sum = nc.second;

                communities[u] = community;
                double delta_q = modularity(graph, communities, m) - modularity(graph, communities, m);

                if (delta_q > max_delta_q) {
                    max_delta_q = delta_q;
                    best_community = community;
                }
            }

            // Move vertex u to the community with the largest modularity gain
            if (best_community != original_community) {
                communities[u] = best_community;
                improvement = true;
            }
        }
    }

    return communities;
}

int main() {
    // Create a graph with 5 vertices
    int n = 5;
    vector<int> row_ptr = {0, 2, 4, 6, 8, 10};
    vector<int> col_idx = {1, 2, 0, 2, 3, 0, 1, 3, 4, 1, 3};
    vector<int> weights = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    Graph graph(n, row_ptr, col_idx, weights);

    // Run the Louvain algorithm
    vector<int> communities = louvain(graph);

    // Print the communities
    for (int i = 0; i < n; ++i) {
        cout << "Vertex " << i << " is in community " << communities[i] << endl;
    }
    return 0;
}