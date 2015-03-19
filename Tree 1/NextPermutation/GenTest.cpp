#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;



vector<int> randomArrGen(int lenght) {
	vector<int>arr(lenght);
	for (int i = 0; i < lenght; i++)
		arr[i] = rand()%1000 + 1;
	return arr;
}

void CreateTest(int numberoftest){
	srand(numberoftest);
	
	ofstream test;
	test.open("Test.txt");

	int index;
	int newValue;
	int begin, end;

	int NumberOfQueries = 100000;

	int lenght = 100000 + 1;
	vector<int>arr = randomArrGen(lenght);


	test << NumberOfQueries << endl << lenght << endl;
	
	for (int i = 0; i < lenght; i++)
		test << arr[i] << ' ';
	
	test << endl;

	int operation;
	
	for (int i = 0; i < NumberOfQueries; i++){
		operation = rand() % 3;
		switch (operation){
		case 0:
			index = rand() % lenght;
			newValue = rand()% 1000 + 1;
			test << operation << endl << index << endl << newValue << endl;
			break;
		case 1:
			begin = rand() % lenght;
			end = begin + rand() % (lenght - begin);
			test << operation << endl << begin << endl << end << endl;
			break;
		case 2:
			begin = rand() % lenght;
			end = begin + rand() % (lenght - begin);
			test << operation << endl << begin << endl << end << endl;
			break;
		}
	}

	test.close();
		
}