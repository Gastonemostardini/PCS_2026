#include <iostream>
#include <vector>
#include <set>
#include "undirected_edge.h"
#include "undirected_graph.h"
#include "undirected_tree.h"

using namespace std;

int main() {
    int a = 1;
    int b = 4;
    int c = 5;

    UndirectedEdge<int> ua(a, c);
    UndirectedEdge<int> ub(a, 23);
    UndirectedEdge<int> uc(c, 89);

    set<int> nodes = { 1 };
    set<UndirectedEdge<int>> edges = { ua, ub, uc };

    TreeGraph<int> graph(nodes, edges);
    graph.add_edge(23, 9);
    cout << graph << endl;

    return 0;
}