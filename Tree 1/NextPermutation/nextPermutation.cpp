#include <iostream>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <assert.h>


//using namespace std;

using std::cout;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::endl;
using std::ios;
using std::string;
using std::swap;

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
	double RSQValue;
	bool HaveToReverse;
	node(){
		priority = rand();
		left = right = NULL;
		c = 1;
		HaveToReverse = 0;
	}
	node(int a){
		priority = rand();
		left = right = NULL;
		c = 1;
		data = a;
		RSQValue = data;
		suffix = 1;
		leftest = rightest = this;
		HaveToReverse = 0;
	}
	void merge(item &tree, item first, item second);
	void split(item tree, int i, item & left, item & right);
	void insert(item & tree, int i, item it);
	double RSQ(int begin, int end);
	item Find(int i);
	void Set(int i, int newValue);
	void UpdateSuffix();
	void UpdatePrefix();
	int CountSuffix(int begin, int end);
	void UpdateLeftestRightest();
	int FindNextBigger(int begin, int end, int value);
	void NextPermutation(item & tree, int begin, int end);
	void ReverseSubSection(item & tree, int begin, int end);
	item FindSubSection(int begin, int end);
	void Push();
	void RecalculateTree();
	int sizeofRSQ(item x);
	int sizeofSuffix(item x);
	int sizeofPrefix(item x);
	int sizeofC(item x);
	void MySwap(item & tree, int i, int j);
	void print();
	
};




void node::UpdateLeftestRightest(){
	if (!right)
		rightest = this;
	else
		rightest = right->rightest;

	if (!left)
		leftest = this;
	else
		leftest = left->leftest;
}

void node::Push(){
	if (this){
		if (HaveToReverse){
			if (left)
				left->HaveToReverse = !left->HaveToReverse;
			if (right)
				right->HaveToReverse = !right->HaveToReverse;
			swap(left, right);
			swap(suffix, prefix);
			HaveToReverse = 0;
		}
	}
}

void node::UpdatePrefix(){
	prefix = sizeofPrefix(left);
	if (sizeofPrefix(left) == sizeofC(left)){
		if ((!left) || (data > left->rightest->data)){
			prefix += 1;
			if ((!right) || (data < right->leftest->data))
				prefix += sizeofPrefix(right);
		}
	}
}

void node::UpdateSuffix(){
	suffix = sizeofSuffix(right);
	if (sizeofSuffix(right) == sizeofC(right)){
		if ((!right) || (data > right->leftest->data)){
			suffix += 1;
			if ((!left) || (data < left->rightest->data))
				suffix += sizeofSuffix(left);
		}
	}
}

void node::merge(item &tree, item first, item second) {
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
	tree->RecalculateTree();
}

void node::split(item tree, int i, item & l, item & r) {//cut i elements from the left side
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

void node::insert(item & tree, int i, item it) {
	if (!tree)
		tree = it;
	item l;
	item r;
	split(tree, i, l, r);
	merge(tree, l, it);
	merge(tree, tree, r);
	tree->RecalculateTree();//can be removed, split, merge do it automatically
}



double node::RSQ(int begin, int end){

	int result = 0;

	if (begin < 0)
		begin = 0;
	if ((end < 0) || (begin > c))
		return 0;


	if (begin >= sizeofC(left) + 1){
		if (!right)
			return 0;
		else
			return right->RSQ(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	}
	if (end < sizeofC(left)){
		if (!left)
			return 0;
		else
			return left->RSQ(begin, end);
	}

	result = data;

	if ((end - begin + 1) == c){
		if (left)
			result += left->RSQValue;
		if (right)
			result += right->RSQValue;
	}

	else{

		if (left)
			result = left->RSQ(begin, end);
		if (right)
			result += right->RSQ(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	}
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
	if (begin < 0)
		begin = 0;
	if ((end < 0) || (!this) || (begin > c))
		return 0;

	int LongestSuffix = 0;

	if (end - begin + 1 == c)
		return suffix;

	if (begin > sizeofC(left) + 1)
		return right->CountSuffix(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	if (end < sizeofC(left))
		return left->CountSuffix(begin, end);
	
	if (right)
		LongestSuffix = right->CountSuffix(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	if ((!right) || ((LongestSuffix == sizeofC(right)) && (data > right->leftest->data))){
		LongestSuffix += 1;
		if ((left) && (data < left->rightest->data))
			LongestSuffix += left->CountSuffix(begin, c);
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


int node::sizeofC(item x){
	if (x)
		return x->c;
	else
		return 0;
}

int node::sizeofRSQ(item x){
	if (x)
		return x->RSQValue;
	else
		return 0;
}

int node::sizeofSuffix(item x){
	if (x)
		return x->suffix;
	else
		return 0;
}

int node::sizeofPrefix(item x){
	if (x)
		return x->prefix;
	else
		return 0;
}


void node::RecalculateTree(){
	Push();
	UpdateLeftestRightest();
	UpdatePrefix();
	UpdateSuffix();
	RSQValue = RSQ(0, c);
	c = sizeofC(right) + sizeofC(left) + 1;
}

void node::ReverseSubSection(item & tree, int begin, int end){
	Push();
	item l;
	item Subsection;
	item r;
	split(tree, end + 1, Subsection, r);
	//cout << sizeofC(Subsection) << ' ' << sizeofC(right) << endl;
	split(Subsection, begin, l, Subsection);
	//cout << " left " << sizeofC(l) << ' ' << "subsection "<<  sizeofC(Subsection) << "right " << sizeofC(r) << endl << endl;
	//Subsection->print();

	Subsection->HaveToReverse = !Subsection->HaveToReverse;
	merge(tree, l, Subsection);
	merge(tree, tree, r);
}


int node::FindNextBigger(int begin, int end, int value){
	Push();
	int ResultIndex = 0;
	if (begin >= sizeofC(left) + 1){
		ResultIndex += sizeofC(left) + 1;
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

void node::MySwap(item & tree, int i, int j){
	item first, ToChange, third, Next, fifth;
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

void node::NextPermutation(item & tree, int begin, int end){
	int suffix = tree->CountSuffix(begin, end);
	if (suffix == sizeofC(tree))
		tree->ReverseSubSection(tree, begin, end);
	item ToChange = tree->Find(end - suffix);
	/*item Next = tree->FindNextBigger(end - suffix + 1, end, ToChange->data);*/
	int ToChangeIndex = end - suffix;
	int Next = tree->FindNextBigger(end - suffix + 1, end, ToChange->data);
	//cout << endl << " To change: " << ToChange->data << ' ' << ToChangeIndex << ' ' << " Next " << Next << endl;
	MySwap(tree, ToChangeIndex, Next);
	ReverseSubSection(tree, end - suffix + 1, end);
}

void node::print(){
	Push();
	if (left)
		left-> print();
	cout << data << ' ';
	if (right)
		right->print();
}

