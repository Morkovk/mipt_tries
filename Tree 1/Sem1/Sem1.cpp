#include <iostream>
#include <algorithm>


using namespace std;
class node {
public:
	int priority;
	node * left, *right;
	int c; //размер дерева (правое + левое + корень)
	int data;
	item leftest;//the leftes child in a tree
	item rightest;//the rightest child in a tree
	int suffix;//dropping
	int prefix;//rising
	int RSQValue;
	bool HaveToReverse;
	node(){
		priority = rand() % 100 - 1;
		left = right = NULL;
		c = 1;
	}
	node(int a){
		priority = rand() % 100 - 1;
		left = right = NULL;
		c = 1;
		data = a;
		RSQValue = data;
		suffix = 1;
		leftest = rightest = this;
	}
	void merge(item &tree, item first, item second);
	void split(item tree, int i, item & left, item & right);
	void insert(item & tree, int i, item it);
	int RSQ(int begin, int end);
	item Find(int i);
	void Set(int i, int newValue);
	void UpdateSuffix();
	void UpdatePrefix();
	item UpdateLeftestRightest();
	void NextPermutation(item & tree, int begin, int end);
	void Push();
};

typedef node * item;

item node::UpdateLeftestRightest(){
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
	if ((this) && (HaveToReverse)){
		if (left)
			left->HaveToReverse = !left->HaveToReverse;
		if (right)
			right->HaveToReverse = !right->HaveToReverse;
		swap(left, right);
	}
}

void node::UpdatePrefix(){
	Push();
	if (!this)
		prefix = 0;
	else if (left->prefix == left->c){
		prefix = left->prefix;
		if (data > right->leftest->data){
			prefix += 1;
			if (data < left->rightest->data)
				prefix += right->prefix;
		}
	}
}

void node::UpdateSuffix(){
	Push();
	if (!this)
		suffix = 0;
	else if (right->suffix == right->c){
		suffix = right->suffix;
		if (data > right->leftest->data){
			suffix += 1;
			if (data < left->rightest->data)
				suffix += left->suffix;
		}
	}
}

void node::merge(item &tree, item first, item second) {
	first->Push;
	second->Push();
	if (!first || !second){
		if (!first)
			tree = second;
		else
			tree = first;
	}
	else if (first->priority > second->priority){
		merge(first->right, first->right, second);
		tree = first;
		tree->c = tree->right->c + tree->left->c + 1;
		tree->RSQ = tree->right->RSQ + tree->left->RSQ + tree->data;
		tree->UpdateSuffix();
		tree->UpdatePrefix();
	}
	else{
		merge(second->left, first, second->left);
		tree = second;
		tree->c = tree->left->c + tree->right->c + 1;
		tree->RSQ = tree->right->RSQ + tree->left->RSQ + tree->data;
		tree->UpdateSuffix();
		tree->UpdatePrefix();
	}
}

void node::split(item tree, int i, item & left, item & right) {//cut 5 elements from the left side
	/*if (!tree)
	left = right = NULL;
	else if (i == tree->left->c){
		left = tree->left;
		right = tree;
		right->left = NULL;
		right->c = tree->c - tree->left->c;
		right->RSQ = tree->RSQ - tree->left->RSQ;
		right->UpdateSuffix();
	}*/
	Push();
	if (i <= tree->left->c){
		split(tree->left, i, left, tree->left);
		right = tree;
		right->c = right->left->c + right->right->c + 1;
		right->RSQValue = right->left->RSQValue + right->data + right->left->RSQValue;
		right->UpdateSuffix();
		tree->UpdatePrefix();
	}
	else{
		split(tree->right, i - tree->c - 1, tree->right, right);
		left = tree;
		left->c = left->left->c + left->right->c + 1;
		left->RSQValue = left->left->RSQValue + left->data + left->left->RSQValue;
		left->UpdateSuffix();
		tree->UpdatePrefix();
	}

}

void node::insert(item & tree, int i, item it) {
	Push();
	item l;
	item r;
	split(tree, i, l, r);
	merge(l, l, it);
	merge(tree, l, r);
}



int node::RSQ(int begin, int end){
	Push();
	if ((!this) || (end < 0))
		return 0;
	else if (c == end - begin + 1)
		return RSQValue;
	else if (begin <= left->c){
		if (end <= left->c)
			return left->RSQ(begin, end);
		else
			return left->RSQ(begin, end) + data + right->RSQ(begin - left->c - 1, end - left->c - 1);
	}
	else if (begin == left->c + 1)
		return data + right->RSQ(begin - left->c - 1, end - left->c - 1);
	else
		return right->RSQ(begin - left->c - 1, end - left->c - 1);
}


item node::Find(int i){
	Push();
	if (i == left->c)
		return this;
	else if (left->c > i)
		left->Find(i);
	else
		right->Find(i);
}


void node::Set(int i, int newValue){
	Push();
	if (i == left->c){
		RSQValue += (newValue - data);
		data = newValue;
		UpdateSuffix();
	}
	else if (left->c > i){
		left->Set(i, newValue);
		RSQValue = left->RSQ + data + left->RSQ;
	}
		
	else{
		right->Set(i, newValue);
		RSQValue = left->RSQ + data + left->RSQ;
	}
}

void node::NextPermutation(item & tree, int begin, int end){

}
