#include <iostream>
#include <string>
#include <ctime>
#include <fstream>



#define _CRT_SECURE_NO_WARNINGS

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

void CreateTest(){

	srand(time(NULL));
	ofstream test("Test.txt");

	int index;
	char newValue;
	int begin, end, formerLength;

	int NumberOfQueries = rand() % 100;
	int strlen = rand() % 100000;
	string str = randomStrGen(strlen);

	test << NumberOfQueries << endl << str << endl;

	int operation;

	for (int j = 0; j < NumberOfQueries; j++){
		operation = rand() % 2;
		test << operation << endl;
		switch (operation){
		case 0:
			index = rand() % str.length();
			newValue = randomLetterGen();
			test << index << endl << newValue << endl;
			break;
		case 1:
			if (!(j % 2)){
				formerLength = rand() % 40 + 1;
				str = randomStrGen(formerLength);
				str.resize(formerLength * 2);
				for (int i = 0; i < formerLength; i++)
					str[formerLength + i] = str[formerLength - i - 1];
			}
			else if (!(j % 3)){
				formerLength = rand() % 40 + 1;
				str = randomStrGen(formerLength);
				str.resize(formerLength * 2 - 1) ;
				for (int i = 0; i < formerLength - 1; i++)
					str[formerLength + i] = str[formerLength - i - 2];
			}
			else{
				formerLength = rand() % 80 + 1;
				str = randomStrGen(formerLength);
			}
			test << str << endl;
			strlen = str.length();
			if (!(j % 2)){
				begin = rand() % (strlen / 2);
				end = strlen - begin - 1;
			}
			else{
				end = rand() % strlen;
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