void BFS(vector <int> &d, vector <int> &p, const deque<deque<pair<int, int> > > & edges, const int start){

	vector <bool> visited(edges.size(), false);

	d.resize(edges.size(), INF);
	p.resize(edges.size());

	deque <int> q;

	d[start] = 0;
	p[start] = start;

	q.push_back(start);

	while (!q.empty())
	{
		int v = q.front();
		q.pop_front();
		for (int i = 0; i < edges[v].size(); ++i){
			int u = edges[v][i].first;
			if (v != u  &&  /*!visited[i] &&*/ d[u] > d[v] + edges[v][i].second){
				p[u] = v;
				d[u] = d[v] + edges[v][i].second;
				/*if (edges[v][i].second == 0){
				q.push_front(i);
				}
				else{*/
				q.push_back(i);
				//	}
			}
		}
		//visited[v] = true;
	}
}

void changeGraphPresentation(const deque<deque<pair<int, int> > > & edgesU, deque<deque<pair<int, int> > > & edges){
	for (int i = 0; i < edgesU.size(); ++i){
		for (int j = 0; j < edges.size(); ++j){
			if (edgesU[i][j].first)
				edges[i].push_back(std::make_pair(j, edgesU[i][j].second));
		}
	}
}

void BFS01(deque <int> &d, deque <int> &p, const deque<deque<pair<int, int> > > & edges2, const int start){

	deque<deque<pair<int, int> > >  edges(edges2.size());// , deque<pair<int, int> >);

	changeGraphPresentation(edges2, edges);

	vector <bool> visited(edges.size(), false);

	d.resize(edges.size(), INF);
	p.resize(edges.size());

	deque <int> q;

	d[start] = 0;
	p[start] = start;

	q.push_back(start);

	while (!q.empty())
	{
		int v = q.front();
		q.pop_front();
		for (int i = 0; i < edges[v].size(); ++i){
			int u = edges[v][i].first;
			if (v != u &&  !visited[u] && d[u] > d[v] + edges[v][i].second){
				p[u] = v;
				d[u] = d[v] + edges[v][i].second;
				if (edges[v][i].second == 0){
					q.push_front(u);
				}
				else{
					q.push_back(u);
				}
			}
		}
		visited[v] = true;
	}
}

void GenerateGr2(const int n, deque<deque<int> > & Gr, const int w, const char* name){

	std::ofstream output;
	output.open(name);

	output << n << std::endl;

	double possib;
	int count = 0;
	int isEdge;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < i + 1; j++){
			if (i == j) {
				Gr[i][j] = 0;
				break;
			}
			else {
				possib = (double)(rand() % 100 + 1) / 100;
				if (count % 2 == 0)
					isEdge = (possib <= 0.5) ? 1 : 0;
				else
					isEdge = (possib <= 0.1) ? 1 : 0;
				Gr[i][j] = Gr[j][i] = isEdge ? rand() % w : INF;
			}
			count++;
		}
	}

	for (int i = 0; i < Gr.size(); ++i){
		for (int j = 0; j < Gr[0].size(); ++j){
			/*if (i == 0 && j == 0) cout << '0' << '\t';
			else if (j == 0)
			output << i - 1 << '\t';
			else if (i == 0)
			output << j - 1 << '\t';
			else*/
			output << Gr[i][j] << '\t';
		}
		output << std::endl;
	}

	output.close();
}

