#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include "node.h"

class Graph
{
	private:
		double** adjMatrix;
		int n;
		
	public:
		Graph(int n, int m, Node * nodes, connection c[]);
		int edgeCnt(); 
		int nodeCnt(); 
		void insertEdge(int u, int v);
		void deleteEdge(int u, int v);		
		bool check(int u, int v);  //sprawdza czy istnieje krawędź
		void bfs (int s);
		void dfs(int s);
		Node find (std::vector <Node> & Q, double * d);
		Node* ownDijkstra (Node startNode, Node * nodes);
		std::vector <Node> path (Node s, Node t, Node * nodes);
		friend std::ostream& operator<<(std::ostream& out, Graph& g);
		~Graph();
};

#endif