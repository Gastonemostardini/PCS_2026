#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include "input.hpp"
#include "edge.h"
#include "graph.h"
#include "tree.h"
#include "search_graph.h"
#include "cycles.h"
#include "weighed_edge.h"
#include "weighed_graph.h"
#include "gradiente.h"
#include "functions.h"
#include "Eigen/Eigen"
#include "dfs_project.h"

using namespace std;


// orientare le maglie in un percorso con versi (+1 / -1)
std::map<Edge<int>, double> orienta_maglia(Cycles<int, Edge<int>>& ciclo, const Graph<int>& graph) {
	std::map<Edge<int>, double> segni;
	std::vector<Edge<int>> rami;
	
	
	for (auto e : graph.all_edges()) {   // estraiamo solo i rami di questa maglia
		if (ciclo.is_active(e)) rami.push_back(e);
	}
	if (rami.empty()) return segni;

	
	std::map<int, std::vector<Edge<int>>> adj;      // mappa di adiacenza per i rami del ciclo
	for (auto e : rami) {
		adj[e.from()].push_back(e);
		adj[e.to()].push_back(e);
	}

	// percorriamo il ciclo
	int current_node = rami[0].from();
	Edge<int> current_edge = rami[0];
	int start_node = current_node;

	while (true) {
		if (current_edge.from() == current_node) {
			segni[current_edge] = 1.0;
			current_node = current_edge.to();
		} else {
			segni[current_edge] = -1.0;
			current_node = current_edge.from();
		}

		if (current_node == start_node) break;

		// prossimo ramo
		for (auto e : adj[current_node]) {
			if (segni.find(e) == segni.end()) {
				current_edge = e;
				break;
			}
		}
	}
	return segni;
}


int main() {
	cout << "=== Lettura e pulizia del circuito ===\n";
	input parser;
	string nome_file = "testinput.txt";

	if (!parser.parse_file(nome_file)) {
		return EXIT_FAILURE;
	}
	
	if (!parser.check_validity()) {
		return EXIT_FAILURE;
	}

	
	parser.clean_graph();              //  rami aperti e isolati
	parser.print_status();
	Graph<int> graph = parser.get_graph();  // otteniamo il grafo
	
	cout << "\n=== esrtazione maglie con De Pina e DFS ===\n";
	
	// De pina
	std::list<Cycles<int, Edge<int>>> maglie_depina = de_pina(graph);
	cout << "De Pina - maglie fondamentali trovate: " << maglie_depina.size() << "\n";

	// Dfs
	int nodo_radice = *graph.all_nodes().begin();
	TreeGraph<int, Edge<int>> albero_dfs = recursive_dfs(graph, nodo_radice);
	std::list<Cycles<int, Edge<int>>> maglie_dfs = find_minimal_cycles(graph, albero_dfs);
	cout << "DFS - maglie fondamentali trovate: " << maglie_dfs.size() << "\n";

	
	
	bool usa_de_pina = true;    // cambiare 'usa_de_pina' in 'false' per usare DFS
	
	std::list<Cycles<int, Edge<int>>> base_maglie;

	if (usa_de_pina) {
		cout << "-> SCELTA: Il sistema verra' risolto usando le maglie di DE PINA.\n";
		base_maglie = maglie_depina;
	} else {
		cout << "-> SCELTA: Il sistema verra' risolto usando le maglie della DFS.\n";
		base_maglie = maglie_dfs;
	}
	

	cout << "\n=== Matrici ===\n";
	
	
	int m = graph.all_edges().size(); // # rami
	int k = base_maglie.size();       // # maglie

	Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, k);
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);
	Eigen::VectorXd v = Eigen::VectorXd::Zero(k);

	cout << "  - Matrice incidenza B (rami x maglie): " << m << "x" << k << "\n";
	cout << "  - Matrice resistenza R (rami x rami):  " << m << "x" << m << "\n";
	cout << "  - Vettore generatori v (Maglie x 1):   " << k << "x1\n";

	cout << "\n=== Matrici ===\n";
	
	// R
	for (const auto& arco : graph.all_edges()) {
		unsigned int riga = graph.edge_number(arco);
		Component comp = parser.find_component(arco.from(), arco.to());
		
		if (comp.type == 'R') {
			R(riga, riga) = comp.value; 
		}
	}
	
	int colonna_maglia = 0; 
	
	//  auto&  perché is_active() in cycles.h non è const
	for (auto& ciclo : base_maglie) {
		
		// diamo un verso di percorrenza alla maglia per kirchhoff
		std::map<Edge<int>, double> orientamento = orienta_maglia(ciclo, graph);

		for (const auto& arco : graph.all_edges()) {
			
			// se questo ramo fa parte della maglia corrente
			if (ciclo.is_active(arco)) {
				unsigned int riga_ramo = graph.edge_number(arco);
				Component comp = parser.find_component(arco.from(), arco.to());
				
				double verso_percorrenza = orientamento[arco];
				B(riga_ramo, colonna_maglia) = verso_percorrenza;

				// se su questo ramo c'è un generatore, gestiamo i segni
				if (comp.type == 'V') {
					int nodo_partenza = (verso_percorrenza == 1.0) ? arco.from() : arco.to();
					int nodo_arrivo   = (verso_percorrenza == 1.0) ? arco.to() : arco.from();

					// salita di potenziale
					if (nodo_partenza == comp.nodeB && nodo_arrivo == comp.nodeA) {
						v(colonna_maglia) += comp.value; 
					} 
					// discesa di potenziale
					else if (nodo_partenza == comp.nodeA && nodo_arrivo == comp.nodeB) {
						v(colonna_maglia) -= comp.value; 
					}
				}
			}
		}
		colonna_maglia++; // passiamo ad analizzare la maglia successiva
	}

	cout << "Matrici create con successo.\n";

	cout << "\n=== soluzione sistema ===\n";
	
	
	// risoluzione sistema 
	Eigen::MatrixXd A = B.transpose() * R * B;
	Eigen::VectorXd x0 = Eigen::VectorXd::Zero(k);  // punto di partenza: vettore zero
	Eigen::VectorXd I_maglie;
	gradiente_cognugato(A, x0, v, I_maglie);
	Eigen::VectorXd I_rami = B * I_maglie;
	Eigen::VectorXd V_rami = R * I_rami;

	for (const auto& arco : graph.all_edges()) {
		Component comp = parser.find_component(arco.from(), arco.to());
		
		// stampiamo solo per le R, ignorando le V
		if (comp.type == 'R') {
			unsigned int riga = graph.edge_number(arco);
			double i_branch = I_rami(riga);
			double v_branch = V_rami(riga);


			// togliamo i -0.0 
			if (std::abs(i_branch) < 1e-10) i_branch = 0.0;
			if (std::abs(v_branch) < 1e-10) v_branch = 0.0;

			cout << comp.id << " : V = " << v_branch << " volts, I = " << i_branch << " amps\n";
		}
	}
	
	return 0;
}