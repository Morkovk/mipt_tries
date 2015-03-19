#include <iostream>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <assert.h>
#include <cstdio>


using namespace std;

class node;
typedef node * item;

class node {
public:

	int priority;
	item left, right;
	int c; //размер дерева (правое + левое + корень)
	int data;
	item leftest;//the leftes child in a tree
	item rightest;//the rightest child in a tree
	int suffix;//dropping
	int prefix;//rising

	node(int a);
	long long RSQValue;
	bool HaveToReverse;
	long long RSQ(int begin, int end);
	item Find(int i);
	void Set(int i, int newValue);
	void UpdateSuffix();
	void UpdatePrefix();
	int CountSuffix(int begin, int end);
	void UpdateLeftestRightest();
	int FindNextBigger(int begin, int end, int value);
	item FindSubSection(int begin, int end);
	void Push();
	void RecalculateTree();
	void print();

};

void merge(item &tree, item first, item second);
void split(item tree, int i, item & left, item & right);
void NextPermutation(item & tree, int begin, int end);
void NextSubPermutation(item & tree);
void insert(item & tree, int i, item & it);
void ReverseSubSection(item & tree, int begin, int end);
void MySwap(item & tree, int i, int j);
int sizeofRSQ(item x);
int sizeofSuffix(item x);
int sizeofPrefix(item x);
int sizeofC(item x);
long long RSQByStepa(item & tree, int begin, int end);


void node::UpdateLeftestRightest(){
	if (!right)
		rightest = this;
	else{
		//right->RecalculateTree();
		rightest = right->rightest;
	}

	if (!left)
		leftest = this;
	else{
		//left->RecalculateTree();
		leftest = left->leftest;
	}
}

node::node(int a){
	priority = rand();
	left = right = NULL;
	c = 1;
	data = a;
	RSQValue = data;
	suffix = 1;
	prefix = 1;
	leftest = rightest = this;
	HaveToReverse = 0;
}


void node::Push(){
	if (HaveToReverse){
			if (left)
				left->HaveToReverse = !left->HaveToReverse;
			if (right)
				right->HaveToReverse = !right->HaveToReverse;
			swap(left, right);
			swap(suffix, prefix);
			swap(leftest, rightest);
			HaveToReverse = 0;
	}
}

void node::UpdatePrefix(){
	prefix = sizeofPrefix(left);
	if (sizeofPrefix(left) == sizeofC(left)){
		if ((!left) || (data >= left->rightest->data)){
			prefix += 1;
			if ((!right) || (data <= right->leftest->data))
				prefix += sizeofPrefix(right);
		}
	}
}

void node::UpdateSuffix(){
	suffix = sizeofSuffix(right);
	if (sizeofSuffix(right) == sizeofC(right)){
		if ((!right) || (data >= right->leftest->data)){
			suffix += 1;
			if ((!left) || (data <= left->rightest->data))
				suffix += sizeofSuffix(left);
		}
	}
}

void merge(item &tree, item first, item second) {

	if (!first || !second){
		if (!first)
			tree = second;
		else
			tree = first;
	}
	else{
		first->Push();
		second->Push();
		if (first->priority > second->priority){
			merge(first->right, first->right, second);
			tree = first;
		}
		else{
			merge(second->left, first, second->left);
			tree = second;
		}
	}
	if (tree != NULL)
		tree->RecalculateTree();
}

void split(item tree, int i, item & l, item & r) {//cut i elements from the left side
	if (tree == NULL){
		l = NULL;
		r = NULL;
	}
	else {
		tree->Push();
		if (i <= sizeofC(tree->left)){
			if (!tree->left){
				l = tree->left;
				r = tree;
			}
			else {
				split(tree->left, i, l, tree->left);
				r = tree;
			}
		}
		else{
			if (!tree->right){
				l = tree;
				r = tree->right;
			}
			else {
				split(tree->right, i - sizeofC(tree->left) - 1, tree->right, r);
				l = tree;
			}
		}
		if (l)
			l->RecalculateTree();
		if (r)
			r->RecalculateTree();
	}
	
}

void insert(item & tree, int i, item & it) {
	if (!tree)
		tree = it;
	else{
		item l = NULL;
		item r = NULL;
		split(tree, i, l, r);
		merge(tree, l, it);
		merge(tree, tree, r);
		//tree->RecalculateTree();//can be removed, split, merge do it automatically
	}
	
}


/*
long long node::RSQ(int begin, int end){

	int result = 0;

	if (begin == 0 && end == c - 1)
		return RSQValue;

	if (begin >= sizeofC(left)){
		if (begin >= sizeofC(left) + 1){
			if (!right)
				return 0;
			else
				return right->RSQ(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
		}
		else{
			if (!right)
				return data;
			else
				return data + right->RSQ(0, end - sizeofC(left) - 1);
		}
	}

	else if (end <= sizeofC(left)){
		if (end < sizeofC(left)){
			if (!left)
				return 0;
			else
				return left->RSQ(begin, end);
		}
		else{
			if (!left)
				return data;
			else
				return data + left->RSQ(begin, end - 1);
		}
	}

	else
		return left->RSQ(begin, sizeofC(left) - 1) + data + right->RSQ(0, end - sizeofC(left) - 1);
}
*/

long long RSQByStepa(item & tree, int begin, int end)
{
	/*if (end - begin + 1 == sizeofC(tree))
		return tree-*/
	item l = NULL, r = NULL, Subsection = NULL;
	split(tree, end + 1, Subsection, r);
	split(Subsection, begin, l, Subsection);
	long long result = 0;
	if (Subsection)
		result = Subsection->RSQValue;
	else
		result = 0;
	merge(tree, l, Subsection);
	merge(tree, tree, r);
	return result;
}

item node::Find(int i){
	Push();
	if (i == sizeofC(left))
		return this;
	else if (sizeofC(left) > i)
		left->Find(i);
	else
		right->Find(i - sizeofC(left) - 1);
}


int node::CountSuffix(int begin, int end){
	Push();
	int LongestSuffix = 0;

	if (end - begin + 1 == c)
		return suffix;

	if (begin >= sizeofC(left)){
		if (begin >= sizeofC(left) + 1)
			return right->CountSuffix(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
		else{
			if (!right)
				return 1;
			else{
				LongestSuffix = right->CountSuffix(0, end - sizeofC(left) - 1);
				if (data >= right->leftest->data)
					LongestSuffix += 1;
			}
		}
	}

	else if (end <= sizeofC(left)){
		if (end <= sizeofC(left) - 1){
			return left->CountSuffix(begin, end);
		}
		else{
			if (!left)
				return 1;
			else{
				LongestSuffix = 1;
				if (data <= left->rightest->data)
					LongestSuffix += left->CountSuffix(begin, sizeofC(left) - 1);
			}
		}
	}

	else{
		LongestSuffix = right->CountSuffix(0, end - sizeofC(left) - 1);
		if (data >= right->leftest->data){
			LongestSuffix += 1;
			if (data <= left->rightest->data)
				LongestSuffix += left->CountSuffix(begin, sizeofC(left) - 1);
		}

	}
	return LongestSuffix;
}



void node::Set(int i, int newValue){
	Push();
	if (i == sizeofC(left)){
		data = newValue;
	}
	else if (sizeofC(left) > i){
		left->Set(i, newValue);
	}
	else{
		right->Set(i - sizeofC(left) - 1, newValue);
	}
	RecalculateTree();
}


int sizeofC(item x){
	if (x)
		return x->c;
	else
		return 0;
}

int sizeofRSQ(item x){
	if (x)
		return x->RSQValue;
	else
		return 0;
}

int sizeofSuffix(item x){
	if (x)
		return x->suffix;
	else
		return 0;
}

int sizeofPrefix(item x){
	if (x)
		return x->prefix;
	else
		return 0;
}


void node::RecalculateTree(){
	Push();
	if (left)
		left->Push();
	if (right)
		right->Push();
	UpdateLeftestRightest();
	UpdatePrefix();
	UpdateSuffix();
	RSQValue = sizeofRSQ(left) + sizeofRSQ(right) + data;
	c = sizeofC(right) + sizeofC(left) + 1;
}

void ReverseSubSection(item & tree, int begin, int end){
	if (tree == NULL)
		return;
	tree->Push();
	item l = NULL;
	item Subsection = NULL;
	item r = NULL;
	split(tree, end + 1, Subsection, r);
	split(Subsection, begin, l, Subsection);
	Subsection->HaveToReverse = !Subsection->HaveToReverse;
	merge(tree, l, Subsection);
	merge(tree, tree, r);
}

int node::FindNextBigger(int begin, int end, int value){
	Push();
	int ResultIndex = 0;
	if (begin >= sizeofC(left) + 1){
		ResultIndex += sizeofC(left) + 1;
		if (right)
			ResultIndex += right->FindNextBigger(begin - sizeofC(left) - 1, end - sizeofC(left) - 1, value);
	}
	else if (end < sizeofC(left))
		ResultIndex += left->FindNextBigger(begin, end, value);
	else{
		if (data <= value){
			ResultIndex += left->FindNextBigger(begin, sizeofC(left), value);
		}
		else {
			ResultIndex = sizeofC(left);
			if ((!right) || (right->leftest->data <= value))
				ResultIndex += 0;
			else
				ResultIndex += 1 + right->FindNextBigger(0, end - sizeofC(left) - 1, value);//we have to count the current node. 
		}
	}
	return ResultIndex;
}

void MySwap(item & tree, int i, int j){
	if (tree == NULL)
		return;
	item first = NULL, ToChange = NULL, third = NULL, Next = NULL, fifth = NULL;
	split(tree, j + 1, tree, fifth);
	split(tree, j, tree, Next);
	split(tree, i + 1, tree, third);
	split(tree, i, first, ToChange);
	merge(tree, first, third);
	merge(tree, tree, fifth);
	insert(tree, i, Next);
	insert(tree, j, ToChange);
	tree->RecalculateTree();
}


void NextSubPermutation(item &tree){
	if (true == NULL)
		return;
	item ToChange = NULL;
	int begin = 0;
	int end = tree->c - 1;
	int suffix = tree->suffix;
	//CountSuffix(begin, end);
	if (suffix == sizeofC(tree))
		ReverseSubSection(tree, begin, end);
	else{
		ToChange = tree->Find(end - suffix);
		int ToChangeIndex = end - suffix;
		int Next = tree->FindNextBigger(end - suffix + 1, end, ToChange->data);
		MySwap(tree, ToChangeIndex, Next);
		ReverseSubSection(tree, end - suffix + 1, end);
	}
}

void NextPermutation(item & tree, int begin, int end){
	if (tree == NULL)
		return;
	item l = NULL, Subsection = NULL, r = NULL;
	split(tree, end + 1, Subsection, r);
	split(Subsection, begin, l, Subsection);
	if (Subsection != NULL){
		//cout << "I watch you";
		NextSubPermutation(Subsection);
	}
	merge(tree, l, Subsection);
	merge(tree, tree, r);
}

void node::print(){
	Push();
	if (left)
		left->print();
	cout << data << ' ';
	if (right)
		right->print();
}

