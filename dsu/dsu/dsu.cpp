#include <vector>
#include <iostream>
#include <cstdio>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdlib>

using std::vector;
using std::cout;
using std::cin;
using std::endl;


class node;
class PersistentTree;
class DSU;


class node{
public:
	int Value;
	node* rightChild;
	node* leftChild;
	bool leaf;
	int index;
	int first;
	int last;
	bool fictive;

	//node(node* left, node* right, int i);
	void ChangeLeaf(int indexToChange, int newValue);
	node(node* example);
	node(){ fictive = true; }
	int FindInTheVersion(int a);
	//node* FindPtr(int a);
};

class PersistentTree{
public:
	vector<node> nodes;
	vector<node*> roots;
	int FirstLeaf;

	int numberOfEls;

	PersistentTree(int _numberOfEls);
	void addVersion(int version, int index, int newValue);
	void addVersion(int version);


private:
	int SizeOfTree;
	void Calc();
};

class DSU{
public:

	//std::vector<int> parent;
	//std::vector<int> ranks;
	PersistentTree *tr;

	bool AreInOneUnion(int a, int b, int version);
	void Unite(int a, int b, int version);
	DSU(int _numberOfEls);
	int Find(int a, int version);
};

DSU::DSU(int _numberOfEls){
	tr = new PersistentTree(_numberOfEls);
}

int node::FindInTheVersion(int a){
	if (a == index)
		return Value;
	else if (a > leftChild->last)
		return rightChild->FindInTheVersion(a);
	else
		return leftChild->FindInTheVersion(a);
}
/*node* node::FindPtr(int a){
	if (a == index)
		return this;
	else if (a > leftChild->last)
		return rightChild->FindPtr(a);
	else
		return leftChild->FindPtr(a);
}*/


node::node(node* example){
	Value = example->Value;
	leaf = example->leaf;
	first = example->first;
	last = example->last;
	index = example->index;
	rightChild = example->rightChild;
	leftChild = example->leftChild;
}


void node::ChangeLeaf(int indexToChange, int newValue){
	node* newNode;
	if (leaf){
		Value = newValue;
		//cout <<"now are " << Value << endl;
	}
	else if (indexToChange > leftChild->last){
		newNode = new node(rightChild);
		newNode->ChangeLeaf(indexToChange, newValue);
		rightChild = newNode;
	}
	else {
		newNode = new node(leftChild);
		newNode->ChangeLeaf(indexToChange, newValue);
		leftChild = newNode;
	}
}


PersistentTree::PersistentTree(int _numberOfEls){

	numberOfEls = _numberOfEls;

	int p = 0;

	while ((1 << p) < numberOfEls) p++;
	FirstLeaf = ((1 << p) - 1);

	p++;

	SizeOfTree = (1 << p);

	nodes.resize(SizeOfTree);

	for (size_t i = 0; i < numberOfEls; i++){
		nodes[FirstLeaf + i].Value = i;
		nodes[FirstLeaf + i].index = FirstLeaf + i;
		nodes[FirstLeaf + i].first = FirstLeaf + i;
		nodes[FirstLeaf + i].last = FirstLeaf + i;
		nodes[FirstLeaf + i].leftChild = NULL;
		nodes[FirstLeaf + i].rightChild = NULL;
		nodes[FirstLeaf + i].leaf = true;
		nodes[FirstLeaf + i].fictive = false;
	}

	Calc();
	roots.push_back(&nodes[0]);
}

void PersistentTree::addVersion(int version, int index, int newValue){
	node *newNode = new node(roots[version]);//create a copy of a previous version
	newNode->ChangeLeaf(FirstLeaf + index, newValue);
	roots.push_back(newNode);
}
void PersistentTree::addVersion(int version){
	node *newNode = new node(roots[version]);//create a copy of a previous version
	roots.push_back(newNode);
}


void PersistentTree::Calc(){
	for (int i = FirstLeaf - 1; i >= 0; i--){
		nodes[i].rightChild = &nodes[2 * i + 2];
		nodes[i].leftChild = &nodes[2 * i + 1];
		if (nodes[i].rightChild->fictive){
			if (nodes[i].leftChild->fictive)
				nodes[i].fictive = true;
			else{
				nodes[i].fictive = false;
				nodes[i].last = FirstLeaf + numberOfEls;
			}
		}
		else{
			nodes[i].last = nodes[i].rightChild->last;
			nodes[i].fictive = false;
		}
		nodes[i].first = nodes[i].leftChild->first;
		nodes[i].index = i;
		nodes[i].leaf = false;
	}
}




int DSU::Find(int a, int version){
	int parentA = tr->roots[version]->FindInTheVersion(tr->FirstLeaf + a);
	if (parentA != a)
		return Find(parentA, version);
	else
		return a;
}

bool DSU::AreInOneUnion(int a, int b, int version){
	int parentA = Find(a, version);
	int parentB = Find(b, version);
	tr->addVersion(version);
	if (parentA == parentB)
		return 1;
	else return 0;

}

void DSU::Unite(int a, int b, int version){
	int parentA = Find(a, version);
	int parentB = Find(b, version);
	int Order = std::rand() % 2;

	//if (ranks[parentB] > ranks[parentA])
	if (Order)
		std::swap(parentA, parentB);
	tr->addVersion(version, parentB, parentA);
}
 
int const MAXN = 20;

/*void CreateTest(){
	srand(4);
	std::ofstream test;
	test.open("input.txt");
	int numberOfEls, numberOfQueries, operationType, version, a, b;
	char operation;
	numberOfEls = rand() % MAXN + MAXN / 2;
	numberOfQueries = rand() % MAXN + MAXN / 2;

	test << numberOfEls << endl << numberOfQueries << endl;

	for (size_t i = 0; i < numberOfQueries; i++){
		operationType = rand() % 2;
		operation = (operationType ? '?' : '+');
		if (i)
			version = rand() % i;
		else
			version = 0;
		a = rand() % numberOfEls;
		b = rand() % numberOfEls;
		test << operation << ' ' << version << ' ' << a << ' ' << b << endl;
	}
}*/


int main(){

	//CreateTest();

	std::ifstream input;
	input.open("input.txt");

	std::ofstream output;
	output.open("output.txt");

	char operation;
	int version;
	int a, b;
	std::string res;


	int numberOfElements = 0, numberOfQueries = 0;

	input >> numberOfElements;
	input >> numberOfQueries;

	
	DSU newDsu(numberOfElements);

	for (size_t i = 0; i < numberOfQueries; i++){
		input >> operation;
		input >> version;
		input >> a >> b;
		--a, --b;
		switch (operation){
		case '?':{
			res = newDsu.AreInOneUnion(a, b, version) ? "YES" : "NO";
			output << res << endl;
			break;
		}
		case '+':{
			newDsu.Unite(a, b, version);
			break;
		}
		}
	}

}
