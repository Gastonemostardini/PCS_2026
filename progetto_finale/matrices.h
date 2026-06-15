#pragma once
#include <iostream>
#include <list>
#include <map>
#include <cstdint>
#include <Eigen/Dense>
#include "edge.h"
#include "graph.h"
#include "cycles.h"
#include "input.hpp"

// Matrici del sistema delle correnti di maglia (metodo delle maglie).
struct CircuitMatrices
{
	Eigen::MatrixXd B; // incidenza rami x maglie
	Eigen::MatrixXd R; // resistenze rami x rami
	Eigen::VectorXd v; // generatori per maglia
};

// Costruisce le matrici B, R e v a partire dal grafo del circuito, dalla base
// delle maglie e dai componenti letti dal parser:
//   B(i,j) = +1 / -1   verso del ramo resistivo i nella maglia j 0 in caso di non presenza
//   R(i,i) = valore della resistenza sul ramo i
//   v(j)   = somma (con segno) dei generatori lungo la maglia j
template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T> && Printable<T>
CircuitMatrices build_matrices(const Graph<T, EdgeT> &graph,
							   const std::list<Cycles<T, EdgeT>> &base_maglie,
							   const input<T> &parser)
{
	int m = graph.all_edges().size(); // # rami
	int k = base_maglie.size();		  // # maglie

	CircuitMatrices mats;
	mats.B = Eigen::MatrixXd::Zero(m, k);
	mats.R = Eigen::MatrixXd::Zero(m, m);
	mats.v = Eigen::VectorXd::Zero(k);

	// std::cout << " Matrice incidenza B (rami x maglie):\t" << mats.B.rows() << "x" << mats.B.cols() << "\n";
	// std::cout << " Matrice resistenza R (rami x rami): \t" << mats.R.rows() << "x" << mats.R.cols() << "\n";
	// std::cout << " Vettore generatori v (Maglie x 1):  \t" << mats.v.rows() << "x" << mats.v.cols() << "\n";

	// R
	for (const auto &arco : graph.all_edges())
	{
		std::size_t riga = graph.edge_number(arco);
		Component<T> comp = parser.find_component(arco.from(), arco.to());

		if (comp.type == 'R')
		{
			mats.R(riga, riga) = comp.value;
		}
	}

	std::size_t colonna_maglia = 0;
	for (auto ciclo : base_maglie)
	{
		// diamo un verso di percorrenza alla maglia per kirchhoff
		// i versi sono solo +1 / 0 / -1, quindi basta un int8_t
		std::map<EdgeT, std::int8_t> orientamento = orienta_maglia(ciclo, graph);

		for (const auto &arco : graph.all_edges())
		{
			// se questo ramo fa parte della maglia corrente
			if (ciclo.is_active(arco))
			{
				std::size_t riga_ramo = graph.edge_number(arco);
				Component<T> comp = parser.find_component(arco.from(), arco.to());

				std::int8_t verso_ciclo = orientamento[arco];
				if (comp.type == 'R')
				{
					mats.B(riga_ramo, colonna_maglia) = verso_ciclo;
				}
				// se su questo ramo c'è un generatore, gestiamo i segni
				// nota: il primo meno è della legge di kirchhoff
				else if (comp.type == 'V')
				{
					std::int8_t verso_componente = (arco.from() == comp.nodeA) ? 1 : -1;
					mats.v(colonna_maglia) -= comp.value * verso_ciclo * verso_componente;
				}
			}
		}
		colonna_maglia++; // passiamo ad analizzare la maglia successiva
	}

	return mats;
}
