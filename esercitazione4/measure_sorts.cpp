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
    vector<float> times3;
    vector<float> times4;
    vector<vector<float>> globalTimes;

    times.resize(2000);
    times2.resize(2000);
    times3.resize(2000);
    times4.resize(2000);
    globalTimes.resize(4);

    for (int i = 0; i < 2000; i++) {
        v.resize(i);
        rf.fill(v, -100, 100);

        vs = v;
	tc.tic();
        selection_sort(vs);
        times[i] = tc.toc();

        vs = v;
        tc.tic();
        bubble_sort(vs);
        times2[i] = tc.toc();

        vs = v;
        tc.tic();
        insertion_sort(vs);
        times3[i] = tc.toc();

	vs = v;
	tc.tic();
	if (i >= 1) mergesort(vs, 0, i - 1);
	times4[i] = tc.toc();

    }
    cout << std::fixed << std::setprecision(12);
    cout << "timesSelection = " << times << ";" << endl;
    cout << "timesBobble = " << times2 << ";" << endl;
    cout << "timesInsertion = " << times3 << ";" << endl;
    cout << "timesMerge = " << times4 << ";" << endl;   
    return 0;
}
