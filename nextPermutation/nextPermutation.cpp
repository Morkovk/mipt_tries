#include <iostream>
#include <algorithm>


using namespace std;


class node {
public:
	int priority;
	node * left, *right;
	int c; //размер дерева (правое + левое + корень)
	int data;

	node(){
		priority = rand();
		left = right = NULL;
		c = 1;
		HaveToReverse = 0;
	}
	node(int a){
		priority = rand();
		left = right = NULL;
		c = 1;
		data = a;
		RSQValue = data;
		suffix = 1;
		leftest = rightest = this;
		HaveToReverse = 0;
	}
	void merge(item &tree, item first, item second);
	void split(item tree, int i, item & left, item & right);
	void insert(item & tree, int i, item it);
	int RSQ(int begin, int end);
	item Find(int i);
	void Set(int i, int newValue);
	void UpdateSuffix();
	void UpdatePrefix();
	int CountSuffix(int begin, int end);
	void UpdateLeftestRightest();
	item FindNextBigger(int begin, int end, int value);
	void NextPermutation(item & tree, int begin, int end);
	void ReverseSubSection(item tree, int begin, int end);
	item FindSubSection(int begin, int end);
	void Push();
	void RecalculateTree();
	int sizeofRSQ(item x);
	int sizeofSuffix(item x);
	int sizeofPrefix(item x);
	int sizeofC(item x);

};

typedef node * item;




class Tree{
	node* root;
	item leftest;//the leftes child in a tree
	item rightest;//the rightest child in a tree
	int suffix;//dropping
	int prefix;//rising
	int RSQValue;
	bool HaveToReverse;
};



void node::UpdateLeftestRightest(){
	if ((!left) || (!right)){
		if (!right)
			rightest = this;
		if (!left)
			leftest = this;
	}
	else{
		leftest = left->leftest;
		rightest = right->rightest;
	}
}

void node::Push(){
	if (HaveToReverse){
		if (left)
			left->HaveToReverse = !left->HaveToReverse;
		if (right)
			right->HaveToReverse = !right->HaveToReverse;
		swap(left, right);
		swap(suffix, prefix);
	}
}

void node::UpdatePrefix(){
	prefix = sizeofPrefix(left);
	if (sizeofPrefix(left) == sizeofC(left)){
		if ((!left) || (data > left->rightest->data)){
			prefix += 1;
			if ((!right) || (data < right->leftest->data))
				prefix += sizeofPrefix(right);
		}
	}
}

void node::UpdateSuffix(){
	suffix = sizeofSuffix(right);
	if (sizeofPrefix(left) == sizeofC(left)){
		if ((!right) || (data < right->leftest->data)){
			suffix += 1;
			if ((!left) || (data > left->rightest->data))
				suffix += sizeofPrefix(right);
		}
	}
}

void node::merge(item &tree, item first, item second) {

	if (!first || !second){
		if (!first)
			tree = second;
		else
			tree = first;
	}
	else{
		first->Push();
		second->Push();
		if (first->priority > second->priority){
			merge(first->right, first->right, second);
			tree = first;
		}
		else{
			merge(second->left, first, second->left);
			tree = second;
		}
		tree->RecalculateTree();
	}
	
}

void node::split(item tree, int i, item & left, item & right) {//cut 5 elements from the left side
	Push();
	if (i <= tree->left->c){
		split(tree->left, i, left, tree->left);
		right = tree;
	}
	else{
		split(tree->right, i - tree->c - 1, tree->right, right);
		left = tree;
	}
	left->RecalculateTree();
	right->RecalculateTree();
}

void node::insert(item & tree, int i, item it) {
	Push();
	item l;
	item r;
	split(tree, i, l, r);
	merge(l, l, it);
	merge(tree, l, r);
	tree->RecalculateTree();//can be removed, split, merge do it automatically
}



int node::RSQ(int begin, int end){
	Push();
	if (begin < 0)
		begin = 0;
	if ((!this) || (end < 0) || (begin > c))
		return 0;

	if (begin > sizeofC(left) + 1)
		return right->RSQ(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	if (end < sizeofC(left))
		return left->RSQ(begin, end);
	return left->RSQ(begin, end) + data + right->RSQ(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
}


item node::Find(int i){
	Push();
	if (i == sizeofC(left))
		return this;
	else if (sizeofC(left) > i)
		left->Find(i);
	else
		right->Find(i);
}

int node::CountSuffix(int begin, int end){
	Push();
	if (begin < 0)
		begin = 0;
	if ((end < 0) || (!this) || (begin > c))
		return 0;

	int LongestSuffix;

	if (begin > sizeofC(left) + 1)
		return right->CountSuffix(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	if (end < sizeofC(left))
		return left->CountSuffix(begin, end);
	
	if (right)
		LongestSuffix = right->CountSuffix(begin - sizeofC(left) - 1, end - sizeofC(left) - 1);
	if ((!right) || ((LongestSuffix == end - sizeofC(left) - 1) && (data > right->leftest->data))){
		LongestSuffix += 1;
		if ((left) && (data < left->rightest->data))
			LongestSuffix += left->CountSuffix(begin, c);
	}
	return LongestSuffix;
}



void node::Set(int i, int newValue){
	Push();
	if (i == sizeofC(left)){
		data = newValue;
	}
	else if (sizeofC(left) > i){
		left->Set(i, newValue);
	}
	else{
		right->Set(i, newValue);
	}
	RecalculateTree();
}


int node::sizeofC(item x){
	if (x)
		return x->c;
	else
		return 0;
}

int node::sizeofRSQ(item x){
	if (x)
		return x->RSQ;
	else
		return 0;
}

int node::sizeofSuffix(item x){
	if (x)
		return x->suffix;
	else
		return 0;
}

int node::sizeofPrefix(item x){
	if (x)
		return x->prefix;
	else
		return 0;
}


void node::RecalculateTree(){
	Push();
	UpdateLeftestRightest;
	UpdatePrefix;
	UpdateSuffix;
	RSQ(0, c);
	c = sizeofC(right) + sizeofC(left) + 1;
}

void node::ReverseSubSection(item tree, int begin, int end){
	Push();
	item left;
	item Subsection;
	item right;
	split(tree, end + 1, Subsection, right);
	split(Subsection, begin, left, Subsection);
	Subsection->HaveToReverse = !Subsection->HaveToReverse;
	merge(Subsection, left, Subsection);
	merge(tree, Subsection, right);

}


item node::FindNextBigger(int begin, int end, int value){
	Push();
	if (begin > sizeofC(left) + 1)
		return right->FindNextBigger(begin - sizeofC(left) - 1, end - sizeofC(left) - 1, value);
	else if (end < sizeofC(left))
		return left->FindNextBigger(begin, end, value);
	else{
		item next;
		if (data <= value){
			return left->FindNextBigger(begin, sizeofC(left), value);
		}
		else {
			next = this;
			if ((!right) || (right->leftest->data <= value))
				return this;
			else
				return right->FindNextBigger(0, end - sizeofC(left) - 1, value);
		}
	}	
}

void node::NextPermutation(item & tree, int begin, int end){
	int suffix = tree->CountSuffix(begin, end);
	item ToChange = tree->Find(end - suffix);
	item Next = FindNextBigger(end - suffix + 1, end, ToChange->data);
	swap(ToChange, Next);
	ReverseSubSection(tree, end - suffix + 1, end);
}


int main(){
	int number = 1000;
	int *arr = new int[number];
	for (int i = 0; i < number; i++)
		arr[i] = rand();


	item tree = &node(arr[0]);


	for (int i = 1; i < number; i++)
		tree->insert(tree, i, &node(arr[i]));

	
}
