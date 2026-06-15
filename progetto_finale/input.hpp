#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include "graph.h"

struct Component
{
	std::string id; // R1, R2, V2, ...
	char type;		// divisione in R e V
	double value;	// valore
	int nodeA;		// nodo di partenza
	int nodeB;		// nodo di arrivo

	// ordine
	int getMinNode() const { return std::min(nodeA, nodeB); }
	int getMaxNode() const { return std::max(nodeA, nodeB); }
};

std::ostream &operator<<(std::ostream &os, const Component &comp)
{
	os << comp.id << " (" << comp.value
	<< ") connette " << comp.nodeA << " e " << comp.nodeB;
	
	return os;
}

class input
{
private:
	std::vector<Component> all_components;
	std::set<int> unique_nodes;

	// grado di ciascun nodo
	std::map<int, int> compute_degrees() const
	{
		std::map<int, int> degrees;
		// d(a) = 0
		for (int node : unique_nodes)
		{
			degrees[node] = 0;
		}
		// sum edges
		for (const auto &comp : all_components)
		{
			degrees.at(comp.nodeA)++;
			degrees.at(comp.nodeB)++;
		}
		return degrees;
	}

public:
	input() = default;

	// 1. parser no spazi e righe vuote
	bool parse_file(const std::string &filename)
	{
		std::ifstream ifs(filename);
		if (!ifs.is_open())
		{
			std::cerr << "Errore: Impossibile aprire il file " << filename << "\n";
			return false;
		}

		std::string id;
		double value;
		int nA, nB;

		// approccio robusto visto a lezione
		while (ifs >> id >> value >> nA >> nB)
		{
			// ignora i componenti in cortocircuito su se stessi
			if (nA == nB)
			{
				std::cerr << "Attenzione: ignorato il componente " << id
						  << " perché è un auto-arco (" << nA << "->" << nB << ")\n";
				continue; //  riga successiva del file
			}
			char type = id[0]; // estrae R o V

			Component comp{id, type, value, nA, nB};
			all_components.push_back(comp);

			unique_nodes.insert(nA);
			unique_nodes.insert(nB);
		}

		ifs.close();
		return true;
	}

	// 2. no parallelo
	bool check_validity() const
	{
		// numero componenti  tra due nodi
		// Chiave: pair(nodo_min, nodo_max), Valore: conteggio
		std::map<std::pair<int, int>, int> connections;

		for (const auto &comp : all_components)
		{
			std::pair<int, int> edge = {comp.getMinNode(), comp.getMaxNode()};
			connections[edge]++;

			if (connections[edge] > 1)
			{
				std::cerr << "Errore: questa configurazione non è consentita "
						  << edge.first << " e " << edge.second << "!\n";
				return false; // no parallelo
			}
		}
		return true;
	}

	// 3. no nodi isolati e foglie
	void clean_graph()
	{
		bool graph_changed = true;

		while (graph_changed)
		{
			graph_changed = false;
			std::map<int, int> degrees = compute_degrees();
			std::vector<Component> active_components;

			for (const auto &comp : all_components)
			{
				// un componente sopravvive solo se entrambi i suoi nodi hanno deg >1
				if (degrees[comp.nodeA] > 1 && degrees[comp.nodeB] > 1)
				{
					active_components.push_back(comp);
				}
				else
				{
					// rifare check
					graph_changed = true;
				}
			}

			// aggiornare
			all_components = active_components;

			// nodi unici
			unique_nodes.clear();
			for (const auto &comp : all_components)
			{
				unique_nodes.insert(comp.nodeA);
				unique_nodes.insert(comp.nodeB);
			}
		}
	}

	const std::vector<Component> &get_components() const { return all_components; }
	const std::set<int> &get_nodes() const { return unique_nodes; }

	// 4. generazione del grafo
	Graph<int> get_graph() const
	{
		Graph<int> circuito(unique_nodes);

		//  archi
		for (const auto &comp : all_components)
		{
			circuito.add_edge(comp.nodeA, comp.nodeB);
		}

		return circuito;
	}

	// 5. recupero componente tramite nodi
	Component find_component(int nA, int nB) const
	{
		int minN = std::min(nA, nB);
		int maxN = std::max(nA, nB);
		for (const auto &comp : all_components)
		{
			if (comp.getMinNode() == minN && comp.getMaxNode() == maxN)
			{
				return comp;
			}
		}
		// Ritorna un componente nullo se non presente
		return Component{"", ' ', 0.0, 0, 0};
	}

	void print_status() const
	{
		std::cout << "--- Stato Corrente del Circuito ---\n";
		std::cout << "Nodi attivi: ";
		for (int n : unique_nodes)
			std::cout << n << " ";
		std::cout << "\n";
		std::cout << "Componenti attivi:\n";
		for (const auto &comp : all_components)
		{
			std::cout << comp << "\n";
		}
		std::cout << "-----------------------------------\n";
	}
};