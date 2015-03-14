#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class node;
class Tree;




class Tree{
public:
	node** arr;
	unsigned int SizeOfTree;
	unsigned int height;

	int t; //coeffcient
	int modul; //modul
	unsigned int FirstLeaf;
	unsigned int LastLeaf;

	Tree(string str);
	bool IsPal(int begin, int end);
	void SetAt(char a, int i);
	void RecalcLeftPolValue(int i);
	void RecalcRightPolValue(int i);
	unsigned int LeftPol(int i, int begin, int end);
	unsigned int RightPol(int i, int begin, int end);
};


class node{
public:
	int index;//index
	int letter;//if leaf
	unsigned int LeftPolValue;//beginning from the left //if node
	unsigned int RightPolValue;//beginning from the right //if node

	int first;//first leaf
	int last;//last leaf

	node(char a, int i);//leaf
	node(int i, node* a, node *b, int t, int modul);//not leaf
};


node::node(char a, int i){//leaf
	index = i;
	letter = (int)a;
	LeftPolValue = letter;
	RightPolValue = letter;
	first = last = i;
}

node::node(int i, node* leftChild, node* rightChild, int t, int modul){//not leaf
	index = i;
	first = leftChild->first;
	last = rightChild->last;
	unsigned int absoluteLeftPolValue = leftChild->LeftPolValue * pow(2, (rightChild->last - rightChild->first + 1)) + rightChild->LeftPolValue;
	LeftPolValue = absoluteLeftPolValue % modul;
	unsigned int absoluteRightPolValue = rightChild->RightPolValue * pow(2, (leftChild->last - leftChild->first + 1)) + leftChild->RightPolValue;
	RightPolValue = absoluteRightPolValue % modul;
}


unsigned int Tree::LeftPol(int i, int begin, int end){
	unsigned int result = 0;
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	if ((begin == (arr[i])->first) && (end == (arr[i])->last))
		result = (arr[i])->LeftPolValue;
	else if (begin > (arr[LeftChild])->last){
		result = LeftPol(RightChild, begin, end);
	}
	else if (end < (arr[RightChild])->first) {
		result = LeftPol(LeftChild, begin, end);
	}
	else{
		result = LeftPol(RightChild, (arr[RightChild])->first, end) + (int)LeftPol(LeftChild, begin, (arr[LeftChild])->last) * pow(t, end - (arr[RightChild])->first + 1);
	}
	return (result % modul);
}




unsigned int Tree::RightPol(int i, int begin, int end){
	unsigned int result = 0;
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	if ((begin == (arr[i])->first) && (end == (arr[i])->last))
		result = (arr[i])->RightPolValue;
	else if (begin > (arr[LeftChild])->last){
		result = RightPol(RightChild, begin, end);
	}
	else if (end < (arr[RightChild])->first) {
		result = RightPol(LeftChild, begin, end);
	}
	else{
		result = RightPol(LeftChild, begin, (arr[LeftChild])->last) + (int)RightPol(RightChild, (arr[RightChild])->first, end) * pow(t, (arr[LeftChild])->last - begin + 1);
	}
	return (result % modul);
}

void Tree::SetAt(char a, int i){
	int index = FirstLeaf + i;//from zero
	arr[index]->letter = (int)a;
	arr[index]->LeftPolValue = arr[index]->RightPolValue = arr[index]->letter;
	do{
		index = (index - 1) / 2;
		RecalcLeftPolValue(index);
		RecalcRightPolValue(index);
	} while (index);
}

void Tree::RecalcLeftPolValue(int i){
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	unsigned int result = arr[LeftChild]->LeftPolValue * pow(t, arr[RightChild]->last - arr[RightChild]->first + 1) + arr[RightChild]->LeftPolValue;
	arr[i]->LeftPolValue = (result % modul);
}

void Tree::RecalcRightPolValue(int i){
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	unsigned int result = arr[RightChild]->RightPolValue * pow(t, arr[LeftChild]->last - arr[LeftChild]->first + 1) + arr[LeftChild]->RightPolValue;
	arr[i]->RightPolValue = (result % modul);
}

bool Tree::IsPal(int begin, int end){
	int begin1 = FirstLeaf + begin;
	int end1 = FirstLeaf + begin + (end - begin) / 2;
	int begin2 = FirstLeaf + begin + (end - begin + 1) / 2;
	int end2 = FirstLeaf + end;//we must not go outside the word
	if (end1 == begin2 && (begin1 != end1 || begin2 != end2)){
		end1 -= 1;
		begin2 += 1;
	}

	unsigned int LeftHalf = LeftPol(0, begin1, end1);
	unsigned int RightHalf = RightPol(0, begin2, end2);
	if (LeftHalf == RightHalf)
		return 1;
	else
		return 0;
}


Tree::Tree(string str){

	modul = pow(2, 31) - 1;
	t = 2;
	
	double height1 = (double(str.length()));
	double height2 = log2(height1) + 1;
	height = (unsigned int)height2;

	//height = (unsigned int)(log2(double(str.length())) + 1);//height of the tree from 0
	
	SizeOfTree = 0;
	for (int l = 0; l <= height; l++){
		SizeOfTree += pow(2, l);
	}
	arr = new node*[SizeOfTree];

	unsigned int cur = SizeOfTree;//the last index is SizeOfTree - 1
		
	cur -= (int)pow(2, height);//the beginning of the zero level

	unsigned int i = 0;

	for (i = 0; i < str.length(); i++){
		node *newLeaf = new node(str[i], cur + i);
		arr[cur + i] = newLeaf;
	}
	for (i; i < (int)pow(2, height); i++){
		node *newLeaf = new node(0, cur + i);
		arr[cur + i] = newLeaf;
	}

	for (int k = height - 1; k >= 0; k--){
		cur -= (int)pow(2, k);
		for (unsigned int i = 0; i < (int)pow(2, k); i++){
			node *newNode = new node(cur + i, arr[(cur + i) * 2 + 1], arr[(cur + i) * 2 + 2], t, modul);
			arr[cur + i] = newNode;
			RecalcLeftPolValue(cur + i);
			RecalcRightPolValue(cur + i);
		}
	}
	FirstLeaf = SizeOfTree - (unsigned int)pow(2, height);
}
