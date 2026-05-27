#pragma once
#include <vector>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& obj) {
    if (obj.size() == 0) {
        os << "vettore vuoto";
    }
    os << "[";
    for (std::size_t i = 0; i < obj.size(); i++) {
        os << obj[i];
        if (i < obj.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& obj) {
    if (obj.empty()) {
        os << "vettore vuoto";
        return os;
    }
    os << "(";
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (it != obj.begin())
            os << ", ";
        os << *it;
    }
    os << ")";
    return os;
}

bool operator*(std::vector<bool> a, std::vector<bool> b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    int sum = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] && b[i])
            sum++;
    }

    return sum % 2;

}

std::vector<bool> operator^(std::vector<bool> a, std::vector<bool> b) {
	if (a.size() != b.size()) {
		throw std::invalid_argument("Vector MUST be the same size.");
	}

	std::vector<bool> res(a.size());
	for (size_t i = 0; i < a.size(); i++){
		res[i] = a[i] ^ b[i];
	}
	return res;
}

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
