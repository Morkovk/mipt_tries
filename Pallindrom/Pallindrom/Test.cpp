#include "MySolution.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <ctime>


#define _CRT_SECURE_NO_WARNINGS


using namespace std;

bool ArrayVsTree(Tree *tr, string str){
	for (int i = 0; i < str.length(); i++){
		if (str[i] != tr->arr[tr->FirstLeaf + i]->letter)
			return false;
	}
}



int main(){

	//CreateTest();

	int NumberOfQueries;
	string str;
	int operation;
	char newValue;
	double MyRSQValue, StupidRSQValue;
	int index, begin, end;
	bool MyResult, StupidResult;
	clock_t startTime;

	ifstream test;
	test.open("Test.txt");
	test >> NumberOfQueries;
	cout << NumberOfQueries << endl;
	test >> str;

	Tree *tr = new Tree(str);


	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;

		switch (operation){
		case 0:{
				   test >> index;
				   test >> newValue;
				   startTime = clock();
				   tr->SetAt(newValue, index);
				   cout << "My time of SetAt function: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
				   startTime = clock();
				   str[index] = newValue;
				   cout << "Normal time of SetAt function: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
				   if (!ArrayVsTree(tr, str)){
					   cout << "ERROR:your programm doesn't work";
					   exit(1);
				   }
				   break;
		}
		case 1:{
				   test >> str;
				   tr = new Tree(str);
				   test >> begin;
				   test >> end;
				   startTime = clock();
				   MyResult = tr->IsPal(begin, end);
				   cout << "My time of IsPal function: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
				   startTime = clock();
				   StupidResult = StupidIsPal(str, begin, end);
				   cout << "Normal time of IsPal function: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;
				   if (MyResult != StupidResult){
					   cout << "ERROR:your programm doesn't work";
					   exit(1);
				   }
				   break;
		}
		}
	}
	cout << "Everyting is ok" << endl;
	test.close();
	getc(stdin);
	system("Pause");

}