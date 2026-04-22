#include <iostream>
#include <vector>
#include <iomanip>
#include "sorts.hpp"
#include "randfiller.h"
#include "timecounter.h"

using namespace std;

int main() {
	timecounter tc;
	randfiller rf;
	vector<int> v;
	vector<int> vs;
	vector<float> times;
	vector<float> times2;
	vector<vector<float>> globalTimes;

	times.resize(19);
	times2.resize(19);
	globalTimes.resize(4);

	for (auto i = 2; i < 19; i++) {
		int j = pow(2, i);
		v.resize(j);
		rf.fill(v, -100, 100);

		vs = v;
		tc.tic();
		if (j >= 1) mergesort(vs, 0, j - 1);
		times[i] = tc.toc();


		vs = v;
		tc.tic();
		if (j >= 1) quicksort(vs, 0, j - 1);
		times2[i] = tc.toc();

	}
	cout << std::fixed << std::setprecision(18);
	cout << "timesMerge = " << times << ";" << endl;
	cout << "timesQuick = " << times2 << ";" << endl;
	return 0;
}
