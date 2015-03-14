#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_VALUE 20

class node;

class PersistentTree{
public:
	vector<node*> nodes;
	vector<node*> roots;

	PersistentTree(vector<int> arr);
	int Statistic(int i, int begin, int end);
	int statisticForTwoPrefixes(int i, node* begin, node* end);
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
	return Value;
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
	node *newNode = new node(roots[roots.size()]);//create a copy of a previous version
	newNode->ChangeLeaf(index, oldVersion);
	roots.push_back(newNode);
}

PersistentTree::PersistentTree(vector<int> arr){
	
	height = (unsigned int)(log2(MAX_VALUE) + 1);//height of the tree from 0

	SizeOfTree = 0;
	for (int l = 0; l <= height; l++){
		SizeOfTree += pow(2, l);
	}

	FirstLeaf = SizeOfTree - pow(2, height);

	roots.reserve(arr.size());
	nodes.resize(SizeOfTree);

	for (int k = FirstLeaf; k < SizeOfTree; k++){
		node newLeaf = node(k);
		nodes[k] = &newLeaf;
	}
	for (int k = FirstLeaf - 1; k >= 0; k--){
		node newNode = node(nodes[2 * k + 1], nodes[2 * k + 2], k);
		nodes[k] = &newNode;
	}

	for (int i = 0; i < arr.size(); i++){
		(nodes[FirstLeaf + arr[i]]->Value)++;
	}

	nodes[0]->CalcSum();

	roots.push_back(nodes[0]);

	for (int l = arr.size() - 1; l >= 0; l--)
		attachPrefix(arr, l);
}

int PersistentTree::Statistic(int i, int begin, int end){//задача найти префикс, количество rsq на котором будет k-1
	node* endOfSubsection = roots[roots.size() - end];
	node* beginOfSubsection = roots[roots.size() - begin];
	return statisticForTwoPrefixes(i, beginOfSubsection, endOfSubsection);
}



int PersistentTree::statisticForTwoPrefixes(int i, node* begin, node* end){
	if (begin->leaf)
		return begin->index;
	if (i < end->leftChild->Value - begin->leftChild->Value)
		return statisticForTwoPrefixes(i, begin->leftChild, end->leftChild);
	else
		return statisticForTwoPrefixes(i - (end->leftChild->Value - begin->leftChild->Value), begin->rightChild, end->rightChild);
}


int PersistentTree::RSQ(int prefix, int begin, int end){
	return roots[roots.size() - prefix]->RSQ(begin, end);
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

int main(){
	vector<int> arr(20);

	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++)
		*i = rand() % 20 + 1;

	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++)
		cout << *i << ' ';
	cout << endl;

	PersistentTree *tr = new PersistentTree(arr);
	cout << tr->Statistic(3, 0, 19);

	std::sort(arr.begin(), arr.end());
	cout << arr[3];
}