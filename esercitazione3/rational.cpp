#include "rational.h"

template<typename I> requires integral<I>
void Rational<I>::operator+=(const Rational<I>& other)
{
	if (isNan() || other.isNan())
	{
		setNan();
		return;
	}

	if (isInf() != other.isInf())
	{
		num_ = (isInf()) ? sgn() : other.sgn();
		den_ = 0;
		return;
	}

	if (isInf() && other.isInf())
	{
		if (sgn() == other.sgn())
			setInf();
		else
			setNan();
		return;
	}

	num_ = num_ * other.den_ + other.num_ * den_;
	den_ = den_ * other.den_;
	normalize();
}

template<typename I> requires integral<I>
void Rational<I>::operator-=(const Rational<I>& other)
{
	if (isNan() || other.isNan()) {
		setNan();
		return;
	}

	if (isInf() != other.isInf())
	{
		num_ = (isInf()) ? sgn() : -other.sgn();
		den_ = 0;
		return;
	}

	if (isInf() && other.isInf())
	{
		if (sgn() == other.sgn())
			setNan();
		else
			setInf();
		return;
	}

	num_ = num_ * other.den_ - other.num_ * den_;
	den_ = den_ * other.den_;
	normalize();
}

template<typename I> requires integral<I>
void Rational<I>::operator*=(const Rational<I>& other)
{
	if (isNan() || other.isNan()) {
		setNan();
		return;
	}

	num_ = num_ * other.num_;
	den_ = den_ * other.den_;
	normalize();
}

template<typename I> requires integral<I>
void Rational<I>::operator/=(const Rational<I>& other)
{
	if (isNan() || other.isNan() || isInf() || other.isInf()) {
		setNan();
		return;
	}

	num_ = num_ * other.den_;
	den_ = den_ * other.num_;
	normalize();
}

template<typename I> requires integral<I>
Rational<I> Rational<I>::operator+(const Rational<I>& other) const {
	Rational<I> result(*this);
	result += other;
	return result;
}

template<typename I> requires integral<I>
Rational<I> Rational<I>::operator-(const Rational<I>& other) const {
	Rational<I> result(*this);
	result -= other;
	return result;
}

template<typename I> requires integral<I>
Rational<I> Rational<I>::operator*(const Rational<I>& other) const {
	Rational<I> result(*this);
	result *= other;
	return result;
}

template<typename I> requires integral<I>
Rational<I> Rational<I>::operator/(const Rational<I>& other) const {
	Rational<I> result(*this);
	result /= other;
	return result;
}

template<typename I> requires integral<I>
Rational<I>::Rational()
{
	num_ = 0;
	den_ = 1;
}

template<typename I> requires integral<I>
Rational<I>::Rational(I num)
{
	num_ = num;
	den_ = 1;
}

template<typename I> requires integral<I>
Rational<I>::Rational(I num, I den)
{
	num_ = num;
	den_ = den;
	normalize();
}

template<typename I> requires integral<I>
void Rational<I>::minimize()
{
	I g = gcd(num_, den_);
	num_ /= g;
	den_ /= g;
}

template<typename I> requires integral<I>
void Rational<I>::normalize()
{
	if (isNan()) {
		setNan();
		return;
	}

	if (isInf()) {
		setInf();
		return;
	}

	minimize();
	if (den_ < 0)
	{
		num_ = -num_;
		den_ = -den_;
	}
}

template<typename I> requires integral<I>
I Rational<I>::num() const
{
	return num_;
}

template<typename I> requires integral<I>
I Rational<I>::den() const
{
	return den_;
}

template<typename I> requires integral<I>
bool Rational<I>::isNan() const {
	return !(num_ || den_);
}

template<typename I> requires integral<I>
bool Rational<I>::isInf() const {
	return (num_ != 0 && den_ == 0);
}

template<typename I> requires integral<I>
int Rational<I>::sgn() const {
	return (num_ > 0) ? 1 : -1;
}

template<typename I> requires integral<I>
void Rational<I>::setNan() {
	num_ = 0;
	den_ = 0;
	return;
}

template<typename I> requires integral<I>
void Rational<I>::setInf() {
	num_ = sgn();
	den_ = 0;
	return;
}