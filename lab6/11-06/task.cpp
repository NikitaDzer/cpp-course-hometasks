#include <boost/graph/adjacency_matrix.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using Graph = boost::adjacency_matrix<boost::undirectedS, int>;
using Vertex = Graph::vertex_descriptor;

int main() {
    const int num_vertices = 10;

    std::random_device rd;
    std::default_random_engine engine{rd()};
    std::uniform_int_distribution<int> dist{1, 10};

    std::vector<std::vector<int>> adjacency_matrix(num_vertices, std::vector<int>(num_vertices));

    for (int i = 0; i < num_vertices; ++i) {
        for (int j = i + 1; j < num_vertices; ++j) {
            int weight = dist(engine);
            adjacency_matrix[i][j] = weight;
            adjacency_matrix[j][i] = weight;
        }
    }

    Graph graph(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = i + 1; j < num_vertices; ++j) {
            boost::add_edge(i, j, graph);
        }
    }

    std::cout << "Adjacency Matrix:\n";
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (i == j) {
                std::cout << "  0";
            } else {
                std::cout << std::setw(3) << adjacency_matrix[i][j];
            }
        }
        std::cout << '\n';
    }

    std::vector<int> path(num_vertices);
    std::iota(path.begin(), path.end(), 0);

    int min_cost = std::numeric_limits<int>::max();
    std::vector<int> best_path;

    do {
        int cost = 0;
        for (int i = 0; i < num_vertices - 1; ++i) {
            cost += adjacency_matrix[path[i]][path[i + 1]];
        }
        cost += adjacency_matrix[path.back()][path.front()];

        if (cost < min_cost) {
            min_cost = cost;
            best_path = path;
        }
    } while (std::next_permutation(path.begin(), path.end()));

    std::cout << "\nOptimal path: ";
    for (int v : best_path) {
        std::cout << v << " -> ";
    }
    std::cout << best_path.front() << '\n';

    std::cout << "Total cost: " << min_cost << '\n';

    return 0;
}
