#pragma once
#include <iostream>
#include "Eigen/Eigen"

void gradiente_cognugato(Eigen::MatrixXd A, Eigen::VectorXd x, Eigen::VectorXd b,
						Eigen::VectorXd& sol, const unsigned int it_max = 10000, 
						const double res_tol = 1.0e-12) {
	Eigen::VectorXd res = b - A * x;
	Eigen::VectorXd passo = res;

	unsigned int it = 0;

	const double r0_norm = res.norm();  // res = b - A*x calcolato sopra
	if (r0_norm < res_tol) { sol = x; return; }

	while (it < it_max && res.norm() > res_tol * r0_norm) {
		const double alpha = ((passo.transpose() * res) / (passo.transpose() * A * passo)).value();
		x = x + alpha * passo;
		res = b - A * x;
		const double beta = ((passo.transpose() * A * res) / (passo.transpose() * A * passo)).value();
		passo = res - beta * passo;
		++it;
	}

	std::cout << it << std::endl;
	sol = x;
}