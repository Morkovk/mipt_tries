#include "MySolution1.h"
#include <fstream>
#include <cstdio>

#define _CRT_SECURE_NO_WARNINGS


//using namespace std;

using std::cout;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::endl;
using std::ios;
using std::string;

int main(){

	CreateTest();

	int NumberOfQueries;
	string str;
	int operation;
	double MyRSQValue, StupidRSQValue;
	int index, newValue, begin, end;

	ifstream test;
	test.open("test.txt");
	test >> NumberOfQueries;
	test >> str;

	node *tr = new node((int)str[0] - (int)'0');

	for (int i = 1; i < str.length(); i++){
		item newNode = new node((int)str[i] - (int)'0');
		tr->insert(tr, i, newNode);
	}

	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;


		switch (operation){
		case 0:{
				   test >> index;
				   test >> newValue;
				   tr->Set(index, newValue);
				   tr->print();
				   StupidSet(str, index, newValue);
				   cout << TreeVsArray(tr, str) << endl;
				   break;
		}
		case 1:{
				   test >> begin;
				   test >> end;
				   tr->NextPermutation(tr, begin, end);
				   StupidNextPermutation(str, begin, end);
				   cout << TreeVsArray(tr, str) << endl;
				   break;
		}
		case 2:{
				   test >> begin;
				   test >> end;
				   double MyRSQValue = tr->RSQ(begin, end);
				   StupidRSQValue = StupidRSQ(str, begin, end);
				   if (MyRSQValue != StupidRSQValue)
					   cout << "ERROR your programm doesn't work" << endl;
		}
		}
	}
	test.close();
}