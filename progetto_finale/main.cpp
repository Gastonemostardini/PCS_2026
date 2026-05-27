#include <iostream>
#include <vector>
#include <set>
#include "edge.h"
#include "graph.h"
#include "tree.h"
#include "search_graph.h"
#include "weighed_edge.h"
#include "weighed_graph.h"
#include "gradiente.h"
#include "functions.h"
#include "Eigen/Eigen"

using namespace std;

int main() {
    //vector<bool> a(10, true);
    //vector<bool> b(10, true);
    //vector<bool> c = a ^ b;
    //bool d = a * b;

    //cout << c << endl;
    //cout << d << endl;


    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    //int e = 5;
    //int f = 6;

    set<int> nodes = { a, b, c, d };
    set<Edge<int>> edges = {
        { a, b },
        { a, c },
        { a, d },
        { d, b },
        //{ e, f },
        //{ e, b },
        //{ c, d },
        //{ 7, d },
        //{ 7, 8 },
        //{ 8, 9 },
        //{ 7, 9 },
        //{ 7, e },
    };

    Graph<int> graph(nodes, { {a, b}, {b, d}, {a, d}, { b, c }, { c, d } });
    //cout << "edges:" << endl;
    //cout << edges << endl;
    //cout << "graph:" << endl;
    //cout << graph << endl;
    //Cycles<int, Edge<int>> S1(graph.all_edges(), { {a, b}, {b, d}, {a, d}, { b, c }, { c, d } });
    //std::vector<Cycles<int, Edge<int>>> S = { S1 };
    cout << de_pina(graph) << endl;

    return 0;
    /*
    SearchGraph search_graph(graph, 4);

    TreeGraph result = search_graph.breadth_first_search();
    TreeGraph result2 = search_graph.depth_first_search();

    Fifo<std::pair<int, int>> fifo;
    //Fifo<int> fifo;
    TreeGraph result3 = graph_visit(graph, 4, fifo);

    Lifo<std::pair<int, int>> lifo;
    //Lifo<int> lifo;
    TreeGraph result4 = graph_visit(graph, 1, lifo);

    cout << "3: " << endl;
    cout << result3 << endl;
    cout << "4: " << endl;
    cout << result4 << endl;
    cout << "GIUSTO" << endl;
    cout << recursive_dfs(graph, 1) << endl;


    WeighedEdge w_e(1,2, 3);
    cout << w_e << endl;
    cout << dijkstra(graph, 1) << endl;
    WeighedGraph<int> graph2({ 1,2,3,4,5 }, { {1,2,1}, {2,3,1}, {4,5,1}, {1,5,100} });
    cout << graph2 << endl;

    cout << dijkstra(graph2, 1) << endl;
    */

    return 0;
}
