#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>
#include <set>
#include "ChessBoard.hpp"
#include "vectors_fun.hpp"


using namespace std;
int main(int argc, const char* argv[]) {
	size_t N = 8;
	for (int k = 1; k < argc; ++k) {
		std::string arg = argv[k];
		if (arg == "--color")
			isColor = true;
		else if (arg == "--overlay")
			isOverlay = true;
		else
			N = std::stoi(arg);

	}

	vector<vector<int>> celle;
	ChessBoard board = ChessBoard(N);
	vector<int> archi(N * N, 0);

	initializeArchi(archi, N);

	int i = 0;
	int j = 0;

	print(board, archi);

	cin.get();

	cout << "i, j = " << i << ", " << j << endl;
	board.setCell(i, j, "♞"); 

	int ni = i;
	int nj = j;

	// Primo aggiornamento nodi vicini
	auto vicini = horseMovableCells(N, i, j);
	for (const auto& v : vicini) {
		archi[pos(N, v[0], v[1])] -= 1;
	}

	print(board, archi, vicini);

	for (size_t a = 0; a < N * N - 1; a++) {

		// attesa input utente 
		cin.get();

		std::vector<std::vector<int>> celle_libere;

		for (const auto& c : vicini) {
			if (board.getCell(c[0], c[1]) == "") {
				celle_libere.push_back(c);
			}
		}

		if (celle_libere.empty()) {
			cout << "vicolo cieco alla mossa " << a << "\n";
			break;
		}

		vicini = celle_libere;

		std::sort(vicini.begin(), vicini.end(),
			[&archi, N](const std::vector<int>& c, const std::vector<int>& b) {
				return archi[pos(N, c[0], c[1])] < archi[pos(N, b[0], b[1])];
			});

		ni = vicini[0][0];
		nj = vicini[0][1];

		// Muovi il cavallo
		board.setCell(i, j, "♘");
		i = ni;
		j = nj;
		board.setCell(i, j, "♞");

		// Calcolo nuove celle vicine
		vicini = horseMovableCells(N, i, j);
		
		// Diminuzione del valore dei nodi;
		// La nomeclatura è un po' strana: il valore rappresenta gli archi del nodo in poszione (i, j)
		for (const auto& v : vicini) {
			archi[pos(N, v[0], v[1])] -= 1;
		}

		// Print della scacchiera	
		print(board, archi, vicini);
	}

	board.setCell(i, j, "♘");
	cout << board;

	return 0;
}
