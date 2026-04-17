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
		cerr << "ERRORE: argomento mancante;\n";
		exit(1);
	}

	string location = "";
	double temps[4];
	ifstream myFile(argv[1]);

	if (myFile.fail() || !myFile.is_open()) {
		cerr << "ERRORE: non Ã stato possibile aprire il file;\n";
		exit(1);
	}

	while (!myFile.eof()) {
		location = "";
		myFile >> location >> temps[0] >> temps[1] >> temps[2] >> temps[3];
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

	myFile.close();

	return 0;
}
