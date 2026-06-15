#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <cstdint>
#include "input.hpp"
#include "edge.h"
#include "graph.h"
#include "tree.h"
#include "search_graph.h"
#include "cycles.h"
#include "weighed_edge.h"
#include "weighed_graph.h"
#include "functions.h"
#include "Eigen/Eigen"
#include "dfs_project.h"
#include "matrices.h"

using namespace std;

// UNICO PUNTO in cui si decide il tipo dei nodi del circuito.
// Cambiando SOLO questa riga (es. in int) il tipo si propaga ovunque.
using NodeType = pair<int, string>;
// using NodeType = int;
using EdgeType = Edge<NodeType>;
using CycleType = Cycles<NodeType, EdgeType>;

int main()
{
	input<NodeType> parser;
	string nome_file = "testinputPIS.txt";

	cout << "=== Lettura e pulizia del circuito ===\n";
	if (!(parser.parse_file(nome_file)) && parser.check_validity())
	{
		return EXIT_FAILURE;
	}
	parser.clean_graph(); //  rami aperti e isolati
	parser.print_status();

	Graph<NodeType> graph = parser.get_graph(); // otteniamo il grafo

	cout << "\n=== estrazione delle maglie ===\n";

	bool usa_de_pina = false; // cambiare 'usa_de_pina' in 'false' per usare DFS

	std::list<CycleType> base_maglie;

	if (usa_de_pina)
	{
		cout << "-> Il sistema verra' risolto usando le maglie di DE PINA.\n";

		// De pina
		base_maglie = de_pina(graph);
		// cout << "De Pina - maglie fondamentali trovate: " << base_maglie.size() << "\n";
	}
	else
	{
		cout << "-> Il sistema verra' risolto usando le maglie della DFS.\n";

		// Dfs
		NodeType nodo_radice = *graph.all_nodes().begin();
		TreeGraph<NodeType, EdgeType> albero_dfs = recursive_dfs(graph, nodo_radice);
		base_maglie = find_minimal_cycles(graph, albero_dfs);
		// cout << "DFS - maglie fondamentali trovate: " << base_maglie.size() << "\n";
	}

	cout << "\n=== Matrici ===\n";

	CircuitMatrices mats = build_matrices(graph, base_maglie, parser);

	cout << "\n=== soluzione sistema ===\n";

	// risoluzione sistema
	Eigen::MatrixXd A = mats.B.transpose() * mats.R * mats.B;
	Eigen::VectorXd I_maglie;

	// Tentiamo la decomposizione di Cholesky (LLT): riesce (info() == Success)
	// solo se A e' simmetrica definita positiva. In tal caso risolviamo in modo
	// diretto e stabile; altrimenti ripieghiamo sul Gradiente Coniugato.
	Eigen::LLT<Eigen::MatrixXd> chol(A);
	if (chol.info() == Eigen::Success)
	{
		cout << "A e' definita positiva: risoluzione con Cholesky (LLT).\n";
		I_maglie = chol.solve(mats.v);
	}
	else
	{
		cout << "A non e' definita positiva: ripiego sul Gradiente Coniugato.\n";
		Eigen::VectorXd x0 = Eigen::VectorXd::Zero(mats.B.cols()); // punto di partenza: vettore zero
		Eigen::ConjugateGradient<Eigen::MatrixXd, Eigen::Lower | Eigen::Upper> cg;
		cg.compute(A);
		I_maglie = cg.solveWithGuess(mats.v, x0);
	}

	Eigen::VectorXd I_rami = mats.B * I_maglie;
	Eigen::VectorXd V_rami = mats.R * I_rami;

	for (const auto &arco : graph.all_edges())
	{
		Component<NodeType> comp = parser.find_component(arco.from(), arco.to());

		// stampiamo solo per le R, ignorando le V
		if (comp.type == 'R')
		{
			unsigned int riga = graph.edge_number(arco);
			double i_branch = I_rami(riga);
			double v_branch = V_rami(riga);

			// togliamo i -0.0
			if (std::abs(i_branch) < 1e-10)
				i_branch = 0.0;
			if (std::abs(v_branch) < 1e-10)
				v_branch = 0.0;

			cout << comp.id << ": V = " << v_branch << " volts \tI = " << i_branch << " amps\n";
		}
	}

	return 0;
}