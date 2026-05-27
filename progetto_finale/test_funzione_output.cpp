//test funzione output
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include "pseudocodice_output.h"

int main(){	
	Eigen::MatrixXd B(3,2);
	B<< 1,1,
		1,-1,
		0,1;
	
	Eigen::MatrixXd R=Eigen::MatrixXd::Zero(3,3);
	R.diagonal()<<10,20,30;
	
	Eigen::VectorXd v(2);
	v<< 10,-20;

	output_function(B,R,v);
	return 0;
}