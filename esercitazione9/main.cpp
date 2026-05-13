#include <iostream>
#include <vector>
#include <set>
#include "undirected_edge.h"
#include "undirected_graph.h"
#include "undirected_tree.h"
#include "search_graph.h"

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

    Fifo<int> fifo;
    TreeGraph result3 = graph_visit(graph, 4, fifo);

    Lifo<int> lifo;
    TreeGraph result4 = graph_visit(graph, 4, lifo);

    //cout << result << endl;
    //cout << result2 << endl;
    //cout << result3 << endl;
    //cout << result4 << endl;

    //TreeGraph<int> result5({ a, b }, { { b, a } }, a);
    //cout << result5 << endl;
    //auto nodes2 = result5.all_nodes();
    //nodes2.insert(c);
    //map<int, int> parent = result5.get_parent();
    //parent[a] = c;

    //TreeGraph<int> result6(nodes2, parent, c);
    //cout << result6 << endl;

    //TreeGraph<int> result7(9);
    //cout << result7 << endl;
    //result7.add_tree(9, result5);
    //cout << result7 << endl;

    if (recursive_dfs(graph, 4) == result3)
        cout << "top" << endl;

    cout << result3 << endl;
    cout << "GIUSTO" << endl;
    cout << recursive_dfs(graph, 4) << endl;
    return 0;
}