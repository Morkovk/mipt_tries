#include "MySolution1.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <vector>


using namespace std;

int StupidRSQ(vector <int> arr, int begin, int end){
	int result = 0;
	for (int i = begin; i <= end; i++){
		result += arr[i];
	}
	return result;
}


void StupidNextPermutation(vector<int> &arr, int begin, int end){

	int suffix = 1;
	int i = end;

	while (arr[i] <= arr[i - 1]){
		suffix++;
		i--;
		if (i < 0)
		break;
	}
	if (suffix >= end - begin + 1){
		int k = begin;
		for (int i = 0; i < (end - k + 1) / 2; i++){
			swap(arr[k + i], arr[end - i]);
		}
	}
	else{
		i--;
		int ToChange = i;

		int j = ToChange + 1;
		while (arr[j] > arr[ToChange]){
			j++;
			if (j > end)
				break;
		}
		j--;

		swap(arr[ToChange], arr[j]);

		for (int l = 0; l < (end - ToChange) / 2; l++){
			swap(arr[ToChange + 1 + l], arr[end - l]);
		}
	}
	//std::next_permutation((arr.begin() + begin), (arr.begin() + end + 1));
}


bool TreeVsArray(item tr, vector <int> arr){
	assert(tr->c == arr.size());
	for (int i = 0; i < arr.size(); i++){
		if (tr->Find(i)->data != arr[i]){
			return false;
		}
	}
	return true;
}

