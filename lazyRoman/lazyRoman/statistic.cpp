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

//#define MAX_VALUE 20

class node;

class PersistentTree{
public:
	vector<node> nodes;
	vector<node*> roots;
	//node** nodes;
	//node** roots;

	PersistentTree(vector<int> & arr);
	int Statistic(int i, int begin, int end);
	int statisticForTwoPrefixes(int i, node* begin, node* end);;
	void attachPrefix(int k, const int minus);
	int RSQ(int prefix, int begin, int end);
	int SmallSumBeg(int elementsNumber, node*begin, node *end);

	int ilona(int l, int r);

private:
	int SizeOfTree;
	int FirstLeaf;
	int range;
	vector<int>copy;
	vector<int>Compressed;
	void Calc();
};

class node{
public:
	int Value;
	int sum;
	node* rightChild;
	node* leftChild;
	bool leaf;
	int index;
	int first;
	int last;

	//int CalcSum();//for the first time
	void RecalcSum();//all next times
	node(int i);
	//node(node* left, node* right, int i);
	void ChangeLeaf(int indexToChange, const int FirstLeaf, const int minus);
	node(node* example);
	int RSQ(int begin, int end);
	int Sum(int begin, int end);
};


node::node(int i){//leaf
	index = i;
	leaf = true;
	Value = 0;
	rightChild = NULL;
	leftChild = NULL;
	last = first = i;
	sum = 0;
}

/*int node::CalcSum(){
	if (!leaf)
		Value = rightChild->CalcSum() + leftChild->CalcSum();
	return Value;
}*/

node::node(node* example){
	Value = example->Value;
	leaf = example->leaf;
	first = example->first;
	last = example->last;
	index = example->index;
	rightChild = example->rightChild;
	leftChild = example->leftChild;
	sum = example->sum;
}


void node::RecalcSum(){
	if (!leaf){
		Value = rightChild->Value + leftChild->Value;
		sum = rightChild->sum + leftChild->sum;
	}
}



void node::ChangeLeaf(int indexToChange, const int FirstLeaf, const int minus){
	node* newNode;
	int i = 0;
	int index = indexToChange - FirstLeaf;
	if (leaf){
		Value--;
		//i = *std::lower_bound(Compressed.begin(), Compressed.end(), index);// - Compressed.begin();
		sum -= minus;
	}
	else if (indexToChange > leftChild->last){
		newNode = new node(rightChild);
		newNode->ChangeLeaf(indexToChange, FirstLeaf, minus);
		rightChild = newNode;
	}
	else {
		newNode = new node(leftChild);
		newNode->ChangeLeaf(indexToChange, FirstLeaf, minus);
		leftChild = newNode;
	}
	RecalcSum();
}


void PersistentTree::attachPrefix(int k, const int minus){//prefix with k elements(we are eliminating k-th element
	int index = Compressed[k];//have to decrement
	//cout << "we lost  " << index << endl;
	node *newNode = new node(roots[roots.size() - 1]);//create a copy of a previous version
	newNode->ChangeLeaf(FirstLeaf + index, FirstLeaf, minus);
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
		//nodes[i].sum = nodes[i].rightChild->index * nodes[i].rightChild->Value + 

		/*if (nodes[i].rightChild->leaf){
			nodes[i].rightChild->sum = (nodes[i].rightChild->index - FirstLeaf) * nodes[i].rightChild->Value;
			nodes[i].leftChild->sum = (nodes[i].leftChild->index - FirstLeaf) * nodes[i].leftChild->Value;
		}*/
		nodes[i].sum = nodes[i].rightChild->sum + nodes[i].leftChild->sum;
	}
}


/*int comp(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}*/


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
		(nodes[FirstLeaf + Compressed[i]].Value)++;
		(nodes[FirstLeaf + Compressed[i]].sum) += arr[i];//неведомая хрень
	}

	Calc();

	roots.push_back(&nodes[0]);

	for (int l = arr.size() - 1; l >= 0; l--)
		attachPrefix(l, arr[l]);
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

int node::Sum(int begin, int end){
	if (begin == first && end == last)
		return sum;
	else if (begin > leftChild->last)
		return rightChild->Sum(begin, end);
	else if (end < rightChild->first)
		return leftChild->Sum(begin, end);
	else
		return leftChild->Sum(begin, end) + rightChild->Sum(begin, end);
}


int PersistentTree::SmallSumBeg(int elementsNumber, node* begin, node* end){//найти префикс, значение rsq на котором будет равно 10% и извлечь sum
	if (begin->leaf){
		int V = end->Value - end->Value;
		int S = end->sum - end->sum;
		int i = begin->index - FirstLeaf;
		if (elementsNumber < V){
			while (elementsNumber < V){
				V--;
				S -= i;
			}
		}
		return S;
	}
	if (elementsNumber == end->leftChild->Value - begin->leftChild->Value)
		return end->leftChild->sum - begin->leftChild->sum;
	if (elementsNumber < end->leftChild->Value - begin->leftChild->Value)
		return SmallSumBeg(elementsNumber, begin->leftChild, end->leftChild);
	else
		return (end->leftChild->sum - begin->leftChild->sum) + SmallSumBeg(elementsNumber - (end->leftChild->Value - begin->leftChild->Value), begin->rightChild, end->rightChild);
}


int PersistentTree::ilona(int l, int r){


	node* endOfSubsection = roots[roots.size() - 1 - r];
	node* beginOfSubsection = roots[roots.size() - 1 - l + 1];

	int n = r - l + 1;

	int tenPerc = n * 1 / 10;
	int nintyPerc = n - tenPerc;
	int BigSum = SmallSumBeg(nintyPerc, beginOfSubsection, endOfSubsection);
	int q = SmallSumBeg(tenPerc, beginOfSubsection, endOfSubsection);

	BigSum -= q;


	return BigSum / (n - 2 * tenPerc);
}
