#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include "node.h"

struct vertex
{
	int val;
	std::vector <uint32_t> connections; 
	bool visited = 0; 
};

class Graph
{
	private:
		int** adjMatrix;
		int n;
		vertex * vv;
		
	public:
		Graph(int n, int m, Node * nodes, connection c[]);
		int edgeCnt(); 
		int nodeCnt(); 
		void insertEdge(int u, int v);
		void deleteEdge(int u, int v);		
		bool check(int u, int v);  //sprawdza czy istnieje krawędź
		void bfs (int s);
		void dfs(int s);
		int find (std::vector <int> Q, double * d);
		int * ownDijkstra (int s);
		friend std::ostream& operator<<(std::ostream& out, Graph& g);
		~Graph();
};

#endif