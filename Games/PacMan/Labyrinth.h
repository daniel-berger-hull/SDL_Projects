#pragma once

#include <stdio.h>

#include <vector>
#include <stack>
#include <string>

#include "Global.h"
#include "GraphNode.h"


class Labyrinth
{
	public:
		Labyrinth();
		~Labyrinth();

		int        load();
		int        getTotalNodes()						 {   return nodes->size();            }
		int        getLabyrinthCell(int col, int row)    {   return labyrinthCells[row][col]; }
		GraphNode* getNodeForCoords(int col, int row);
		GraphNode* getNode(int index)				     {   return nodes->at(index);         }

	private:

		int test;
		std::stack<GraphNode*>   scanStack;
		std::vector<GraphNode*>* nodes;
		GraphNode*				 lastExistingNeighbourFound = nullptr;


		bool		isPathAvailable(int fromRow, int fromCol, ScanDirection direction);
		bool		isNeighbourNodeExist(GraphNode* currentNode, ScanDirection relativePos);
		GraphNode*  createNeighbourNode(GraphNode* currentNode, ScanDirection relativePos);

		ScanDirection getMirrorDirection(ScanDirection direction);
		ScanDirection getMirrorDirectionCoords(ScanDirection dir, int* col, int* row);

		void		cleanUp();

		int nextNodeID = 1;

		int labyrinthCells[TOTAL_ROWS][TOTAL_COLUMNS] =

		{

			{0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0},
			{0,	1,	1,	0,	1,	1,	0,	1,	0,	1,	1,	0,	1,	1,	0},
			{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},
			{0,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	0},
			{0,	0,	0,	0,	1,	0,	0,	1,	0,	0,	1,	0,	0,	0,	0},
			{1,	1,	1,	0,	1,	1,	0,	1,	0,	1,	1,	0,	1,	1,	1},
			{1,	1,	1,	0,	1,	0,	0,	0,	0,	0,	1,	0,	1,	1,	1},
			{1,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	1},
			{0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0},
			{1,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	1},
			{1,	1,	1,	0,	1,	0,	0,	0,	0,	0,	1,	0,	1,	1,	1},
			{1,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	1},
			{0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0},
			{0,	1,	1,	0,	1,	1,	0,	1,	0,	1,	1,	0,	1,	1,	0},
			{0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0},
			{1,	0,	1,	0,	1,	0,	1,	1,	1,	0,	1,	0,	1,	0,	1},
			{0,	0,	0,	0,	1,	0,	0,	1,	0,	0,	1,	0,	0,	0,	0},
			{0,	1,	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1,	1,	0},
			{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0}

		};



};

