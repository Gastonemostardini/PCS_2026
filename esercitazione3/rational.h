#pragma once

#include <cmath>
#include <concepts>
#include <numeric>
using namespace std;

/**
 * @brief Classe template per numeri razionali.
 *
 * Rappresenta una frazione num/den sempre in forma ridotta
 * con il segno sul numeratore.
 * 
 * @tparam I Tipo intero 
 * 
 * @note Casi speciali:
 *		- **NaN**: rappresentato come 0/0 (denominatore zero con numeratore zero)
 *		- **+Inf**: rappresentato come 1/0
 *		- **-Inf**: rappresentato come -1/0
 *		- Qualsiasi operazione che coinvolge NaN restituisce NaN
 *		- Divisione per zero produce ±Inf
 */
template<typename I> requires integral<I>
class Rational
{
	I num_;
	I den_;

	/** @brief Pretty print della funzione. */
	friend ostream& operator<<(ostream& os, const Rational<I>& obj) {
		if (obj.den_ == 1)
			os << obj.num_;
		else
			os << obj.num_ << "/" << obj.den_;
		return os;
	}

	/** @brief Riduce la frazione ai minimi termini usando il MCD */
	void minimize();
	/** @brief Riduce la frazione ai minimi e ordina il segno */
	void normalize();
	int sgn() const;
	bool isNan() const;
	bool isInf() const;

	void setNan();
	void setInf();

public:
	/* Costruttori */
	/** @brief Costruttore defoult */
	Rational();
	/** @brief Costruttore con parametri numeratore e denominatore */
	Rational(I num, I den);
	/** @brief Costruttore con solo numeratore */
	Rational(I num);

	/* Overloading degli operatori */
	Rational<I>& operator+=(const Rational<I>& other);
	Rational<I>& operator-=(const Rational<I>& other);
	Rational<I>& operator*=(const Rational<I>& other);
	Rational<I>& operator/=(const Rational<I>& other);
	Rational<I> operator+(const Rational<I>& other) const;
	Rational<I> operator-(const Rational<I>& other) const;
	Rational<I> operator*(const Rational<I>& other) const;
	Rational<I> operator/(const Rational<I>& other) const;
	Rational<I> operator-() const;

	/* Getter */
	I num() const;
	I den() const;
};

