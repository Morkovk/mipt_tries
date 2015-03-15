#include "MySolution1.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <assert.h>

//using namespace std;

using std::cout;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::endl;
using std::ios;
using std::string;

int StupidRSQ(string str, int begin, int end){
	int result = 0;
	for (int i = begin; i <= end; i++){
		result += (int)str[i] - (int)'0';
	}
	return result;
}

bool cmp(const char &a, const char &b){
	return (bool)((int)a - (int)b);
}


void StupidNextPermutation(string &str, int begin, int end){
	std::next_permutation(str[begin], str[end], cmp);
}

void StupidSet(string str, int index, int newValue){
	str[index] = (char)((int)'0' + newValue);
}

bool TreeVsArray(item tr, string str){
	assert(tr->c == str.length());
	for (int i; i < str.length(); i++){
		if (tr->Find(i)->data != (int)str[i] - (int)'0'){
			cout << "ERROR: your programm doesn't work";
			return false;
		}
	}
	return true;
}

