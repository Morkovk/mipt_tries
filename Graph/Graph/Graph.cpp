#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>

#define INFINITE  100000
#define FOUND 0
#define NOT_FOUND 1

using std::vector;
using std::cout;
using std::cin;
using std::endl;

int FindWay(int i, int mask, vector <vector <int>> & edges, int n, vector<vector<int>> & IsWayThru){

	if (!mask)
	{
		IsWayThru[i][mask] = (edges[i][0] ? FOUND : NOT_FOUND);
	}
	
	if (IsWayThru[i][mask] != INFINITE)
			return IsWayThru[i][mask];

	

	for (int j = 0; j < n; j++){
		if (edges[i][j] && ((1 << j) & mask)){
			IsWayThru[i][mask] = FindWay(j, mask - (1 << j), edges, n, IsWayThru);
			if (IsWayThru[i][mask] == FOUND){
				return IsWayThru[i][mask];
			}
		}
	}
	IsWayThru[i][mask] = NOT_FOUND;
	return IsWayThru[i][mask];
}

void GenerateGr(int n, vector<vector<int>> & Gr){
	//for (int i = 0; i < n; i++)
		//Gr[i].resize(n);

	for (int i = 0; i < n; i++){
		for (int j = 0; j < i + 1; j++){
			if (i == j) Gr[i][j] = 1;
			else Gr[i][j] = rand() % 2;
			Gr[j][i] = Gr[i][j];
		}
	}
}

int Hamilton(vector <vector <int>> & edges, int n){
	vector<vector<int>> IsWayThru(n); 
	//IsWayThru.resize(n, vector<int>(n));
	for (int i = 0; i < n; i++)
		IsWayThru[i].resize(1 << n, INFINITE);

	IsWayThru[0][0] = 0;
	int res = FindWay(0, (1 << n) - 1 - 1, edges, n, IsWayThru);
	if (res == NOT_FOUND)
		return 0;
	else
		return 1;

}


int main(){

	std::ofstream output;
	output.open("output.txt");

	srand(9597);

	const int MAX_VERTICES = 1000;
	const int NumberOfGraphs = 1000;
	int count = 0;
	vector<double> proportions(MAX_VERTICES + 1);
	for (int n = 1; n <= MAX_VERTICES; n++){
		for (int i = 0; i < NumberOfGraphs; i++){
			vector <vector<int>> G;
			G.resize(n, vector<int>(n));
			GenerateGr(n, G);
			if (Hamilton(G, n))
				count++;
		}
		proportions[n] = (double)count / (double)NumberOfGraphs;
		cout << n << ' ' << proportions[n] << endl;
		output << n << ' ' << proportions[n] << endl;
		count = 0;
	}
	output.close();

}

