#pragma once


#define UNDEFINED_COLUMN          -1
#define UNDEFINED_ROW             -2
#define UNDEFINED_INDEX           -5



#define DEFAULT_LENGHT			   1



#define  NORTH_INDEX  0
#define  EAST_INDEX   1
#define  SOUTH_INDEX  2
#define  WEST_INDEX   3

enum ScanDirection { NOT_DEFINED = 0, NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };





class GraphNode
{
public:
	GraphNode();
	GraphNode(int x, int y, int len);
	GraphNode(long id,int x, int y, int len);



	~GraphNode();

	void addNeighbor(GraphNode* neighbour, ScanDirection relativePos);
	GraphNode* getNeighbor(ScanDirection currentDir);
	GraphNode* getNeighbor(int index);
	int  getNeighborCount();

	bool isSameNode(GraphNode* otherNode);

	ScanDirection  mirrorDirection(ScanDirection currentDir);



	long id;
	int col;
	int row;
	int length;

	GraphNode* neighbours[4];
	
	//static int nextID;

};




