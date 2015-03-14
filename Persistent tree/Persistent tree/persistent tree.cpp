#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

#define MAX_VALUE 1000


class PersistentTree{
public:
	vector<node*> nodes;
	vector<node*> roots;

	PersistentTree(vector<int> arr);
	void Statistic(int i, int begin, int end);
	void attachPrefix(vector<int> arr, int k);
	int RSQ(int prefix, int begin, int end);

private:
	int height;
	int SizeOfTree;
	int FirstLeaf;
};

class node{
public:
	int Value;
	node* rightChild;
	node* leftChild;
	bool leaf;
	int index;
	int first;
	int last;

	int CalcSum();//for the first time
	int RecalcSum();//all next times
	node(int i);
	node(node* left, node* right, int i);
	void increment();
	node* node::ChangeLeaf(int index, node* oldVersion);
	node(node* example);
	int RSQ(int begin, int end);
	node* statistic(int i);
};

node::node(node* left, node* right, int i){
	leaf = false;
	index = i;
	rightChild = right;
	leftChild = left;
	first = leftChild->first;
	last = rightChild->last;
}

node::node(int i){//leaf
	index = i;
	leaf = true;
	Value = 0;
	rightChild = NULL;
	leftChild = NULL;
	last = first = i;
}

int node::CalcSum(){
	if (!leaf)
		Value = rightChild->CalcSum() + leftChild->CalcSum();
	return Value;
}

node::node(node* example){
	Value = example->Value;
	leaf = example->leaf;
	first = example->first;
	last = example->last;
	index = example->index;
	rightChild = example->rightChild;
	leftChild = example->leftChild;
}


int node::RecalcSum(){
	if (!leaf)
		Value = rightChild->Value + leftChild->Value;
}

int node::RSQ(int begin, int end){
	if (begin == first && end == last)
		return Value;
	else if (begin > leftChild->last)
		return rightChild->RSQ(begin, end);
	else if (end < rightChild->first)
		return leftChild->RSQ(begin, end);
	else
		return leftChild->RSQ(begin, end) + rightChild->RSQ(begin, end);
}

node* node::ChangeLeaf(int index, node* oldVersion){
	node* newNode;
	if (leaf){
		newNode = new node(oldVersion);
		newNode->Value--;
	}
	else if(index > leftChild->last){
		newNode = ChangeLeaf(index, oldVersion);
		rightChild = newNode;
	}
	else {
		newNode = ChangeLeaf(index, oldVersion);
		leftChild = newNode;
	}
	newNode->RecalcSum();
	return newNode;
}


void PersistentTree::attachPrefix(vector<int> arr, int k){//prefix with k elements(we are eliminating k-th element)
	int index = arr[k];//have to decrement
	node* oldVersion = nodes[FirstLeaf + index];
	node *newNode = new node(roots[roots.size]);//create a copy of a previous version
	newNode->ChangeLeaf(index, oldVersion);
	roots.push_back(newNode);
}

PersistentTree::PersistentTree(vector<int> arr){
	
	height = (unsigned int)(log2(MAX_VALUE) + 1);//height of the tree from 0

	SizeOfTree = 0;
	for (int l = 0; l <= height; l++){
		SizeOfTree += pow(2, l);
	}

	FirstLeaf = SizeOfTree - (int)pow(2, height);

	roots.reserve(MAX_VALUE);
	nodes.resize(SizeOfTree);

	for (int k = FirstLeaf; k < SizeOfTree; k++){
		node newLeaf = node(k);
		nodes[k] = &newLeaf;
	}
	for (int k = FirstLeaf; k > 0; k++){
		node newNode = node(nodes[2 * k + 1], nodes[2 * k + 2], k);
		nodes[k] = &newNode;
	}

	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++){
		nodes[FirstLeaf + *i]->Value++;
	}

	nodes[0]->CalcSum;

	roots.push_back(nodes[0]);

	for (int l = arr.size() - 1; l >= 0; l--)
		attachPrefix(arr, l);
}

void PersistentTree::Statistic(int i, int begin, int end){//задача найти префикс, количество rsq на котором будет k-1
	node* endOfSubsection = roots[roots.size() - end];
	node* beginOfSubsection = roots[roots.size() - begin];
}


int PersistentTree::RSQ(int prefix, int begin, int end){
	roots[roots.size() - prefix]->RSQ(begin, end);
}

node* node::statistic(int i){
	if (leaf)
		return this;
	if (i < leftChild->Value)
		return leftChild->statistic(i);
	else if (i >= leftChild->Value)
		return rightChild->statistic(i - leftChild->Value);
}