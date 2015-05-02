#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <fstream>

using std::cout;
using std::cin;
using std::vector;
using std::endl;

#define MAX_VALUE 1000

class node;

class PersistentTree{
public:
	vector<node> nodes;
	vector<node*> roots;
	//node** nodes;
	//node** roots;

	PersistentTree(vector<long long> & arr);
	long long Statistic(long long i, long long begin, long long end);
	long long statisticForTwoPrefixes(long long i, node* begin, node* end);;
	void attachPrefix(long long k, const long long minus);
	long long RSQ(long long prefix, long long begin, long long end);
	long long SmallSumBeg(long long elementsNumber, node*begin, node *end);

	long long ilona(long long l, long long r);

private:
	long long SizeOfTree;
	long long FirstLeaf;
	long long range;
	vector<long long>copy;
	vector<long long>Compressed;
	void Calc();
};

class node{
public:
	long long Value;
	long long sum;
	node* rightChild;
	node* leftChild;
	bool leaf;
	long long index;
	long long first;
	long long last;

	//long long CalcSum();//for the first time
	void RecalcSum();//all next times
	node(long long i);
	//node(node* left, node* right, long long i);
	void ChangeLeaf(long long indexToChange, const long long FirstLeaf, const long long minus);
	node(node* example);
	long long RSQ(long long begin, long long end);
	long long Sum(long long begin, long long end);
};


node::node(long long i){//leaf
	index = i;
	leaf = true;
	Value = 0;
	rightChild = NULL;
	leftChild = NULL;
	last = first = i;
	sum = 0;
}


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



void node::ChangeLeaf(long long indexToChange, const long long FirstLeaf, const long long minus){
	node* newNode;
	long long i = 0;
	long long index = indexToChange - FirstLeaf;
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


void PersistentTree::attachPrefix(long long k, const long long minus){//prefix with k elements(we are eliminating k-th element
	long long index = Compressed[k];//have to decrement
	//cout << "we lost  " << index << endl;
	node *newNode = new node(roots[roots.size() - 1]);//create a copy of a previous version
	newNode->ChangeLeaf(FirstLeaf + index, FirstLeaf, minus);
	roots.push_back(newNode);
}

void PersistentTree::Calc(){

	for (long long i = FirstLeaf - 1; i >= 0; i--){
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


/*long long comp(const void * a, const void * b)
{
return (*(long long*)a - *(long long*)b);
}*/


PersistentTree::PersistentTree(vector<long long> & arr){

	copy.resize(arr.size());
	Compressed.resize(arr.size());

	std::copy(arr.begin(), arr.end(), copy.begin());

	std::sort(copy.begin(), copy.end());



	for (long long i = 0; i < arr.size(); i++){
		Compressed[i] = std::lower_bound(copy.begin(), copy.end(), arr[i]) - copy.begin();
	}

	range = std::lower_bound(copy.begin(), copy.end(), (*(copy.end() - 1))) - copy.begin();

	long long p = 0;

	while ((1 << p) < range) p++;
	FirstLeaf = ((1 << p) - 1);
	p++;

	SizeOfTree = (1 << p);

	nodes.resize(SizeOfTree, node(0ll));

	for (long long i = FirstLeaf; i < SizeOfTree; i++){
		nodes[i].index = i;
		nodes[i].last = nodes[i].first = i;
	}

	for (long long i = 0; i < Compressed.size(); i++){
		(nodes[FirstLeaf + Compressed[i]].Value)++;
		(nodes[FirstLeaf + Compressed[i]].sum) += arr[i];//неведомая хрень
		//nodes[FirstLeaf + Compressed[i]].original = arr[i];
	}

	Calc();

	roots.push_back(&nodes[0]);

	for (long long l = arr.size() - 1; l >= 0; l--)
		attachPrefix(l, arr[l]);
}

long long PersistentTree::Statistic(long long i, long long begin, long long end){//задача найти префикс, количество rsq на котором будет k-1
	//begin++;
	//end--;
	node* endOfSubsection = roots[roots.size() - 1 - end];
	node* beginOfSubsection = roots[roots.size() - 1 - begin];
	long long res = statisticForTwoPrefixes(i, beginOfSubsection, endOfSubsection);
	return copy[res];
}



long long PersistentTree::statisticForTwoPrefixes(long long i, node* begin, node* end){
	if (begin->leaf)
		return begin->index - FirstLeaf;
	if (i >= end->leftChild->Value - begin->leftChild->Value)
		return statisticForTwoPrefixes(i - (end->leftChild->Value - begin->leftChild->Value), begin->rightChild, end->rightChild);
	else
		return statisticForTwoPrefixes(i, begin->leftChild, end->leftChild);
}


long long PersistentTree::RSQ(long long prefix, long long begin, long long end){
	return roots[roots.size() - prefix]->RSQ(begin, end);
}

long long node::RSQ(long long begin, long long end){
	if (begin == first && end == last)
		return Value;
	else if (begin > leftChild->last)
		return rightChild->RSQ(begin, end);
	else if (end < rightChild->first)
		return leftChild->RSQ(begin, end);
	else
		return leftChild->RSQ(begin, end) + rightChild->RSQ(begin, end);
}

long long node::Sum(long long begin, long long end){
	if (begin == first && end == last)
		return sum;
	else if (begin > leftChild->last)
		return rightChild->Sum(begin, end);
	else if (end < rightChild->first)
		return leftChild->Sum(begin, end);
	else
		return leftChild->Sum(begin, end) + rightChild->Sum(begin, end);
}


long long PersistentTree::SmallSumBeg(long long elementsNumber, node* begin, node* end){//найти префикс, значение rsq на котором будет равно 10% и извлечь sum
	if (begin->leaf){
		long long V = end->Value - begin->Value;
		long long S = end->sum - begin->sum;
		long long i = begin->index - FirstLeaf;
		if (elementsNumber < V){
			while (elementsNumber < V){
				V--;
				S -= end->sum / end->Value;
			}
		}
		return S;
	}
	if (elementsNumber == end->leftChild->Value - begin->leftChild->Value)
		return end->leftChild->sum - begin->leftChild->sum;
	if (elementsNumber > end->leftChild->Value - begin->leftChild->Value)
		return (end->leftChild->sum - begin->leftChild->sum) + SmallSumBeg(elementsNumber - (end->leftChild->Value - begin->leftChild->Value), begin->rightChild, end->rightChild);
	else if (elementsNumber < end->leftChild->Value - begin->leftChild->Value)
		return SmallSumBeg(elementsNumber, begin->leftChild, end->leftChild);
}


long long PersistentTree::ilona(long long l, long long r){

	node* endOfSubsection = roots[roots.size() - 1 - r];
	node* beginOfSubsection = roots[roots.size() - 1 - l + 1];

	long long n = r - l + 1;

	long long tenPerc = n * 1 / 10;
	long long nintyPerc = n - tenPerc;
	long long BigSum = SmallSumBeg(nintyPerc, beginOfSubsection, endOfSubsection);
	long long q = SmallSumBeg(tenPerc, beginOfSubsection, endOfSubsection);

	BigSum -= q;

	//cout << BigSum << endl;
	return BigSum / (n - 2 * tenPerc);
}



void stupid()
{
	long long n, m;
	cin >> n >> m;
	vector <long long> a(n);
	for (long long i = 0; i < n; ++i)
		cin >> a[i];
	for (long long j = 0; j < m; ++j)
	{
		long long l, r;
		cin >> l >> r;
		long long len = r - l + 1;
		long long t = len / 10;

		vector<long long>copy(len);
		std::copy(a.begin() + l - 1, a.begin() + r, copy.begin());
		std::sort(copy.begin(), copy.end());
		long long romRes = copy[(copy.size() - 1) / 2];
		long long s = 0;
		for (long long k = t; k < len - t; k++){
			s += copy[k];
			//cout << s << ' ';
		}
		long long res = s / (len - 2 * t);
		cout << abs(res - romRes) << endl;
	}
}

int main(){
	/*freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	stupid();
	return 0;
	*/
	vector<long long> a;
	long long n, m, l, r;
	std::ifstream input;
	input.open("input.txt");

	std::ofstream output;
	output.open("output.txt");

	input >> n >> m;

	a.reserve(n);

	long long c;

	for (long long i = 0; i < n; i++){
		input >> c;
		a.push_back(c);
	}


	PersistentTree tr(a);

	for (long long i = 0; i < m; i++){
		input >> l >> r;

		long long roman = tr.Statistic((r - l) / 2, l - 1, r);
		long long ilona = tr.ilona(l, r);
		long long res = abs(ilona - roman);

		output << res << std::endl;
	}
	return 0;
	//for (long long i = 0; i < MAX_VALUE; i++){
	//	c = rand() % 2;
	//	a.push_back(c);
	//}
	//PersistentTree tr(a);


	///*for (long long i = 0; i < MAX_VALUE; i++){

	//	cout << a[i] << ' ';
	//}*/
	//cout << endl;

	//long long len, t;

	//for (long long i = 0; i < MAX_VALUE; i++){
	//	r = rand() % MAX_VALUE + 1;
	//	l = rand()% r + 1;
	//	//cout << l << ' ' << r << ' ' << tr.ilona(l, r) << endl;
	//	len = r - l + 1;
	//	t = len / 10;

	//	vector<long long>copy(len);
	//	std::copy(a.begin() + l - 1, a.begin() + r, copy.begin());
	//	std::sort(copy.begin(), copy.end());
	//	long long romRes = copy[(copy.size() - 1) / 2];
	//	long long s = 0;
	//	for (long long k = t; k < len - t; k++){
	//		s += copy[k];
	//		//cout << s << ' ';
	//	}
	//	long long res = s / (len - 2 * t);
	//	long long il = tr.ilona(l, r);
	//	if (res != il)  cout << "!0";
	//	else if (romRes != tr.Statistic((r - l) / 2, l - 1, r)) cout << "!!0";
	//	else cout << '1';
	//	cout << endl;// << s / (len - 2 * t) << endl;
	//}
	//getchar();
	//return 0;
}