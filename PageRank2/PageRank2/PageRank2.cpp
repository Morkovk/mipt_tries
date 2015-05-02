#include <vector>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <utility>
#include <map> 
#include <set>
#include <string>
#include <iterator>
#include <algorithm>


#define D 0.85
#define WordsToPrint 50

using std::vector;
using std::pair;
using std::cout;
using std::cin;
using std::string;
using std::pair;
using std::map;
using std::set;

typedef set<string> links;
typedef pair <int, string> indexedWord;
typedef pair <string, int> indexedWordR;
typedef pair <string, double> weightedWord;


const double eps = 1E-16;

double iteration(const vector<vector<int>> &edges, vector <pair <double, int>> & weights, int size, double d){
	double res = 0;
	vector<double> newVal(size, 0);
	for (int i = 0; i < size; i++){
		double val = (double)weights[i].first / (double)weights[i].second;
		for (int j = 0; j < size; j++){
			if (i != j){
				if (edges[i][j])
					newVal[j] += val;
			}
		}
	}

	for (int i = 0; i < size; i++){
		//res += (V[i].first - (1 - d + d * newVal[i]));
		//cout << res << ' ';
		weights[i].first = 1 - d + d * newVal[i];
	}
	//cout << std::endl;
	return res;
}

void PageRank(vector <pair<double, int>> & weights, vector <vector<int>> & edges, int size){//первичный ключ i по V и по edges - один и тот же

	double d = D;

	const double init = 1;
	int count = 0;
	for (int i = 0; i < size; i++){
		count = 0;
		for (int j = 0; j < size; j++){
			if (edges[i][j] && i != j)
				count++;
		}
		weights[i] = std::make_pair(init, count);
	}

	double diff = init;
	int s = 0;

	while (abs(diff) > eps)
	{
		diff = iteration(edges, weights, size, d);
		s++;
	}

}


int correct(string &s){
	int last = s.size() - 1;

	while (s[last] == '\n' && last >= 0){
		s.resize(last);
		last--;
	}
	if (last < 0)
		return -1; 

	while (s[0] == '\n' && s.size() > 0){
		s.erase(s.begin());
	}
	if (s.size() == 0)
		return -1;

	if (s[0] == '\'' || s[0] == '\"')
		s.erase(s.begin());

	if (s[s.size() - 1] == '\'' || s[s.size() - 1] == '\"')
		s.resize(s.size() - 1);

	if (s[s.size() - 1] == '.' || s[s.size() - 1] == ';' || s[s.size() - 1] == ',' || s[s.size() - 1] == '!' || s[s.size() - 1] == '?' || s[s.size() - 1] == ':' || s[s.size() - 1] == ','){
		if (s[s.size() - 2] == '.')
			s.resize(s.size() - 3);
		else
			s.resize(s.size() - 1);
	}

	s[0] = tolower(s[0]);

}


void GenerateGr(vector<vector<int>> & edges, int n, map <string, links> & words, map <string, int> & indexedWordsR){
	int i = 0;

	map <string, links>::iterator j = words.begin();

	map<string, int>::iterator k = indexedWordsR.begin();
	while (k != indexedWordsR.end()){
		links::iterator l = words[(*k).first].begin();
		while ( l != words[(*k).first].end()){
			edges[(*k).second][indexedWordsR[(*l)]] = 1;
			l++;
		}
		k++;
	}
}

static bool sortPair(const pair<string, double> &a, const pair <string, double> &b){
	return a.second < b.second;
}


int myread(map <string, links> & words, string file){
	
	std::ifstream f;
	f.open(file);

	std::string s1, s2, s3, s4, s5, s;

	int n = 0;


	while (std::getline(f, s, ' ')){

		if (correct(s) == -1)
			continue;

		
		if (!words.count(s)){
			n++;
			links a;
			words.insert(std::make_pair(s, a));
		}

		if (n > 5) {
			words[s5].insert(s);
			words[s].insert(s5);
		}
		if (n > 4) {
			words[s4].insert(s);
			words[s].insert(s4);
			s5 = s4;
		}
		if (n > 3) {
			words[s3].insert(s);
			words[s].insert(s3);
			s4 = s3;
		}
		if (n > 2) {
			words[s2].insert(s);
			words[s].insert(s2);
			s3 = s2;
		}
		if (n > 1){
			words[s1].insert(s);
			words[s].insert(s1);
			s2 = s1;
		}
		s1 = s;
	}

	return n;

}


int main(){

	std::ofstream f;
	f.open("output.txt");

	string file = "input2.txt";

	map <string, links> words;

	int n = myread(words, file);
	

	vector<vector<int>> edges(n, vector<int>(n, 0));


	map <int, string> indexedWords;

	map <string, int> indexedWordsR;

	map <string, links>::iterator j = words.begin();
	int i = 0;
	while (j != words.end()){
		indexedWord a = std::make_pair(i, j->first);
		indexedWords.insert(a); 
		indexedWordR b = std::make_pair(j->first, i);
		indexedWordsR.insert(b);
		j++;
		i++;
	}
	

	GenerateGr(edges, n, words, indexedWordsR);

	vector <pair<double,int>> weights(n);//weight + number of edges

	PageRank(weights, edges, n);

	//map<double, int> weights;
	//need: word + weight
	vector<weightedWord> weightedWords;

	for (int i = 0; i < n; i++){
		weightedWords.push_back(std::make_pair(indexedWords[i], weights[i].first));
	}

	std::sort(weightedWords.begin(), weightedWords.end(), sortPair);

	int m = (n > WordsToPrint) ? n - WordsToPrint : 0;
	for (int i = n - 1; i >= m; i--){
		f << weightedWords[i].first << std::endl;
	}

	//getchar();
}
