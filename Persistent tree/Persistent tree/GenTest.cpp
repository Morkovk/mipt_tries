#include <cstdio>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>

using std::vector;

using std::cout;
using std::cin;
using std::vector;
using std::endl;

const int MAXN = 100000;
const int range = 1000;

void CreateTest(){
	srand(1);
	std::ofstream test;
	test.open("test.txt");

	int NumberOfQueries, length, begin, end, k;
	vector<int>MyArr;

	NumberOfQueries = MAXN;

	test << NumberOfQueries << endl;

	length = rand() % MAXN + MAXN / 2;

	test << length << endl;

	MyArr.resize(length);
	for (vector<int>::iterator i = MyArr.begin(); i < MyArr.end(); i++){
		*i = rand() % range;
		test << *i << " ";
	}
	test << endl;
	
	for (int i = 0; i < NumberOfQueries; i++){		
		end = rand() % length + 1;
		begin = rand() % end;
		int maxK = end - begin;
		k = rand() % maxK;
		test << begin << endl << end << endl << k << endl;
	}

	test.close();
}