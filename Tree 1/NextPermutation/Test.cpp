#include "MySolution1.h"
#include <fstream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <limits>
#include <iomanip>


using namespace std;

int main(){

	CreateTest(1);

	int NumberOfQueries;
	vector<int> arr;
	int operation;
	int lenght;
	long long MyRSQValue, StupidRSQValue;
	int index, newValue, begin, end;
	clock_t startTime;
	vector<int>MyResults;
	vector<int>NormResults;
	

	ifstream test;
	test.open("test.txt");
	test >> NumberOfQueries;

	cout << NumberOfQueries << endl;

	test >> lenght;

	int numb;

	for (int i = 0; i < lenght; i++){
		test >> numb;
		arr.push_back(numb);
		//cout << arr[i] << ' ';
	}


	node *tr = new node(arr[0]);
	

	for (int i = 1; i < lenght; i++){
		node* newNode = new node(arr[i]);
		insert(tr, i, newNode);
	}

	//cout << NumberOfQueries << endl;

	startTime = clock();

	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;
		
		switch (operation){
			case 0:{
					   test >> index;
					   test >> newValue;
					   tr->Set(index, newValue);
					   break;
			}
			case 1:{
					   test >> begin;
					   test >> end;
					   NextPermutation(tr, begin, end);
					   break;
			}
			case 2:{
					   test >> begin;
					   test >> end;
					   //MyRSQValue = tr->RSQ(begin, end);
					   MyRSQValue = RSQByStepa(tr, begin, end);
					   MyResults.push_back(MyRSQValue);
					   break;
			}
		}
		//cout << operation;
	}

	cout << std::setprecision(15) << "My time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	test.seekg(0);
	test >> lenght >> NumberOfQueries;
	for (int i = 0; i < lenght; i++)
		test >> numb;

	startTime = clock();

	for (int i = 0; i < NumberOfQueries; i++){
		test >> operation;
		switch (operation){
			case 0:{
					   test >> index;
					   test >> newValue;
					   arr[index] = newValue;
					   break;
			}
			case 1:{
					   test >> begin;
					   test >> end;
					   StupidNextPermutation(arr, begin, end);
					   break;
			}
			case 2:{
					   test >> begin;
					   test >> end;
					   StupidRSQValue = StupidRSQ(arr, begin, end);
					   NormResults.push_back(StupidRSQValue);
					   break;
			}
		}
	}

	cout << std::setprecision(15) << "Normal time: " << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << endl;

	for (int i = 0; i < NormResults.size(); i++){
		if (MyResults[i] != NormResults[i])
			cout << "ERROR";
	}
	cout << "ok";

	test.close();
	getchar();
}