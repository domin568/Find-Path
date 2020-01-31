#include <vector>
#include <iostream>

struct edge
{
	int s;
	int t;
};
struct vertex
{
	int val;
	std::vector <unsigned int> polaczenia; 
	bool visited = 0; 
};

class Graph
{
	private:
		int** adjMatrix;
		int n;				//liczba wierzcholkow
		vertex * vv;
		
	public:
		// m = liczba polaczen
		Graph(int n, int m, edge edges[], bool directed = false);
		int edgeCnt(); 
		int nodeCnt(); 
		void insertEdge(int u, int v);
		void deleteEdge(int u, int v);		
		bool check(int u, int v);  //sprawdza czy istnieje krawędź
		void bfs (int s);
		void dfs(int s);
		friend std::ostream& operator<<(std::ostream& out, Graph& g);
		~Graph();
};