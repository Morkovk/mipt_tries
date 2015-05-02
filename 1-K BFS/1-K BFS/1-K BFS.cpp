#include <cstdio>
#include <fstream>
#include <deque>
#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <queue>
#include <functional>
#include <ctime>
#include <boost\graph\adjacency_list.hpp>
#include <boost\graph\dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;


using std::vector;
using std::cout;
using std::cin;
using std::deque;
using std::pair;
using std::queue;
using std::priority_queue;

#define INF 1000000
#define weight (1 << 8)
#define Gsize 10000


class Graph{
public:
	deque<deque<pair <int, int> > >  edges;
	int n;
	int max_w;
	int k;

	void GenerateGr(const int n, const int w);
	void BFS0k(vector<long long> &d, vector<int> &p, const int start);
	void readGraph(const char *name);
	void writeGraph(const char* name);
	void BFSCycle(vector<long long> &d, vector<int> &p, const int start);
	void Dijkstra(vector<long long> &d, vector <int> &p, const int start);
};


void Graph::writeGraph(const char* name){

	max_w = 0;

	std::ofstream output;
	output.open(name);

	int n = edges.size();

	output << n << ' ' << k << std::endl;

	for (int i = 0; i < n; ++i){
		output << edges[i].size() << '\n';
		for (int j = 0; j < edges[i].size(); ++j){
			output << edges[i][j].first << ' ';
			output << edges[i][j].second << '\t';
			max_w += edges[i][j].second;
		}
		output << std::endl;
	}
	output.close();

	//cout << max_w;
}

void Graph::GenerateGr(const int m, const int w){

	k = w;

	srand(3457);

	n = m;

	edges.resize(n);

	double possib;
	int count = 0;
	int isEdge;
	int _w;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < i + 1; j++){
			if (i == j)
				continue;
			else{
				possib = (double)(rand() % 100 + 1) / 100;
				if (count % 2 == 0)
					isEdge = (possib <= 0.05) ? 1 : 0;
				else
					isEdge = (possib <= 0.01) ? 1 : 0;
				if (isEdge){
					_w = rand() % w + 1;
					edges[i].push_back(std::make_pair(j, _w));
					edges[j].push_back(std::make_pair(i, _w));
				}
			}
			count++;
		}
	}
}



void Graph::readGraph(const char *name){

	std::ifstream input;
	input.open(name);

	int n;
	input >> n >> k;
	edges.clear();
	edges.resize(n);

	int c, v, _w;

	for (int i = 0; i < n; ++i){
		input >> c;
		if (c == 0) input >> c;
		for (int j = 0; j < c; ++j){
			input >> v >> _w;
			edges[i].push_back(std::make_pair(v, _w));
		}
	}
}



void Graph::BFS0k(vector<long long> &d, vector<int> &p, const int start){

	int s = edges.size();

	deque <deque<int> > edges2(s);

	for (int i = 0; i < edges.size(); i++){
		edges2[i].resize(edges[i].size());
		for (int j = 0; j < edges[i].size(); ++j)
			edges2[i][j] = edges[i][j].first;
	}
	
	
	int last;
	int realSize = edges2.size();
	int collapse = 0;
	int collapseHeight = 0;

	vector <bool> visited(edges2.size(), false);

	d.resize(edges2.size(), INF);
	p.resize(edges2.size(), -1);

	deque <int> q;

	d[start] = 0;
	p[start] = start;

	q.push_back(start);

	while (!q.empty())
	{
		int v = q.front();
		q.pop_front();
		for (int i = 0; i < edges2[v].size(); ++i){
			if (/*edges2[v][i] > collapseHeight && visited[edges2[v][i] - collapse] || edges2[v][i] <= collapseHeight &&*/ visited[edges2[v][i]]) continue;
			if (v < realSize && edges[v][i].second > 1){
				int val = edges[v][i].second;
				last = edges2.size();
				edges2.push_back(deque<int>());
				int aim = edges2[v][i];
				edges2[v][i] = last;
				val--;
				while (val > 1){
					edges2.push_back(deque<int>());
					edges2[last].push_back(last + 1);
					val--;
					last++;
				}
				edges2[last].push_back(aim);
			}

			int u = (edges2[v][i] > collapseHeight) ? edges2[v][i] - collapse : edges2[v][i];

			d.resize(last + 1, INF);
			p.resize(last + 1, -1);
			visited.resize(last + 1, false);

			if (!visited[u] && d[u] > d[v] + 1){
				p[u] = v;
				d[u] = d[v] + 1;
				q.push_back(u);
				if (u < realSize){
					int l = p[u];
					//collapseHeight += p[u];
					while (p[u] >= realSize){
						p[u] = p[l];
						int q = l;//temporary
						l = p[l];
						/*p.erase(p.begin() + q);
						d.erase(d.begin() + q);
						visited.erase(visited.begin() + q);
						edges2[l].pop_back();
						edges2.erase(edges2.begin() + q);
						collapse++;*/
					}
					v = p[u];
					i = edges2[v].size();//!!!!!!
					edges2[v][i - 1] = u;
				}
			}
		}
		visited[v] = true;
	}


}




int main(){
	srand(5968);

	Graph Gr;
	Gr.GenerateGr(Gsize, weight);

	std::string name = "output.txt";
	Gr.writeGraph(name.c_str());

	Gr.readGraph(name.c_str());

	cout << "gr read\n";

	vector <long long> d;
	vector <int> p;

	clock_t startTime = clock();
	Gr.BFSCycle(d, p, 0);
	cout << double(clock() - startTime) / (double)CLOCKS_PER_SEC << " seconds." << std::endl;


	typedef float Weight;
	typedef boost::property<boost::edge_weight_t, Weight> WeightProperty;
	typedef boost::property<boost::vertex_name_t, int> NumberProperty;

	typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS, NumberProperty, WeightProperty > BGraph;

	typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

	typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap;
	//typedef boost::property_map < Graph, boost::vertex_name_t >::type NameMap;

	typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
	typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;

	BGraph BG;
	for (int i = 0; i < Gr.edges.size(); ++i){
		if (BG.vertex_set.size() < i)
			add_vertex(i, BG);
		for (int j = 0; j < Gr.edges[i].size(); ++j){
			add_edge(i, Gr.edges[i][j].first, Gr.edges[i][j].second, BG);
		}
	}

	std::vector<Vertex> predecessors(boost::num_vertices(BG)); // To store parents
	std::vector<Weight> distances(boost::num_vertices(BG)); // To store distances

	IndexMap indexMap = boost::get(boost::vertex_index, BG);
	PredecessorMap predecessorMap(&predecessors[0], indexMap);
	DistanceMap distanceMap(&distances[0], indexMap);

	boost::dijkstra_shortest_paths(BG, 0, boost::distance_map(distanceMap), predecessor_map(predecessorMap));



	/*for (int i = 0; i < Gr.n; ++i)
		cout << p[i] << ' ';

	cout << std::endl << std::endl;

	for (int i = 0; i < Gr.n; ++i){
		if (d[i] == INF)
			cout << "INF" << ' ';
		else
			cout << d[i] << ' ';
	}*/
	getchar();
}

void Graph::BFSCycle(vector<long long> &d, vector<int> &p, const int start){
	vector<queue<int> > q(k + 1);
	q[0].push(start);

	d.resize(n, INF);
	p.resize(n, -1);

	int curD = -1;
	int i = 0;

	int numberOfV = 1;

	p[start] = 0;
	d[start] = 0;

	while(numberOfV > 0){//все очереди в векторе пуст
		curD++;
		i = curD % k;
		while (!q[i].empty()) {
			int v = q[i].front();
			q[i].pop();
			numberOfV--;
			if (curD > d[v])
				continue;
			for (int j = 0; j < edges[v].size(); ++j) {
				int u = edges[v][j].first;
				if (d[u] > d[v] + edges[v][j].second) {
					q[(d[v] + edges[v][j].second) % k].push(u);
					numberOfV++;
					d[u] = d[v] + edges[v][j].second;
					p[u] = v;
				}
			}
		}
	}
}


bool greater(pair<int, int> a, pair<int, int> b){
	return (a.second > b.second);
}


void Graph::Dijkstra(vector<long long> &d, vector <int> &p, const int start){
	d.resize(n, INF);
	p.resize(n, -1);

	d[start] = 0;
	priority_queue < pair<int, int>, vector<pair<int, int> >, std::function<bool(pair<int, int>, pair<int, int>)> > q(greater);
	q.push(std::make_pair(start, 0));

	while (!q.empty()){
		int v = q.top().first;
		int dist = q.top().second;
		q.pop();
		if (dist > d[v])
			continue;
		for (int i = 0; i < edges[v].size(); ++i){
			int u = edges[v][i].first;
			if (d[u] > d[v] + edges[v][i].second){
				d[u] = d[v] + edges[v][i].second;
				p[u] = v;
				q.push(std::make_pair(u, d[u]));
			}
		}
	}

	

}



