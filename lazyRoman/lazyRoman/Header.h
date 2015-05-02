#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>

using std::vector;


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

	int CalcSum();//for the first time
	int RecalcSum();//all next times
	node(int i);
	//node(node* left, node* right, int i);
	void ChangeLeaf(int indexToChange, const int FirstLeaf, const int minus);
	node(node* example);
	int RSQ(int begin, int end);
	int Sum(int begin, int end);
};

class PersistentTree{
public:
	vector<node> nodes;
	vector<node*> roots;

	PersistentTree(vector<int> & arr);
	int Statistic(int i, int begin, int end);
	int statisticForTwoPrefixes(int i, node* begin, node* end);
	void attachPrefix(int k, const int minus);
	int RSQ(int prefix, int begin, int end);
	int SmallSumBeg(int elementsNumber, node*begin, node *end);

	int ilona(int l, int r);

private:
	int SizeOfTree;
	int FirstLeaf;
	int range;
	vector<int> copy;
	vector<int> Compressed;
};