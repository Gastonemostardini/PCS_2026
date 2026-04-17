#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>
#include "ChessBoard.hpp"
#include "vectors_fun.hpp"

using namespace std;
int main(int argc, const char* argv[]) {
    const size_t N = (argc >= 2) ? std::stoi(argv[1]) : 8;

    vector<vector<int>> celle;
	ChessBoard board = ChessBoard(N);
	vector<int> archi(N * N, 0);
	
	initializeArchi(archi, N);

    int i = 0;
    int j = 0;
    cout << "i, j = " << i << ", " << j << endl;
	board.setCell(i, j, "♘"); 

    int ni = i;
    int nj = j;

    auto vicini = horseMovableCells(N, i, j);
    for (const auto& v : vicini) {
        archi[pos(N, v[0], v[1])] -= 1;
    }

    for (size_t a = 0; a < N * N - 1; a++) {
        celle = horseMovableCells(N, i, j);

        std::vector<std::vector<int>> celle_libere;
        for (const auto& c : celle) {
            if (board.getCell(c[0], c[1]) == "") {
                celle_libere.push_back(c);
            }
        }

        if (celle_libere.empty()) {
            cout << "vicolo cieco alla mossa " << a << "\n";
            break;
        }

        celle = celle_libere;

        std::sort(celle.begin(), celle.end(),
            [&archi, N](const std::vector<int>& c, const std::vector<int>& b) {
                return archi[pos(N, c[0], c[1])] < archi[pos(N, b[0], b[1])];
            });

        ni = celle[0][0];
        nj = celle[0][1];

        // Muovi il cavallo
        board.setCell(i, j, "♘");
        i = ni;
        j = nj;
        board.setCell(i, j, "♞");

        vicini = horseMovableCells(N, i, j);
        for (const auto& v : vicini) {
            archi[pos(N, v[0], v[1])] -= 1;
        }

        cout << board;
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    board.setCell(i, j, "♘");
    cout << board;

	return 0;
}
