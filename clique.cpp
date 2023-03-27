#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

/**
 * @brief The most popular algorithm for Maximal Clique Enumeration is the
 * Bron-Kerbosch algorithm. Here is the implementation in C++ with the graph
 * formatted in CSR (Compressed Sparse Row).
 */
using namespace std;

// Graph class to represent the graph in CSR format.
class Graph {
 public:
  int n;                // Number of vertices in the graph
  vector<int> row_ptr;  // row_ptr: vector containing starting index for each
                        // vertex in col_idx
  vector<int>
      col_idx;  // col_idx: vector containing adjacent vertices for each vertex

  // Constructor to initialize the graph
  Graph(int n, const vector<int> &row_ptr, const vector<int> &col_idx)
      : n(n), row_ptr(row_ptr), col_idx(col_idx) {}
};

// Structure to store intermediate states for the iterative Bron-Kerbosch
// algorithm
struct State {
  set<int> R;  // R: set of vertices in the current partial clique
  set<int>
      P;  // P: set of vertices that can be added to the current partial clique
  set<int> X;  // X: set of vertices that are already processed
};

// Non-recursive (iterative) Bron-Kerbosch algorithm to find maximal cliques
void bron_kerbosch_iterative(const Graph &graph) {
  // Initialize stack for storing intermediate states
  stack<State> states;
  State initial_state;
  for (int i = 0; i < graph.n; ++i) {
    initial_state.P.insert(i);
  }
  states.push(initial_state);

  while (!states.empty()) {
    State current_state = states.top();
    states.pop();

    // If both P and X are empty, print the current maximal clique
    if (current_state.P.empty() && current_state.X.empty()) {
      for (int vertex : current_state.R) {
        cout << vertex << " ";
      }
      cout << endl;
    }

    // Iterate over vertices in P (current candidate set)
    while (!current_state.P.empty()) {
      // Select a vertex from P
      int vertex = *current_state.P.begin();

      // Remove the selected vertex from P
      current_state.P.erase(current_state.P.begin());

      // Create a new state by adding the selected vertex to the current partial
      // clique R
      State new_state;
      new_state.R = current_state.R;
      new_state.R.insert(vertex);

      // Get neighbors of the selected vertex
      int start = graph.row_ptr[vertex];
      int end = graph.row_ptr[vertex + 1];
      set<int> neighbors(graph.col_idx.begin() + start,
                         graph.col_idx.begin() + end);

      // Calculate the intersection of P and neighbors
      set_intersection(current_state.P.begin(), current_state.P.end(),
                       neighbors.begin(), neighbors.end(),
                       inserter(new_state.P, new_state.P.begin()));

      // Calculate the intersection of X and neighbors
      set_intersection(current_state.X.begin(), current_state.X.end(),
                       neighbors.begin(), neighbors.end(),
                       inserter(new_state.X, new_state.X.begin()));

      // Push the new state to the stack
      states.push(new_state);

      // Add the selected vertex to the set X of processed vertices
      current_state.X.insert(vertex);
    }
  }
}

int main() {
  int n = 5;
  vector<int> row_ptr = {0, 2, 4, 6, 7, 8};
  vector<int> col_idx = {1, 4, 0, 2, 1, 3, 2, 0};

  Graph graph(n, row_ptr, col_idx);

  cout << "Maximal Cliques:" << endl;
  bron_kerbosch_iterative(graph);

  return 0;
}
