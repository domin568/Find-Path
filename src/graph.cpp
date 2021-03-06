#include <queue>
#include "headers/graph.h"

Graph::Graph(int n, int m, Node * nodes, connection c [])
{
	this->n = n;
	adjMatrix = new double*[n];
	for (int i = 0; i < n; i++)
	{
		adjMatrix[i] = new double[n] {};
	}
	for (int i = 0 ; i  < m; i++)
	{
		connection conn = c[i];
		adjMatrix[conn.s.getID()][conn.t.getID()] = conn.distance;
		adjMatrix[conn.t.getID()][conn.s.getID()] = conn.distance;
	}

	for (int i = 0 ; i < n ; i++)
	{
		for (int j = 0 ; j < n ; j++)
		{
			if (adjMatrix[i][j] != 0)
			{
				nodes[i].addNeighbour (nodes[j]);
			}
		}
	}
}
Node Graph::find (std::vector <Node> & Q, double * d)
{
	int min = INT_MAX;
	Node minNode;
	for (int i = 0 ; i < Q.size(); i++)
	{
		Node actualNode = Q[i];
		
		if (d[actualNode.getID()] < min)
		{
			min = d[actualNode.getID()];
			minNode = actualNode;
		}
	}
	return minNode;
}
Node* Graph::ownDijkstra (Node startNode, Node * nodes)
{
	double * d = new double [n]; // dlugosc polaczen
	Node * p = new Node [n]; // poprzednicy

	for (int i = 0; i < n; i++)
	{
		d[i] = 0xffffffff;
	}

	d[startNode.getID()] = 0;

	std::vector <Node> Q;
	std::vector <Node> N;

	for (int i = 0 ; i < n; i++)
	{
		Q.push_back(nodes[i]);
	}

	while (Q.size() > 0)
	{
		Node minNode = find(Q,d); // z najkrotsza droga wybieramy

		// szukamy jego sasiadow

		for (int i = 0; i < n; i++)
		{
			if (adjMatrix[minNode.getID()][i] > 0)
			{
				N.push_back(nodes[i]);
			}
		}

		int i = 0;

		while (N.size() > 0) // w przypadku gdy znajdziemy wiecej niz jednego sasiada
		{
			Node actualSasiad = N.back();
			if (d[minNode.getID()] + adjMatrix[minNode.getID()][actualSasiad.getID()] < d[actualSasiad.getID()])
			{
				d[actualSasiad.getID()] = d[minNode.getID()] + adjMatrix[minNode.getID()][actualSasiad.getID()];
				p[actualSasiad.getID()] = minNode;
			}

			N.pop_back();
			i++;
		}

		for (int i = 0; i < Q.size(); i++) // usuwamy z node'ow do sprawdzenia, gdyz zostal juz sprawdzony
		{
			if (Q[i].getID() == minNode.getID())
			{
				Q.erase(Q.begin() + i);
			}
		}
	}
	delete [] d;
	return p;
}
std::vector <Node> Graph::path (Node s, Node t, Node * nodes)
{
	Node * p = ownDijkstra(s,nodes);
	Node actual = t;
	std::vector <Node> a;
	while (actual.getID() != -1)
	{
		a.push_back(actual);
		actual = p[actual.getID()];
	}
	std::reverse (a.begin(),a.end());
	delete [] p;
	return a;
}

int Graph::edgeCnt()
{
	int sum = 0;
	for (int i = 0 ; i < n; i++)
	{
		for (int j = 0 ; j < n; j++)
		{
			if (adjMatrix[i][j] == 1)
			{
				sum++;
			}
		}
	}
 	return sum/2;
}
int Graph::nodeCnt()
{
	return n;
} 	
void Graph::insertEdge(int u, int v)
{
	adjMatrix[u][v] = 1;
	adjMatrix[v][u] = 1;
}
void Graph::deleteEdge(int u, int v)
{
	adjMatrix[u][v] = 0;
	adjMatrix[v][u] = 0;	
}
	
bool Graph::check(int u, int v)
{
	if (adjMatrix[u][v])
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*
void Graph::bfs (int s)
{
	std::queue <vertex> kolejka;
	vv[s].visited = true;
	std::cout << s << " ";
	kolejka.push (vv[s]);
	while (kolejka.size() > 0)
	{
		vertex elem = kolejka.front();
		kolejka.pop(); // pop z front'u
		for (int i = 0; i < elem.connections.size(); i++)
		{
			int actualPolaczenie = elem.connections[i];
			if (! vv[actualPolaczenie].visited )
			{
				vv[elem.connections[i]].visited = 1;
				std::cout << vv[actualPolaczenie].val << " ";
				kolejka.push(vv[actualPolaczenie]);
			}
		}
	}
	std::cout << std::endl;
}
void Graph::dfs(int s)
{
	std::cout << s << " "; 
	vv[s].visited = 1;
	for (int i = 0 ; i < vv[s].connections.size(); i++)
	{
		if (!vv[vv[s].connections[i]].visited) // jezeli jeszcze nie odwiedzony
		{
			dfs(vv[s].connections[i]);
		}
	}
}	
*/
std::ostream& operator << (std::ostream& out, Graph& g)
{
	for (int i = 0; i < g.n; i++)
	{
		for (int j = 0 ; j < g.n; j++)
		{
			out << g.adjMatrix[i][j] << " ";
		}
		out << std::endl;
	}
	return out;
}
Graph::~Graph()
{
	for (int i = 0 ; i < n ; i++)
	{
		delete adjMatrix[i];
	}
}