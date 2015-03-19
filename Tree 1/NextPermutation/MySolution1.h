#include <string>
#include <iostream>
#include <vector>

using namespace std;//using namespace std;


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
	long long RSQValue;
	bool HaveToReverse;

	node(int a);
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

void StupidNextPermutation(vector <int>&arr, int begin, int end);
int StupidRSQ(vector <int> arr, int begin, int end);
bool cmp(const char &a, const char &b);
bool TreeVsArray(item tr, vector <int> arr);
void CreateTest(int);
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
