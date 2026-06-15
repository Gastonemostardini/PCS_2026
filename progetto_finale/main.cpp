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

	int m = graph.all_edges().size(); // # rami
	int k = base_maglie.size();		  // # maglie

	Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, k);
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);
	Eigen::VectorXd v = Eigen::VectorXd::Zero(k);

	cout << " Matrice incidenza B (rami x maglie):\t" << B.rows() << "x" << B.cols() << "\n";
	cout << " Matrice resistenza R (rami x rami): \t" << R.rows() << "x" << R.cols() << "\n";
	cout << " Vettore generatori v (Maglie x 1):  \t" << v.rows() << "x" << v.cols() << "\n";

	// R
	for (const auto &arco : graph.all_edges())
	{
		unsigned int riga = graph.edge_number(arco);
		Component<NodeType> comp = parser.find_component(arco.from(), arco.to());

		if (comp.type == 'R')
		{
			R(riga, riga) = comp.value;
		}
	}

	int colonna_maglia = 0;

	for (auto ciclo : base_maglie)
	{

		// diamo un verso di percorrenza alla maglia per kirchhoff
		// i versi sono solo +1 / 0 / -1, quindi basta un int8_t
		std::map<EdgeType, std::int8_t> orientamento = orienta_maglia(ciclo, graph);

		for (const auto &arco : graph.all_edges())
		{

			// se questo ramo fa parte della maglia corrente
			if (ciclo.is_active(arco))
			{
				unsigned int riga_ramo = graph.edge_number(arco);
				Component<NodeType> comp = parser.find_component(arco.from(), arco.to());

				int8_t verso_ciclo = orientamento[arco];
				if (comp.type == 'R')
				{
					B(riga_ramo, colonna_maglia) = verso_ciclo;
				}
				// se su questo ramo c'è un generatore, gestiamo i segni
				// nota: il primo meno è della legge di kirchhoff
				else if (comp.type == 'V')
				{
					int8_t verso_componente = (arco.from() == comp.nodeA) ? 1 : -1;
					v(colonna_maglia) -= comp.value * verso_ciclo * verso_componente;
				}
			}
		}
		colonna_maglia++; // passiamo ad analizzare la maglia successiva
	}

	cout << "\n=== soluzione sistema ===\n";

	// risoluzione sistema
	Eigen::MatrixXd A = B.transpose() * R * B;
	Eigen::VectorXd I_maglie;

	// Tentiamo la decomposizione di Cholesky (LLT): riesce (info() == Success)
	// solo se A e' simmetrica definita positiva. In tal caso risolviamo in modo
	// diretto e stabile; altrimenti ripieghiamo sul Gradiente Coniugato.
	Eigen::LLT<Eigen::MatrixXd> chol(A);
	if (chol.info() == Eigen::Success)
	{
		cout << "A e' definita positiva: risoluzione con Cholesky (LLT).\n";
		I_maglie = chol.solve(v);
	}
	else
	{
		cout << "A non e' definita positiva: ripiego sul Gradiente Coniugato.\n";
		Eigen::VectorXd x0 = Eigen::VectorXd::Zero(k); // punto di partenza: vettore zero
		Eigen::ConjugateGradient<Eigen::MatrixXd, Eigen::Lower | Eigen::Upper> cg;
		cg.compute(A);
		I_maglie = cg.solveWithGuess(v, x0);
	}

	Eigen::VectorXd I_rami = B * I_maglie;
	Eigen::VectorXd V_rami = R * I_rami;

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