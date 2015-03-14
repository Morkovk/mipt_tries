#include<string>
#include <iostream>

using namespace std;

bool StupidIsPal(string str, int begin, int end){
	int i = 0;
	while (begin + i <= end - i) {
		if (str[begin + i] != str[end - i])
			return false;
		i++;
	}
	return true;
}