#include <iostream>
#include <algorithm>

#define MAX_DATA 10000

using namespace std;
struct node {
	int priority;
	node * left, *right;
	int c; //размер дерева (правое + левое + корень)
	int data;
	bool HaveToAdd;
	bool HaveToSet;
	int HaveToAddValue;
	int HaveToSetValue;
	int RMQ;
	int RSQ;
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
		RMQ = data;
		RSQ = data;
		HaveToAdd = 0;
		HaveToSet = 0;
	}
};
typedef node * item;


void SetAt(item &tree, int begin, int end, int newValue){
	if (tree != NULL){
		if (tree->c == end - begin + 1){

			SetAt(tree->left, begin, tree->left->c);
			SetAt(tree->right, tree->left->c + 1, end - tree->left->c - 1);
			Push(tree);
			tree->HaveToSet = 1;
			tree->HaveToSetValue = newValue;
		}
	}
}

void Push(item x){
	if (x->HaveToSet > 0){
		x->data = x->HaveToSet;
		x->HaveToSet = 0;
	}
	if (x->HaveToAdd > 0){
		x->data += HaveToAddValue;
		x->HaveToAdd = 0;
	}
}

void merge(item & tree, item first, item second) {
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
		tree->RMQ = min(tree->right->RMQ, tree->left->RMQ, tree->data);
	}
	else{
		merge(second->left, first, second->left);
		tree = second;
		tree->c = tree->left->c + tree->right->c + 1;
		tree->RSQ = tree->right->RSQ + tree->left->RSQ + tree->data;
		tree->RMQ = min(tree->right->RMQ, tree->left->RMQ, tree->data);
	}
}

void split(item tree, int i, item & left, item & right) {//cut 5 elements from the left side
	/*if (!tree)
		left = right = NULL;
	else */if (i == tree->left->c){
		left = tree->left;
		right = tree;
		right->left = NULL;
		right->c = tree->c - tree->left->c;
		right->RMQ = min(right->data, right->right->data);
		right->RSQ = right->data + right->right->data;
	}
	else if (i < tree->left->c){
		split(tree->left, i, left, tree->left);
		right = tree;
		right->c = right->left->c + right->right->c + 1;
		right->RMQ = min(right->left->data, right->right->data);
		right->RSQ = min(right->left->data, right->right->data);
	}
	else{
		split(tree->right, i - tree->c - 1, tree->right, right);
		left = tree;
		left->c = left->left->c + left->right->c + 1;
		left->RMQ = min(right->left->data, right->right->data);
		left->RSQ = min(right->left->data, right->right->data);
	}
	
}

void insert (item & tree, int i, item it) {
	item l;
	item r;
	split(tree, i, l, r);
	merge(l, l, it);
	merge(tree, l, r);
}

void remove(item &tree, int i){
	item l;
	item deleted;
	item r;
	split(tree, i, l, r);
	split(r, 1, deleted, r);
	merge(tree, l, r);
}

void shift(item &tree, int begin, int end, int shiftBy){
	int mid = end - shiftBy;
	item left, right, firstPart, secondPart;
	split(tree, end, tree, right);
	split(tree, mid, tree, secondPart);
	split(tree, begin, left, firstPart);

	merge(tree, left, secondPart);
	merge(tree, tree, firstPart);
	merge(tree, tree, secondPart);
}

void Set(item &tree, int i, int newValue){
	item itemToChange = Find(tree, i);
	itemToChange->RSQ += (newValue - itemToChange->data);
	itemToChange->RMQ = min(itemToChange->RMQ, newValue);
	itemToChange->data = newValue;
}

void Add(item &tree, int i, int addValue){
	item itemToChange = Find(tree, i);
	itemToChange->RSQ += addValue;
	itemToChange->RMQ = min(itemToChange->left->data, itemToChange->right->data);
	itemToChange->data = addValue;
}


item Find(item tree, int i){
	if (i == tree->left->c)
		return tree;
	else if (tree->left->c > i)
		Find(tree->left, i);
	else
		Find(tree->right, i);
}


void SetAt(item &tree, int begin, int end, int newValue){
	
}

int RMQ(item tree, int begin, int end){
	if (!tree)
		return MAX_DATA;
	if (tree->left->c + 1 < begin)
		return RMQ(tree->right, begin - tree->left->c - 1, end - tree->left->c - 1);
	else if (tree->left->c > end)
		return RMQ(tree->left, begin, end);
	else if (tree->left->c + 1 == end){
		return min(RMQ(tree->left, begin, end - 1), tree->data);
	}
	else{
		return min(RMQ(tree->left, begin, tree->left->c), tree->data, RSQ(tree->right, 0, end - tree->left->c - 1));
	}
}

int RSQ(item tree, int begin, int end){
	if (!tree)
		return 0;
	else if (tree->left->c + 1 < begin)
		return RSQ(tree->right, begin - tree->left->c - 1, end - tree->left->c - 1);
	else if (tree->left->c > end)
		return RSQ(tree->left, begin, end);
	else if (tree->left->c + 1 == end){
		return RSQ(tree->left, begin, end - 1) + tree->data;
	}
	else{
		return RSQ(tree->left, begin, tree->left->c) + tree->data + RSQ(tree->right, 0, end - tree->left->c - 1);
	}
}

int main(){

}