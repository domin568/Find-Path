#ifndef STRUCTS_H
#define STRUCTS_H
#include <inttypes.h>
#include <cmath>
#include <vector>

struct Vector2d
{
    double x;
    double y;
};

double calcDistance (Vector2d point1, Vector2d point2);

class Node 
{
	private:
		uint32_t id = -1;
		Vector2d position;
		bool visited = 0;
		std::vector <Node> connections; 
	public:
		void draw ();	
		void addNeighbour (const Node & c);
		Node (uint32_t id, Vector2d position);
		Node ();
		uint32_t getID () { return id; }
		void setID (uint32_t id) { this->id = id; }
		Vector2d getPosition () { return position; }
		void setPosition (Vector2d vec) { this->position = vec; }
		void visit () { visited = 1; }
		bool isVisited () { return visited; }
};

struct connection 
{
	Node s;
	Node t;
	double distance = 0;
};



#endif