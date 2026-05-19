#include <iostream>
#include "Eigen/Eigen"

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
	Eigen::VectorXd res = -(A * x - b);
		
	double res_norm_0 = res.norm();

	const unsigned int it_max = 10000;
	unsigned int it = 0;
	const double res_tol = 1.0e-12;
	
	while (it < it_max && res.norm() / res_norm_0 > res_tol) {
		const double alpha_k = ((res.transpose() * res) / (res.transpose() * A * res)).value();
		x = x + alpha_k * res;
		res = res - alpha_k * A * res;
		res_norm_0 = res.norm();
		++it;
	}

	const auto err_abs = (x - x_ex).norm();
	const auto err_rel = (x_ex.norm() < tol) ? (x - x_ex).norm()
											 : (x - x_ex).norm() / x_ex.norm();

	cout << x << endl;
	cout << x_ex << endl;
	cout << it << endl;
	cout << err_rel << endl;
	cout << err_abs << endl;

	return 0;
}
