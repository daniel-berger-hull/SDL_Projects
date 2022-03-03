#include "GraphNode.h"


//int GraphNode::nextID = 0;

GraphNode::GraphNode() 
	: col(UNDEFINED_COLUMN), row(UNDEFINED_ROW), length(DEFAULT_LENGHT)
{
	//this->id = ++nextID;
	for (int i = 0; i < 4; i++) neighbours[i] = nullptr;
}



//GraphNode::GraphNode(int x, int y, int len, ScanDirection dir)
GraphNode::GraphNode(int x, int y, int len) 
	  : col(x), row(y), length(len)


{
	//this->col = x;
	//this->row = y;
	//this->length = len;
	//this->direction = dir;


	

	for (int i = 0; i < 4; i++) neighbours[i] = nullptr;

}

GraphNode::GraphNode(long id, int x, int y, int len) : col(x), row(y), length(len)
{
	this->id = id;
}




GraphNode::~GraphNode()
{
	//if (neighbours != nullptr)  delete neighbours;
}


bool GraphNode::isSameNode(GraphNode* otherNode)
{

	if (this->col != otherNode->col || 
		this->row != otherNode->row)	return false;
	
	return true;
}


void GraphNode::addNeighbor(GraphNode* neighbour, ScanDirection relativePos)
{
	int arrayIndex          = UNDEFINED_INDEX;
	
	switch (relativePos)
	{
		case NORTH:
			arrayIndex = NORTH_INDEX;
			break;
		case EAST:
			arrayIndex = EAST_INDEX;
			break;
		case SOUTH:
			arrayIndex = SOUTH_INDEX;
			break;
		case WEST:
			arrayIndex = WEST_INDEX;
			break;
		default:
			return;
			break;
	}

	neighbours[arrayIndex] = neighbour;
	
}

GraphNode* GraphNode::getNeighbor(ScanDirection direction)
{
	switch (direction)
	{
		case NOT_DEFINED:
			break;
		case NORTH:
			return neighbours[0];
			break;
		case EAST:
			return neighbours[1];
			break;
		case SOUTH:
			return neighbours[2];
			break;
		case WEST:
			return neighbours[3];
			break;
		default:
			break;
	}
}

GraphNode* GraphNode::getNeighbor(int index)
{
	if (index < 0 || index >= 4)  return nullptr;
	else return neighbours[index];
}








int GraphNode::getNeighborCount()
{
	int count = 0;
	for (int dirIndex = NORTH; dirIndex <= WEST; dirIndex++)
	{
		if (neighbours[dirIndex-1] != nullptr)  count++;
	}

	return count;
}




ScanDirection  GraphNode::mirrorDirection(ScanDirection currentDir)
{

	ScanDirection mirror;

	switch (currentDir)
	{
	case NORTH:
		mirror = SOUTH;
		break;
	case EAST:
		mirror = WEST;
		break;
	case SOUTH:
		mirror = NORTH;
		break;
	case WEST:
		mirror = EAST;
		break;
	}

	return mirror;
}
