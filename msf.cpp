#include <iostream>
#include <limits>
#include <tuple>
#include <vector>

/*
 * Boruvka's algorithm for finding the Minimum Spanning Forest (MSF).
 * Code Generated by GPT-4.
 * The correctness is not verified.
 */

struct CSRGraph {
  int num_vertices;
  std::vector<int> row_ptr;
  std::vector<int> col_idx;
  std::vector<int> weights;
};

struct UnionFind {
  std::vector<int> parent;

  UnionFind(int size) : parent(size) {
    for (int i = 0; i < size; ++i) {
      parent[i] = i;
    }
  }

  int find(int x) {
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  void unite(int x, int y) {
    int root_x = find(x);
    int root_y = find(y);
    if (root_x != root_y) {
      parent[root_y] = root_x;
    }
  }
};

std::vector<std::tuple<int, int, int>> boruvkaMSF(const CSRGraph& graph) {
  int n = graph.num_vertices;
  UnionFind uf(n);
  std::vector<std::tuple<int, int, int>> msf;
  int num_components = n;

  while (num_components > 1) {
    // Initialize an array to store the cheapest edge for each connected
    // component
    std::vector<int> cheapest(n, -1);
    std::vector<int> min_weights(n, std::numeric_limits<int>::max());

    // Iterate through all vertices
    for (int u = 0; u < n; ++u) {
      // Get the root of the connected component containing vertex u
      int u_root = uf.find(u);

      // Iterate through all edges connected to vertex u
      for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; ++i) {
        int v = graph.col_idx[i];
        // Get the root of the connected component containing vertex v
        int v_root = uf.find(v);

        // If u and v are in the same connected component, ignore the edge
        if (u_root == v_root) {
          continue;
        }

        int w = graph.weights[i];

        // Update the cheapest edge for connected components containing u and v
        if (w < min_weights[u_root]) {
          min_weights[u_root] = w;
          cheapest[u_root] = v;
        }
        if (w < min_weights[v_root]) {
          min_weights[v_root] = w;
          cheapest[v_root] = u;
        }
      }
    }

    // Iterate through all connected components
    for (int c = 0; c < n; ++c) {
      // If there's a cheapest edge for the connected component, add it to the
      // MSF
      if (cheapest[c] != -1) {
        int u = c;
        int v = cheapest[c];
        int w = min_weights[c];

        // If the connected components containing u and v are not already
        // united, unite them and add the edge to the MSF
        if (uf.find(u) != uf.find(v)) {
          uf.unite(u, v);
          msf.emplace_back(u, v, w);
          --num_components;
        }
      }
    }
  }

  return msf;
}

int main() {
  CSRGraph graph = {5,
                    {0, 2, 5, 8, 11, 14},
                    {1, 3, 0, 2, 3, 4, 0, 1, 4, 0, 1, 3},
                    {7, 5, 7, 3, 9, 7, 8, 9, 4, 5, 2, 6}};

  std::vector<std::tuple<int, int, int>> msf = boruvkaMSF(graph);

  for (const auto& [u, v, w] : msf) {
    std::cout << "Edge (" << u << ", " << v << ") with weight " << w
              << std::endl;
  }

  return 0;
}