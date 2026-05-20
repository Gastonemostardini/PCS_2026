#include <iostream>
#include "Eigen/Eigen"
#include "gradiente.h"

using namespace std;
int main(int argc, char** argv)
{
	const double tol = 1.0e-15;
	unsigned int n = 10;
	Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n);

	if (abs(B.determinant()) < tol)
		return -1;

	Eigen::MatrixXd A = B.transpose() * B;
	Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);

	Eigen::VectorXd b = A * x_ex;

	Eigen::VectorXd x = Eigen::VectorXd::Zero(n);

	Eigen::VectorXd res = Eigen::VectorXd::Zero(n);
	cout << res << endl;
	gradiente_cognugato(A, x, b, res);
	cout << res << endl;

	return 0;
}
