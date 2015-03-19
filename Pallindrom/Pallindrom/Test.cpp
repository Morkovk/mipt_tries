#include "MySolution.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <ctime>
#include <limits>
#include <iomanip>


#define _CRT_SECURE_NO_WARNINGS


using namespace std;

bool ArrayVsTree(Tree *tr, string str){
	for (int i = 0; i < str.length(); i++){
		int index = tr->FirstLeaf + i;
		node n = tr->arr[index];
		if ((int)str[i] != n.letter)
			return false;
	}
}



int main(){

	CreateTest();

	int NumberOfQueries;
	string str;
	int operation;
	char newValue;
	int index, begin, end;
	bool MyResult, StupidResult;
	clock_t startTime;

	ifstream test;
	test.open("Test.txt");
	test >> NumberOfQueries;

	vector<bool>MyResults;
	vector<bool>StupidResults;

	//cout << NumberOfQueries << endl;
	test >> str;

	Tree *tr = new Tree(str);

	startTime = clock();


	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;

		switch (operation){
		case 0:{
				   test >> index;
				   test >> newValue;
				   tr->SetAt(newValue, index);
				   //str[index] = newValue;
				   /*if (!ArrayVsTree(tr, str)){
					   cout << "ERROR:your programm doesn't work";
					   exit(1);
				   }*/
				   break;
		}
		case 1:{
				   test >> str;
				   //tr = new Tree(str);
				   Tree tr(str);
				   cout << i;
				   test >> begin;
				   test >> end;
				   MyResult = tr.IsPal(begin, end);
				   MyResults.push_back(MyResult);
				   //delete tr;
				   //StupidResult = StupidIsPal(str, begin, end);
				  /*if (MyResult != StupidResult){
					   cout << "ERROR:your programm doesn't work";
					   exit(1);
				   }*/
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
				   test >> index;
				   test >> newValue;
				   str[index] = newValue;
				   //str[index] = newValue;
				   /*if (!ArrayVsTree(tr, str)){
				   cout << "ERROR:your programm doesn't work";
				   exit(1);
				   }*/
				   break;
		}
		case 1:{
				   test >> str;
				   test >> begin;
				   test >> end;
				   StupidResult = StupidIsPal(str, begin, end);
				   StupidResults.push_back(StupidResult);
				   /*if (MyResult != StupidResult){
				   cout << "ERROR:your programm doesn't work";
				   exit(1);
				   }*/
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