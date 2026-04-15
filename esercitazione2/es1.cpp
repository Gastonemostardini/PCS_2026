#include <iostream>
using namespace std;

int main()
{
	double ad[4] = { 0.0, 1.1, 2.2, 3.3 };
	float af[8] = { 0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7 };
	int ai[3] = { 0, 1, 2 };

	cout << "ad\n";
	for(int i = 0; i < 4; i++)
		cout << &ad[i] << "\n";
	cout << "\n";

	cout << "af\n";
	for (int i = 0; i < 8; i++)
		cout << &af[i] << "\n";
	cout << "\n";

	cout << "ai\n";
	for (int i = 0; i < 3; i++)
		cout << &ai[i] << "\n";

	cout << "----------------------------------------------";

	int x = 1;
	float y = 1.1;

	cout << "\nx: \n";
	cout << x << " ";
	cout << &x << "\n";

	cout << "\ny: \n";
	cout << y << " ";
	cout << &y << "\n";

	(&y)[1] = 0;
	cout << "\nx: \n";
	cout << x << " ";
	cout << &x << "\n";

	cout << "\ny: \n";
	cout << y << " ";
	cout << &y << "\n";

	cout << "----------------------------------------------";
	cout << "\n\n";
	return 0;
}
