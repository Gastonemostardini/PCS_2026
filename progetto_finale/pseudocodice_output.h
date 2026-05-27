//pseudo codice per risoluzione del sistema lineare e output
#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>

//creo funzione che faccia entrambe le cose e come output
//come input metto le matrici che mi servono per la risoluzione del sistema
void
output_function(Eigen::MatrixXd B, Eigen::MatrixXd R, Eigen::VectorXd v){
	
	//risoluzione sistema
	Eigen::MatrixXd A = B.transpose()*R*B;
	Eigen::VectorXd I = A.llt().solve(v); 
	
	//calcolo delle Vr (cadute di tensione dei resistori)
	Eigen::VectorXd Vr = R*B*I;
	Eigen::VectorXd Ir = B*I;
	
	//scrittura dell'output
	int dim = R.rows();
	for (int i = 0; i < dim; i++) {
        std::cout << "R" << i + 1 << " : V = " << Vr(i) << " volts, I = " << Ir(i) << " amps" << "\n";
    }
	
}