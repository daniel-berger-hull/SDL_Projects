#include <stdio.h>
#include <cmath>

#include "GraphSprite.h"


GraphSprite::GraphSprite()  : Sprite()
{
	printf("GraphSprite Constructor of %p called\n", this);
}

GraphSprite::~GraphSprite()
{

}

void GraphSprite::setCurrentNode(GraphNode* node)
{
	if (node != nullptr) currentNode = node;
}




void GraphSprite::setDestinationNode(GraphNode* node)
{
	if (node != nullptr) destinationNode = node;
}

void GraphSprite::reverseDirection()
{
	printf("GraphSprite reverseDirection \n");


	GraphNode* currentCell = getCurrentNode();
	GraphNode* currentTargetCell = getDestinationNode();
	
	setDestinationNode(currentCell);
	setCurrentNode(currentTargetCell);
	

	switch (this->direction)
	{
	case(SPRITE_DIRECTION_UP):
		this->direction = SPRITE_DIRECTION_DOWN;
		break;

	case(SPRITE_DIRECTION_RIGHT):
		this->direction = SPRITE_DIRECTION_LEFT;
		break;

	case(SPRITE_DIRECTION_DOWN):
		this->direction = SPRITE_DIRECTION_UP;
		break;

	case(SPRITE_DIRECTION_LEFT):
		this->direction = SPRITE_DIRECTION_RIGHT;
		break;

	}

}





void GraphSprite::update()
{
	
	if (!active) return;

	int xPosDestination = LIMIT_AREA_LEFT + (getDestinationNode()->col * INTERNAL_BORDER_TICKNESS) + 5;
	int yPosDestination = LIMIT_AREA_TOP + (getDestinationNode()->row * INTERNAL_BORDER_TICKNESS) + 5;
	int dx = xPosDestination - this->dest.x;
	int dy = yPosDestination - this->dest.y;


	// If there is any position difference, move toward the destination node...
	
		if (dx > 0)
		{
			dest.x++;
			setDirection(SPRITE_DIRECTION_RIGHT);
		}			
		else if (dx < 0)
		{ 
			dest.x--;
			setDirection(SPRITE_DIRECTION_LEFT);
		}
		// Note: Used a double if here, as it can be situation where either the x or y is equal
		//       and we don't want to just test the x > 0 and default that the sprite has to move to
		//       left side, as it may happen that the x Pos is equal, and only the y position needs to 
		//       be upudated...
			
		if (dy > 0)
			dest.y++;
		else if (dy < 0)
			dest.y--;
	
	
	// Reached the destination... (Current Position and Target position exactly the same)
	// time to find where to go next...
	if (dx == 0 && dy == 0)
	{
		if (findNextNode() == nullptr)   active = false;
	}
	
}




GraphNode* GraphSprite::findNextNode()
{

	GraphNode* currentCell = getCurrentNode();
	GraphNode* currentTargetCell = getDestinationNode();
	GraphNode* nextTargetCell = nullptr;



	bool validDirFound = false;
	int nbrPossibleDir = currentTargetCell->getNeighborCount();
	int currentDirection = getDirection();
	int nextDirection = 0;

	//For few situation, the number of neighbors dictate the next step
	// Case 1 Neighbor:  
	//   Here, the target is a dead end, and the only possible path
	//   is to turn back to the initial node (assignatedCell here)
	if (nbrPossibleDir == 1)
	{
		nextTargetCell = currentCell;
		nextDirection = currentDirection + 2;
		if (nextDirection == 5)  nextDirection = SPRITE_DIRECTION_UP;
		if (nextDirection == 6)  nextDirection = SPRITE_DIRECTION_RIGHT;
	}


	// Case 2 Neighbors:
	//  Here aside the node we come from (see previous case), there 
	//  exist one extra neighbor. This is the case for aligned nodes,
	//  or in few cases, nodes in an 90 degre elbow
	//  Regardless of the case (aligned or elbow) --> We take this single path...
	else if (nbrPossibleDir == 2)
	{
		int i;
		for ( i = 0; i < 4; i++)
		 {
			GraphNode* tmpCell = currentTargetCell->getNeighbor(i);
			if (tmpCell != nullptr && tmpCell != currentCell)
			{
				nextTargetCell = tmpCell;
				nextDirection = i;
				break;
			}
		}

		bool notFound = true;
	}

	// 
	// 
	// 
	// Case 3  or 4 Neighbors:
	//  We start to have 
	else if (nbrPossibleDir >= 3)
	{
		for (int i = 0; i < 10; i++)
		{
			int randomIndex = rand() % 4;
			GraphNode* tmpCell = currentTargetCell->getNeighbor(randomIndex);
			if (tmpCell != nullptr && tmpCell != currentCell)
			{
				nextTargetCell = tmpCell;
				nextDirection = randomIndex;
				break;
			}
		}

		bool notFound = true;
	}


	if (nextTargetCell == nullptr)
	{
		int a = 2;
	}


	setCurrentNode(currentTargetCell);
	setDestinationNode(nextTargetCell);
	setDirection(nextDirection);

	validDirFound = true;
	active = validDirFound;
	return nextTargetCell;

}



