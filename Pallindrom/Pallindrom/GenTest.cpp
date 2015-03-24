#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;


char randomLetterGen(){
	static string charset = "cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	return charset[rand() % charset.length()];
}


string randomStrGen(int length) {
	string result;
	result.resize(length);
	for (int i = 0; i < length; i++)
		result[i] = randomLetterGen();
	return result;
}

const int MAXN = 100000;

void CreateTest(){

	srand(1);
	ofstream test("Test.txt");

	int index1, index2;
	char newValue;
	int begin, end, formerLength;

	int NumberOfQueries = MAXN;
	int strlength = MAXN;
	string str = randomStrGen(strlength);

	test << NumberOfQueries << endl << str << endl;

	int operation;

	for (int j = 0; j < NumberOfQueries; j++){
		operation = rand() % 2;
		test << operation << endl;
		switch (operation){
		case 0:
			index1 = rand() % (strlength / 2);
			index2 = strlength - index1 - 1;
			newValue = randomLetterGen();
			test << index1 << endl << index2 << endl <<  newValue << endl;
			break;
		case 1:
			if (!(j % 2)){
				begin = rand() % (strlength / 2);
				end = strlength - begin - 1;
			}
			else{
				end = rand() % strlength;
				begin = 0;
				if (end)
					begin = rand() % end;
			}
			test << begin << endl << end << endl;
			break;
		}

	}
	test.close();
}