#include "MySolution.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <ctime>
#include <limits>
#include <iomanip>


#define _CRT_SECURE_NO_WARNINGS


using namespace std;

/*bool ArrayVsTree(Tree *tr, string str){
	for (int i = 0; i < str.length(); i++){
		int index = tr->FirstLeaf + i;
		node n = tr->arr[index];
		if ((int)str[i] != n.letter)
			return false;
	}
}*/

int i;


int main(){

	
	

	int NumberOfQueries;
	string str;
	int operation;
	char newValue;
	int index1, index2, _begin, _end;
	bool MyResult, StupidResult;
	clock_t startTime;
	ifstream test;
	vector<bool>MyResults;
	vector<bool>StupidResults;

	CreateTest();

	test.open("Test.txt");
	test >> NumberOfQueries;


	//cout << NumberOfQueries << l;
	test >> str;
	
	Tree tr(str);

	startTime = clock();


	for (i = 0; i < NumberOfQueries; i++){
		test >> operation;

		switch (operation){
		case 0:{
				   test >> index1;
				   test >> index2;
				   test >> newValue;
				   tr.SetAt(newValue, index1);
				   tr.SetAt(newValue, index2);
				   break;
		}
		case 1:{
				   test >> _begin;
				   test >> _end;
				   MyResult = tr.IsPal(_begin, _end);
				   MyResults.push_back(MyResult);
				   break;
		}
		}
	}
	cout << std::setprecision(15) << "My time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;


	test.seekg(0);
	test >> NumberOfQueries;
	test >> str;

	startTime = clock();

	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;

		switch (operation){
			case 0:{
					   test >> index1;
					   test >> index2;
					   test >> newValue;
					   str[index1] = newValue;
					   str[index2] = newValue;
					   break;
			}
			case 1:{
					   test >> _begin;
					   test >> _end;
					   StupidResult = StupidIsPal(str, _begin, _end);
					   StupidResults.push_back(StupidResult);
					   break;
			}
		}
	}
	cout << std::setprecision(15) << "Stupid time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	for (int i = 0; i < StupidResults.size(); i++){
		if (MyResults[i] != StupidResults[i])
			cout << "ERROR";
	}
	cout << "ok";
	test.close();
	getc(stdin);
	system("Pause");
}