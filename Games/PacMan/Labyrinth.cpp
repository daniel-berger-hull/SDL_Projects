#include "Labyrinth.h"





Labyrinth::Labyrinth()
{

}

Labyrinth::~Labyrinth()
{

	cleanUp();
}


GraphNode* Labyrinth::getNodeForCoords(int col, int row)
{
	GraphNode* nodeFound = nullptr;

	 
	for (std::vector<GraphNode*>::iterator it = nodes->begin(); it != nodes->end(); ++it)
	{
		GraphNode* currentNode = *it;

		if (currentNode->col == col && currentNode->row == row)
		{
			nodeFound = currentNode;
			break;
		}
	}

	return nodeFound;
}


ScanDirection getMirrorDirection(ScanDirection direction)
{
	switch (direction)
	{
	case NORTH:
		return SOUTH;
		break;
	case EAST:
		return WEST;
		break;
	case SOUTH:
		return NORTH;
		break;
	case WEST:
		return EAST;
		break;
	default:
		break;
	}
}

ScanDirection getMirrorDirectionCoords(ScanDirection direction, int* col, int* row)
{
	switch (direction)
	{
	case NORTH:
		(*row)++;
		return SOUTH;
		break;
	case EAST:
		(*col)--;
		return WEST;
		break;
	case SOUTH:
		(*row)--;
		return NORTH;
		break;
	case WEST:
		(*col)++;
		return EAST;
		break;
	default:
		break;
	}
}



int Labyrinth::load()
{
	

	nodes = new std::vector<GraphNode*>();
	GraphNode* currentNode = nullptr;
	GraphNode* neighbourNode = nullptr;


	ScanDirection directionsArray[5];

	directionsArray[0] = NOT_DEFINED;
	directionsArray[1] = NORTH;
	directionsArray[2] = EAST;
	directionsArray[3] = SOUTH;
	directionsArray[4] = WEST;

	std::string directionNames[5];

	directionNames[0] = std::string("Not Defined");
	directionNames[1] = std::string("NORTH");
	directionNames[2] = std::string("EAST");
	directionNames[3] = std::string("SOUTH");
	directionNames[4] = std::string("WEST");



	int totalNodeCount = 0;		//Keep track of the number of dynamic GraphNode created, to monitor the potential memory leaks...


	for (int row = 0; row < TOTAL_ROWS; row++)
	{
		for (int column = 0; column < TOTAL_COLUMNS; column++)
		{
			// If this cell is blocked, then no path possible so pass to next one...
			if (labyrinthCells[row][column] != 0)
			{
				printf("(%d,%d)  XXX\n", column, row);
				continue;
			}


			printf("(%d,%d)\n", column, row);

			currentNode = nullptr;
			currentNode = getNodeForCoords(column, row);
			if (currentNode == nullptr)
			{
				currentNode = new GraphNode(column, row, 1);
				nodes->push_back(currentNode);
				totalNodeCount++;
				printf("  New Node #%d created (%d,%d)  Total Neighbours = %d\n", totalNodeCount, column, row, currentNode->getNeighborCount());
			}

			// Scan the direction clock wise, NORTH, EAST, ... till
			// you find an empty cell, and check if there is alread a cell
			// recorded for that spot... 
			// If Yes, then work from this cell
			// If No, then create a new cell
			// But in both case,  make sure it is connected with its neighboors..
			for (int dirIndex = NORTH; dirIndex <= WEST; dirIndex++)
			{
				// First, make sure that the cell in the direction you look at is empty
				// If not, then we can't make anything with that cell (it  is a wall basically)
				if (isPathAvailable(row, column, directionsArray[dirIndex]))
				{
					//There is a need to have an extra node, as it doesn't exist
					// Is there is no Cell recorded in that direction, then add this cell
					// to the graph...
					if (!isNeighbourNodeExist(currentNode, directionsArray[dirIndex]))
					{
						neighbourNode = createNeighbourNode(currentNode, directionsArray[dirIndex]);
						nodes->push_back(neighbourNode);
						totalNodeCount++;
				
						//const char* cstr = directionNames[dirIndex].c_str();
						//printf("%s ", cstr);

					}
					else
					{
						// The previous call to the isNeighbourNodeExist method carry over a pointer to the 
						// Node found for the coords given (lastExistingNeighbourFound variable)
						GraphNode* existingNeighbour = lastExistingNeighbourFound;

						//This test is a bit redondant here has we know by now that a Neighbour exists...
						if (existingNeighbour != nullptr)
						{
							ScanDirection mirrorDir = existingNeighbour->mirrorDirection(directionsArray[dirIndex]);

							currentNode->addNeighbor(existingNeighbour, directionsArray[dirIndex]);
							existingNeighbour->addNeighbor(currentNode, mirrorDir);
						}
					}

					// Note: There may be a code clean up here, as in both case of the if, we add neighbors
					//       to the new or existing cell (depends on the case)
					

				}
				


			}

			printf("\n");
		}

	}

	return OK; 
}

bool Labyrinth::isPathAvailable(int fromRow, int fromCol, ScanDirection direction)
{
	//Use the from coords has a starting point,
	//as the direction parameter will determined
	// the 'to' coords to look for...
	int toRow = fromRow;
	int toCol = fromCol;


	switch (direction)
	{
		case NORTH:
			if (fromRow <= 0)  return false;
			toRow--;
			break;
		case EAST:
			if (fromCol >= TOTAL_COLUMNS - 1)  return false;
			toCol++;
			break;
		case SOUTH:
			if (fromRow >= TOTAL_ROWS - 1)  return false;
			toRow++;
			break;
		case WEST:
			if (fromCol <= 0)  return false;
			toCol--;
			break;
		default:
			break;
	}

	return (labyrinthCells[toRow][toCol] == 0);
}


// Before creating any new nodes, we need firt to make sure that they exist in the
// first place, otherwise we are going to run into memory waist and other complication...
// We are leveraging the vector of GraphNode*, and check on by one if 
bool Labyrinth::isNeighbourNodeExist(GraphNode* currentNode, ScanDirection relativePos)
{
	bool neighourFound = false;
	int  neighbourCol = currentNode->col;
	int  neighbourRow = currentNode->row;
	lastExistingNeighbourFound = nullptr;


	//Need to adjust the coords in regard to the relative position with the current node asked...
	switch (relativePos)
	{
		case NORTH:
			if (neighbourRow <= 0)  return false;
			neighbourRow--;
			break;
		case EAST:
			if (neighbourCol >= TOTAL_COLUMNS - 1)  return false;
			neighbourCol++;
			break;
		case SOUTH:
			if (neighbourRow >= TOTAL_ROWS - 1)		return false;
			neighbourRow++;
			break;
		case WEST:
			if (neighbourCol <= 0)					return false;
			neighbourCol--;
			break;
	}


	for (std::vector<GraphNode*>::iterator it = nodes->begin(); it != nodes->end(); ++it)
	{
		GraphNode* currentNode = *it;  

		if (currentNode->col == neighbourCol && currentNode->row == neighbourRow)
		{
			neighourFound = true;
			lastExistingNeighbourFound = currentNode;
			break;
		}
	}

	return neighourFound;
}



GraphNode* Labyrinth::createNeighbourNode(GraphNode* currentNode, ScanDirection relativePos)
{
	int col = currentNode->col;
	int row = currentNode->row;

	switch (relativePos)
	{
	case NORTH:
		row--;
		printf("  New Node (NORTH Neighbor) created (%d,%d)\n", col, row);
		break;
	case EAST:
		col++;
		printf("  New Node (EAST Neighbor) created (%d,%d)\n", col, row);
		break;
	case SOUTH:
		row++;
		printf("  New Node (SOUTH Neighbor) created (%d,%d)\n", col, row);
		break;
	case WEST:
		col--;
		printf("  New Node (WEST Neighbor) created (%d,%d)\n", col, row);
		break;
	default:
		break;
	}


	GraphNode* newNode = new GraphNode(col, row, 1);
//	GraphNode* newNode = new GraphNode(nextNodeID++,col, row, 1);

	ScanDirection mirrorDir = newNode->mirrorDirection(relativePos);

	currentNode->addNeighbor(newNode, relativePos);
	newNode->addNeighbor(currentNode, mirrorDir);

	printf("  Main Node Total Neighbours = %d , New Neighbour Node Total Neighbours = %d\n", currentNode->getNeighborCount(), newNode->getNeighborCount());

	return newNode;
}

void Labyrinth::cleanUp()
{
	printf("Labyrinth cleanUp in progress...\n");

	int nodeDeleted = 0;
	for (int i = 0; i < nodes->size(); i++)
	{
		delete (*nodes)[i];
		nodeDeleted++;
	}

	printf("Total Node Deleted = %d\n", nodeDeleted);

}
