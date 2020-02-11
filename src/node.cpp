#include "headers/node.h"

Node::Node (uint32_t id, Vector2d position)
{
	this->id = id;
	this->position = position;
}
Node::Node () {}

void Node::addNeighbour (const Node & c)
{
	connections.push_back(c);
}
