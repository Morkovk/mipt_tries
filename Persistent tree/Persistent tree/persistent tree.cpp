#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::cin;
using std::vector;
using std::endl;

#define MAX_VALUE 20

class node;

class PersistentTree{
public:
	vector<node> nodes;
	vector<node*> roots;
	//node** nodes;
	//node** roots;

	PersistentTree(vector<int> & arr);
	int Statistic(int i, int begin, int end);
	int statisticForTwoPrefixes(int i, node* begin, node* end);
	void attachPrefix(vector<int> & arr, int k);
	int RSQ(int prefix, int begin, int end);
	void Calc();

private:
	int SizeOfTree;
	int FirstLeaf;
	int range;
	vector<int>copy;
	vector<int>Compressed;
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
	//node(node* left, node* right, int i);
	void ChangeLeaf(int indexToChange);
	node(node* example);
	int RSQ(int begin, int end);
};

/*node::node(node* left, node* right, int i){
	leaf = false;
	index = i;
	rightChild = right;
	leftChild = left;
	first = leftChild->first;
	last = rightChild->last;
}*/

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



void node::ChangeLeaf(int indexToChange){
	node* newNode;
	if (leaf){
		Value--;
		//cout <<"now are " << Value << endl;
	}
	else if(indexToChange > leftChild->last){
		newNode = new node(rightChild);
		newNode->ChangeLeaf(indexToChange);
		rightChild = newNode;
	}
	else {
		newNode = new node(leftChild);
		newNode->ChangeLeaf(indexToChange);
		leftChild = newNode;
	}
	RecalcSum();
}


void PersistentTree::attachPrefix(vector<int> & arr, int k){//prefix with k elements(we are eliminating k-th element
	int index = arr[k];//have to decrement
	//cout << "we lost  " << index << endl;
	node *newNode = new node(roots[roots.size() - 1]);//create a copy of a previous version
	newNode->ChangeLeaf(FirstLeaf + index);
	roots.push_back(newNode);
}

void PersistentTree::Calc(){
	
	for (int i = FirstLeaf - 1; i >= 0; i--){
		nodes[i].rightChild = &nodes[2 * i + 2];
		nodes[i].leftChild = &nodes[2 * i + 1];
		nodes[i].Value = nodes[i].rightChild->Value + nodes[i].leftChild->Value;
		nodes[i].last = nodes[i].rightChild->last;
		nodes[i].first = nodes[i].leftChild->first;
		nodes[i].index = i;
		nodes[i].leaf = false;
	}
}


int comp(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}


PersistentTree::PersistentTree(vector<int> & arr){

	copy.resize(arr.size());
	Compressed.resize(arr.size());

	std::copy(arr.begin(), arr.end(), copy.begin());

	std::sort(copy.begin(), copy.end());

	

	for (int i = 0; i < arr.size(); i++){
		Compressed[i] = std::lower_bound(copy.begin(), copy.end(), arr[i]) - copy.begin();
	}

	range = std::lower_bound(copy.begin(), copy.end(), (*(copy.end() - 1))) - copy.begin();

	int p = 0;

	while ((1 << p) < range) p++;
	FirstLeaf = ((1 << p) - 1);
	p++;

	SizeOfTree = (1 << p);

	nodes.resize(SizeOfTree, node(0));

	for (int i = FirstLeaf; i < SizeOfTree; i++){
		nodes[i].index = i;
		nodes[i].last = nodes[i].first = i;
	}

	for (int i = 0; i < Compressed.size(); i++){
		(nodes[FirstLeaf + Compressed[i]].Value)++;//неведомая хрень
	}
		
	Calc();

	roots.push_back(&nodes[0]);

	for (int l = arr.size() - 1; l >= 0; l--)
		attachPrefix(Compressed, l);
}

int PersistentTree::Statistic(int i, int begin, int end){//задача найти префикс, количество rsq на котором будет k-1
	node* endOfSubsection = roots[roots.size() - 1 - end];
	node* beginOfSubsection = roots[roots.size() - 1 - begin];
	int res = statisticForTwoPrefixes(i, beginOfSubsection, endOfSubsection);
	return copy[res];
}



int PersistentTree::statisticForTwoPrefixes(int i, node* begin, node* end){
	if (begin->leaf)
		return begin->index - FirstLeaf;
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

/*int main(){
	vector<int> arr(20);

	const int range = 15;

	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++)
		*i = rand() % range;

	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++)
		cout << *i << ' ';
	cout << endl;

	PersistentTree *tr = new PersistentTree(arr, range);
	cout << tr->Statistic(7, 4, 17) << endl;

	std::sort(arr.begin() + 4, arr.begin() + 17);

	cout << arr[11] << endl;
	for (vector<int>::iterator i = arr.begin(); i < arr.end(); i++)
		cout << *i << ' ';
	cout << endl; 

}*/