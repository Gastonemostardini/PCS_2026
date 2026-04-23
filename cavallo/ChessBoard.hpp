#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <set>

inline bool isColor = false;
inline bool isOverlay = false;

class ChessBoard {
	int N_ = 8;
	std::vector<std::string> v;

	friend std::ostream& operator<<(std::ostream& os, const ChessBoard& obj) {
		for (int r = 0; r < obj.N_; r++) {
			for (int c = 0; c < obj.N_; c++) {
				if (obj.getCell(r, c) != "")
					os << obj.getCell(r, c);
				else {
					if (r % 2 != c % 2)
						os << "■";
					else
						os << "□";
				}
				os << ' ';
			}
			os << '\n';
		}
		os << '\n';
		return os;
	}

	void print(std::ostream& os, const std::vector<std::vector<int>>& overlay) const {
		for (int r = 0; r < N_; r++) {
			for (int c = 0; c < N_; c++) {
				if (getCell(r, c) != "")
					os << getCell(r, c);
				else
					os << overlay[r][c];
				os << ' ';
			}
			os << '\n';
		}
		os << '\n';
	}


	int pos(int r, int c) const {
		return r * N_ + c;
	}

public:
	ChessBoard() {
		N_ = 8;
		v.clear();
		v.resize(N_ * N_, "");
	}

	ChessBoard(int N) {
		N_ = N;
		v.clear();
		v.resize(N_ * N_, "");
	}
	
	void clear() {
		//std::cout << "[ChessBoard::clear] chiamato, size=" << v.size() << std::endl;
		std::fill(v.begin(), v.end(), "");
	}

	int getDim() const {
		return N_;
	}

	int getCellNumber() const {
		return N_ * N_;
	}

	void printDim() const {
		std::cout << N_ << "x" << N_ << std::endl;
	}

	void validateCell(int r, int c) const {
		if (r < 0 || r >= N_ || c < 0 || c >= N_)
			throw std::out_of_range("ChessBoard: indici ("
				+ std::to_string(r) + ","
				+ std::to_string(c) + ") fuori range");
	}

	void validatePiece(std::string piece) {
		static const std::unordered_set<std::string> validPieces = {
			"\u2654",	// ♔ Re bianco
			"\u2655",	// ♕ Donna bianca
			"\u2656",	// ♖ Torre bianca
			"\u2657",	// ♗ Alfiere bianco
			"\u2658",	// ♘ Cavallo bianco
			"\u2659",	// ♙ Pedone bianco
			"\u265A",	// ♚ Re nero
			"\u265B",	// ♛ Donna nera
			"\u265C",	// ♜ Torre nera
			"\u265D",	// ♝ Alfiere nero
			"\u265E",	// ♞ Cavallo nero
			"\u265F",	// ♟ Pedone nero
			"",			// vuoto sistema con quadratino adeguato

		};

		if (validPieces.find(piece) == validPieces.end()) {
			throw std::invalid_argument("Pezzo non valido: '" + piece +
				"' non è un pezzo degli scacchi Unicode");
		}
	}

	std::string getCell(int r, int c) const {
		validateCell(r, c);
		return (v[pos(r, c)]);
	}

	void setCell(int r, int c, std::string piece) {
		validateCell(r, c);
		validatePiece(piece);
		v[pos(r,c)] = piece;
	}

	void print(std::ostream& os, const std::vector<int>& overlay) const {
		for (int r = 0; r < N_; r++) {
			for (int c = 0; c < N_; c++) {
				if (getCell(r, c) != "")
					os << getCell(r, c);
				else
					os << overlay[pos(r, c)];
				os << ' ';
			}
			os << '\n';
		}
		os << '\n';
	}


	void print(std::ostream& os,
		const std::vector<int>& overlay,
		const std::vector<std::vector<int>>& vicini) const {

		const std::string GIALLO = "\033[33m";
		const std::string RESET = "\033[0m";

		std::set<int> vicini_pos;
		for (const auto& vicinio : vicini)
			vicini_pos.insert(pos(vicinio[0], vicinio[1]));

		for (int r = 0; r < N_; r++) {
			for (int c = 0; c < N_; c++) {
				if (getCell(r, c) != "")
					os << getCell(r, c);
				else {
					bool evidenzia = vicini_pos.count(pos(r, c)) > 0;
					if (evidenzia) os << GIALLO;
					os << overlay[pos(r, c)];
					if (evidenzia) os << RESET;
				}
				os << ' ';
			}
			os << '\n';
		}
		os << '\n';
	}

};


// UTILS
void print(ChessBoard board,
	const std::vector<int>& overlay)
{
	if (isOverlay)
		board.print(std::cout, overlay);
	else
		std::cout << board;
}

void print(ChessBoard board,
	const std::vector<int>& overlay,
	const std::vector<std::vector<int>>& highlight)
{
	if (isColor && isOverlay)
		board.print(std::cout, overlay, highlight);
	else if (isOverlay)
		board.print(std::cout, overlay);
	else
		std::cout << board;
}
