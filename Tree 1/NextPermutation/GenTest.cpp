#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#define _CRT_SECURE_NO_WARNINGS

//using namespace std;

using std::cout;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::endl;
using std::ios;
using std::string;


char randomLetterGen(){
	static string charset = "1234567890";
	return charset[rand() % charset.length()];
}


string randomStrGen(int length) {
	srand(time(NULL));
	string result;
	result.resize(length);
	for (int i = 0; i < length; i++)
		result[i] = randomLetterGen();
	return result;
}

void CreateTest(){
	
	ofstream test;
	test.open("Test.txt");

	int index;
	char newValue;
	int begin, end;

	int NumberOfQueries = rand()%100;
	int strlen = rand()% 1000;
	string str = randomStrGen(strlen);

	test << NumberOfQueries << endl << str << endl;

	int operation;
	
	for (int i = 0; i < NumberOfQueries; i++){
		operation = rand() % 3;
		switch (operation){
		case 0:
			index = rand() % strlen;
			newValue = randomLetterGen();
			test << index << endl << newValue << endl;
			break;
		case 1:
			begin = rand() % strlen;
			end = rand() % strlen - begin + 1;
			test << begin << endl << end << endl;
			break;
		case 2:
			begin = rand() % strlen;
			end = rand() % strlen - begin + 1;
			test << begin << endl << end << endl;
			break;
		}
	}

	test.close();
		
}