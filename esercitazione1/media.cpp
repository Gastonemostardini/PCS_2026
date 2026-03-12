#include <iostream>
#include <fstream>

using namespace std;

double media(double valori[], int n) {
	double somma = 0;

	for (int i = 0; i < n; i++) {
		somma += valori[i];
	}

	return somma / n;
}

int main(int argc, const char* argv[])
{
	if (!argv[1]) {
		cout << "ERRORE: argomento mancante;\n";
		exit(1);
	}

	string location = "";
	double temps[4];
	ifstream MyReadFile(argv[1]);

	if (MyReadFile.fail() || !MyReadFile.is_open()) {
		cout << "ERRORE: file inesistente;\n";
		exit(1);
	}

	while (!MyReadFile.eof()) {
		location = "";
		MyReadFile >> location >> temps[0] >> temps[1] >> temps[2] >> temps[3];
		if (location == "") {
			break;
		}

		cout << location;

		// OCD personale
		if (location.length() < 8){
			cout << "\t\t";
		} else {
			cout << "\t";
		}

		cout << media(temps, 4) << "\n";
	}

	MyReadFile.close();

	return 0;
}