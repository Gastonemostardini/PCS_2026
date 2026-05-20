#include <iostream>
#include "Eigen/Eigen"
#include "gradiente.h"

int test_gradiente() {
	const double tol = 1.0e-11;
	double test_tol = 1;
	Eigen::MatrixXd B;
	Eigen::MatrixXd A;
	Eigen::VectorXd x_ex;
	Eigen::VectorXd b;
	Eigen::VectorXd x;
	Eigen::VectorXd res;
	for (int n = 1; n < 50; n++){
		B = Eigen::MatrixXd::Random(n, n);

		if (abs(B.determinant()) < tol)
			return -1;

		A = B.transpose() * B;

		//Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
		//const double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size() - 1);

		//test_tol = tol * cond;

		x_ex = Eigen::VectorXd::Ones(n);

		b = A * x_ex;

		x = Eigen::VectorXd::Zero(n);

		res = Eigen::VectorXd::Zero(n);

		std::cout << n << std::endl;
		gradiente_cognugato(A, x, b, res, 10000, 10e-13);
		//std::cout << (res - x_ex).norm() << std::endl;

		if ((res - x_ex).norm() > test_tol)
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main() {
    return test_gradiente();
}