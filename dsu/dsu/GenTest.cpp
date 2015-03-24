#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>

using std::vector;
using std::endl;

int const MAXN = 100000;

void CreateTest(){
	srand(1);
	std::ofstream test;
	test.open("input.txt");
	int numberOfEls, numberOfQueries, operation, version, a, b;
	numberOfEls = rand()% MAXN + MAXN / 2;
	numberOfQueries = rand() % MAXN + MAXN / 2;

	test << numberOfEls << endl << numberOfQueries << endl;

	for (size_t i = 0; i < numberOfQueries; i++){
		operation = rand() % 2;
		version = rand() % i - 1;
		a = rand() % numberOfEls - 1;
		b = rand() % numberOfQueries - 1;
		test <<  operation << ' ' << version << ' ' << a << ' ' << b << endl;		
	}
}