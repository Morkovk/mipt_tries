#include <iostream>
using namespace std;

class node;

class Tree{
public:
	node** arr;
	int SizeOfTree;
	int height;

	int t; //coeffcient
	int modul; //modul
	int FirstLeaf;
	int LastLeaf;

	Tree(string str);
	bool IsPal(int begin, int end);
	void SetAt(char a, int i);
	void RecalcLeftPolValue(int i);
	void RecalcRightPolValue(int i);
	int LeftPol(int i, int begin, int end);
	int RightPol(int i, int begin, int end);
};


class node{
public:
	int index;//index
	int letter;//if leaf
	double LeftPolValue;//beginning from the left //if node
	double RightPolValue;//beginning from the right //if node

	int first;//first leaf
	int last;//last leaf

	node(char a, int i);//leaf
	node(int i, node* a, node *b, int t, int modul);//not leaf
};

bool StupidIsPal(string str, int begin, int end);
void CreateTest();