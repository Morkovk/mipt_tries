#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class node;
class Tree;




class Tree{
public:
	vector <node> arr;
	unsigned int SizeOfTree;

	int t; //coeffcient
	int modul; //modul
	unsigned int FirstLeaf;
	vector<int> powers;

	Tree(string str);
	bool IsPal(int begin, int end);
	void SetAt(char a, int i);
	void RecalcLeftPolValue(int i);
	void RecalcRightPolValue(int i);
	unsigned int LeftPol(int i, int begin, int end);
	unsigned int RightPol(int i, int begin, int end);
	void buildArr(string & str, int cur, int from, int to);
};


class node{
public:
	int index;//index
	int letter;//if leaf
	unsigned int LeftPolValue;//beginning from the left //if node
	unsigned int RightPolValue;//beginning from the right //if node

	int first;//first leaf
	int last;//last leaf

	node(){};

	/*node(char a, int i);//leaf
	node(int i, node* a, node *b, int t, int modul);//not leaf*/
};


/*node::node(char a, int i){//leaf
	index = i;
	letter = (int)a;
	LeftPolValue = letter;
	RightPolValue = letter;
	first = last = i;
}

node::node(int i, node* leftChild, node* rightChild, int t, int modul){//not leaf
	index = i;
	first = leftChild->first;
	last = rightChild->last;
	unsigned int absoluteLeftPolValue = leftChild->LeftPolValue * pow(2, (rightChild->last - rightChild->first + 1)) + rightChild->LeftPolValue;
	LeftPolValue = absoluteLeftPolValue % modul;
	unsigned int absoluteRightPolValue = rightChild->RightPolValue * pow(2, (leftChild->last - leftChild->first + 1)) + leftChild->RightPolValue;
	RightPolValue = absoluteRightPolValue % modul;
}*/


unsigned int Tree::LeftPol(int i, int begin, int end){
	unsigned int result = 0;
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	if ((begin == (arr[i]).first) && (end == (arr[i]).last))
	{
		result = (arr[i]).LeftPolValue;
		/*long long HASH = 0;
		for (int j = begin; j <= end; ++j)
			HASH = (HASH * 257 + arr[j].LeftPolValue) % modul;
		printf("%d %lld", arr[i].LeftPolValue, HASH);*/
	}
	else if (begin > (arr[LeftChild]).last){
		result = LeftPol(RightChild, begin, end);
	}
	else if (end < (arr[RightChild]).first) {
		result = LeftPol(LeftChild, begin, end);
	}
	else{
		result = LeftPol(RightChild, (arr[RightChild]).first, end) + ((long long)LeftPol(LeftChild, begin, (arr[LeftChild]).last) * powers[end - (arr[RightChild]).first + 1] % modul);
	}
	return (result % modul);
}




unsigned int Tree::RightPol(int i, int begin, int end){
	unsigned int result = 0;
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	if ((begin == (arr[i]).first) && (end == (arr[i]).last))
		result = (arr[i]).RightPolValue;
	else if (begin > (arr[LeftChild]).last){
		result = RightPol(RightChild, begin, end);
	}
	else if (end < (arr[RightChild]).first) {
		result = RightPol(LeftChild, begin, end);
	}
	else{
		result = RightPol(LeftChild, begin, (arr[LeftChild]).last) + ((long long)RightPol(RightChild, (arr[RightChild]).first, end) * powers[arr[LeftChild].last - begin + 1] % modul);
	}
	return (result % modul);
}

void Tree::SetAt(char a, int i){
	int index = FirstLeaf + i;//from zero
	arr[index].letter = (int)a;
	arr[index].LeftPolValue = arr[index].RightPolValue = arr[index].letter;
	do{
		index = (index - 1) / 2;
		RecalcLeftPolValue(index);
		RecalcRightPolValue(index);
	} while (index);
}

void Tree::RecalcLeftPolValue(int i){
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	long long result = ((long long) arr[LeftChild].LeftPolValue * powers[arr[RightChild].last - arr[RightChild].first + 1]) % modul + arr[RightChild].LeftPolValue;
	//printf("%d %d %d\n", arr[LeftChild].LeftPolValue, arr[RightChild].LeftPolValue, arr[RightChild].last - arr[RightChild].first + 1);
	//printf("%d\n", powers[4]);
	arr[i].LeftPolValue = (result % modul);
	long long HASH = 0;
	/*for (int j = arr[i].first; j <= arr[i].last; ++j)
		HASH = (HASH * 257 + arr[j].LeftPolValue) % modul;
	printf("%d %lld\n", arr[i].LeftPolValue, HASH);*/
	/*if (arr[i].LeftPolValue != HASH)
		printf("WAIT!");*/
	return;
}

void Tree::RecalcRightPolValue(int i){
	int LeftChild = 2 * i + 1;
	int RightChild = 2 * i + 2;
	unsigned int result = ((long long) arr[RightChild].RightPolValue * powers[arr[LeftChild].last - arr[LeftChild].first + 1]) % modul + arr[LeftChild].RightPolValue;
	arr[i].RightPolValue = (result % modul);
}

bool Tree::IsPal(int begin, int end){
	int begin1 = FirstLeaf + begin;
	int end1 = FirstLeaf + begin + (end - begin) / 2;
	int begin2 = FirstLeaf + begin + (end - begin + 1) / 2;
	int end2 = FirstLeaf + end;//we must not go outside the word
	if (end1 == begin2 && (begin1 != end1 || begin2 != end2)){
		end1 -= 1;
		begin2 += 1;
	}

	unsigned int LeftHalf = LeftPol(0, begin1, end1);
	unsigned int RightHalf = RightPol(0, begin2, end2);
	if (LeftHalf == RightHalf)
		return 1;
	else
		return 0;
		
	/*long long HASH = 0;
	for (int i = begin; i <= end; ++i)
		HASH = (HASH * 257 + arr[FirstLeaf + i].LeftPolValue) % modul;

	unsigned int LeftHalf = LeftPol(0, FirstLeaf + begin, FirstLeaf + end);
	printf("%lld %d\n", HASH, LeftHalf);
	unsigned int RightHalf = RightPol(0, FirstLeaf + begin, FirstLeaf + end);
	return (LeftHalf == RightHalf);
	*/
}

void Tree::buildArr(string & str, int cur, int from, int to){
	if (from == to) {
		if (FirstLeaf == -1)
			FirstLeaf = cur;
		if (str[from] == '\0')
			arr[cur].letter = 0;
		else
			arr[cur].letter = str[from];
		arr[cur].LeftPolValue = arr[cur].letter;
		arr[cur].RightPolValue = arr[cur].letter;
		arr[cur].first = arr[cur].last = cur;
		arr[cur].index = cur;
		//cout << arr[cur].letter << "   " << cur << endl;
	}
	else{
		int mid = (from + to) / 2;
		int RightChild = 2 * cur + 2;
		int LeftChild = 2 * cur + 1;
		buildArr(str, LeftChild, from, mid);
		buildArr(str, RightChild, mid + 1, to);
		arr[cur].letter = 0;
		arr[cur].first = arr[LeftChild].first;
		arr[cur].last = arr[RightChild].last;
		RecalcLeftPolValue(cur);
		RecalcRightPolValue(cur);
		
		arr[cur].index = cur;
		//cout << arr[cur].letter << "   " << cur << endl;
	}
}



Tree::Tree(string str){
	modul = (1 << 31) - 1;
	t = 257;

	SizeOfTree = str.length();

	FirstLeaf = -1;

	int p = 0;

	while ((1 << p) < SizeOfTree) p++;
	str.resize(1 << p);
	powers.resize((1 << p));
	p++;
	
	SizeOfTree = (1 << p);

	arr.resize(SizeOfTree);
	powers[0] = 1;
	for (int i = 1; i < powers.size(); ++i)
		powers[i] = ((long long) powers[i - 1] * t) % modul;

	buildArr(str, 0, 0, str.length() - 1);
}

/*Tree::Tree(string str){

	modul = pow(2, 31) - 1;
	t = 2;
	
	height = (unsigned int)(log2(double(str.length())) + 1);//height of the tree from 0
	
	SizeOfTree = pow(2, height);

	for (int l = 0; l <= height; l++){
		SizeOfTree += pow(2, l);
	}
	arr = new node*[SizeOfTree];

	unsigned int cur = SizeOfTree;//the last index is SizeOfTree - 1
		
	cur -= (int)pow(2, height);//the beginning of the zero level

	unsigned int i = 0;

	for (i = 0; i < str.length(); i++){
		node *newLeaf = new node(str[i], cur + i);
		arr[cur + i] = newLeaf;
	}
	for (i; i < (int)pow(2, height); i++){
		node *newLeaf = new node(0, cur + i);
		arr[cur + i] = newLeaf;
	}

	for (int k = height - 1; k >= 0; k--){
		cur -= (int)pow(2, k);
		for (unsigned int i = 0; i < (int)pow(2, k); i++){
			node *newNode = new node(cur + i, arr[(cur + i) * 2 + 1], arr[(cur + i) * 2 + 2], t, modul);
			arr[cur + i] = newNode;
			RecalcLeftPolValue(cur + i);
			RecalcRightPolValue(cur + i);
		}
	}
	FirstLeaf = SizeOfTree - (unsigned int)pow(2, height);
}
*/