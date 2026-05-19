#include <iostream>
#include <vector>
#include <set>
#include "undirected_edge.h"
#include "undirected_graph.h"
#include "undirected_tree.h"
#include "search_graph.h"
#include "weighed_edge.h"
#include "weighed_graph.h"

using namespace std;

int main() {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    int f = 6;

    set<int> nodes = { a, b, c, d, e, f, 7, 8, 9 };
    set<UndirectedEdge<int>> edges = {
        { a, b },
        { a, c },
        { a, d },
        { a, e },
        { e, f },
        { e, b },
        { c, d },
        { 7, d },
        { 7, 8 },
        { 8, 9 },
        { 7, 9 },
        { 7, e },
    };

    UndirectedGraph<int> graph(nodes, edges);
    //cout << graph << endl;

    SearchGraph search_graph(graph, 4);

    //TreeGraph result = search_graph.breadth_first_search();
    TreeGraph result2 = search_graph.depth_first_search();

    Fifo<std::pair<int, int>> fifo;
    //Fifo<int> fifo;
    TreeGraph result3 = graph_visit(graph, 4, fifo);

    Lifo<std::pair<int, int>> lifo;
    //Lifo<int> lifo;
    TreeGraph result4 = graph_visit(graph, 1, lifo);

    //cout << "3: " << endl;
    //cout << result3 << endl;
    //cout << "4: " << endl;
    //cout << result4 << endl;
    //cout << "GIUSTO" << endl;
    //cout << recursive_dfs(graph, 1) << endl;


    WeighedEdge w_e(1,2, 3);
    cout << w_e << endl;
    cout << dijkstra(graph, 1) << endl;
    WeighedGraph<int> graph2({ 1,2,3,4,5 }, { {1,2,1}, {2,3,1}, {4,5,1}, {1,5,100} });
    cout << dijkstra(graph2, 1) << endl;

    return 0;
}