#include <string>
#include <iostream>

using namespace std;//using namespace std;
 
using std::cout;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::endl;
using std::ios;
using std::string;
class node;
typedef node * item;

class node {
public:
	int priority;
	item left, right;
	int c; //������ ������ (������ + ����� + ������)
	int data;
	item leftest;//the leftes child in a tree
	item rightest;//the rightest child in a tree
	int suffix;//dropping
	int prefix;//rising
	double RSQValue;
	bool HaveToReverse;
	node();
	node(int a);
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

void StupidNextPermutation(string &str, int begin, int end);
int StupidRSQ(string str, int begin, int end);
bool cmp(const char &a, const char &b);
bool TreeVsArray(item tr, string str);
void StupidSet(string str, int index, int newValue);
void CreateTest();
