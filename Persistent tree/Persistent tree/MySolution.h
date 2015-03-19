#include <vector>

using std::vector;


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

void CreateTest();
