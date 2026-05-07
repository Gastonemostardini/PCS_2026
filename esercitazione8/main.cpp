#include <iostream>
#include <vector>
#include <set>
#include "undirected_edge.h"
#include "undirected_graph.h"

using namespace std;

int main() {
    int a = 1;
    int b = 4;
    int c = 5;

    UndirectedEdge<int> ua(a, b);
    UndirectedEdge<int> ub(b, c);
    UndirectedEdge<int> uc(a, c);

    set<int> nodes = { c, 86 };
    set<UndirectedEdge<int>> edges = { ub, ub, uc };
    set<UndirectedEdge<int>> edges2 = { ub };

    UndirectedGraph<int> graph(nodes);
    UndirectedGraph<int> graph2(edges);
    UndirectedGraph<int> graph3(nodes, edges2);
    cout << graph << endl;

    //graph.add_edge(1, 1);
    UndirectedEdge aaaa(1, 3);
    graph.add_edge(aaaa);
    UndirectedGraph<int> graph4 = graph - graph2;
    cout << graph << endl;
    cout << graph2 << endl;
    cout << graph3 << endl;
    cout << graph4 << endl;


    return 0;
}