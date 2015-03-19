#include <cstdio>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include "MySolution.h"
#include <limits>
#include <iomanip>
#include <algorithm>


using std::cout;
using std::cin;
using std::vector;
using std::endl;

int main(){

	//CreateTest();
	std::ifstream test;
	test.open("test.txt");

	clock_t startTime;

	int NumberOfQueries, length, begin, end, k;
	vector<int>MyArr;
	vector<int>MyResults;
	vector<int>CurCopy;
	vector<int>StupidResults;

	test >> NumberOfQueries;
	
	MyResults.resize(NumberOfQueries);

	test >> length;
	MyArr.resize(length);
	for (vector<int>::iterator i = MyArr.begin(); i < MyArr.end(); i++){
		test >> *i;
	}
	PersistentTree *tr = new PersistentTree(MyArr);

	startTime = clock();

	
	for (int i = 0; i < NumberOfQueries; i++){
		test >> begin >> end >> k;
		int res = tr->Statistic(k, begin, end);
		//cout << res << endl;
		MyResults[i] = res;
	}

	cout << std::setprecision(15) << "My time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;


	test.seekg(0);

	test >> NumberOfQueries;
	test >> length;

	MyArr.resize(length);

	for (vector<int>::iterator i = MyArr.begin(); i < MyArr.end(); i++){
		test >> *i;
	}

	StupidResults.resize(NumberOfQueries);
	CurCopy.resize(length);

	startTime = clock();


	for (int i = 0; i < NumberOfQueries; i++){
		std::copy(MyArr.begin(), MyArr.end(), CurCopy.begin());
		test >> begin >> end >> k;
		std::sort(CurCopy.begin() + begin, CurCopy.end() + end);
		//cout << res << endl;
		StupidResults[i] = CurCopy[begin + k];
	}

	cout << std::setprecision(15) << "Stupid time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	for (int i = 0; i < StupidResults.size(); i++){
		if (MyResults[i] != StupidResults[i])
			cout << "ERROR";
	}
	cout << "ok";

	test.close();
	getchar();
}