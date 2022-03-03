#include <stdio.h>

#include <vector>
#include <stack>
#include <string>

#include <cmath> 


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include "Global.h"
#include "Sprite.h"
#include "GraphSprite.h"
#include "SpriteManager.h"



#include "GraphNode.h"
#include "Labyrinth.h"



SDL_Window*    win;
SDL_Renderer*  render;
SDL_Texture*   texture, * text;


// controls annimation loop
int close = 0;
int speed = 300;
SDL_Point points[20];
SDL_Rect cellRects[165];




//GraphSprite* spriteArray[TOTAL_NUMBER_GHOSTS];
Sprite* spriteArray[TOTAL_NUMBER_GHOSTS];



GraphSprite  redGhost;
GraphSprite  greenGhost;
GraphSprite  orangleGhost;
GraphSprite  pinkGhost;
SpriteManager spriteManager;

Labyrinth* labyrinth = nullptr;


bool displayCell       = true;
bool displayCellLinks  = true;
bool displayCellCoords = false;



int init();
int update();
int redrawScreen();

SDL_Texture* loadTexture(SDL_Renderer* render, const char* fileName, SDL_Rect* destRect, int sizeRatio);
int loadGameTextures();

void calculateCellCenters();
void drawLabyrinth();
void drawGraphCells();
void drawGraphCellsLinks();
void drawGraphCellsCoords();
void resetGhosts();
void placeGhostInNode(int spriteIndex, int nodeNumber);
void handleMouseClick(int x, int y);

int  cleanUp();





SDL_Texture* loadTexture( SDL_Renderer* render , const char* fileName, SDL_Rect* destRect , int sizeRatio )
{
	
	SDL_Surface* surface = IMG_Load( fileName );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( render, surface );
	SDL_FreeSurface(surface);

	SDL_QueryTexture( texture, NULL, NULL, &(destRect->w), &(destRect->h) );

	destRect->w /= sizeRatio;
	destRect->h /= sizeRatio;

	return texture;
}


int loadGameTextures()
{
	SDL_Rect     destRect;
	SDL_Texture* tmpTexture;
	

	tmpTexture = loadTexture(render, "D:\\data\\workspace\\C++\\SDL\\games\\Debug\\Ghost_Red.png", &destRect, GHOSTS_TEXTURE_RATIO);
	redGhost.setTexture(tmpTexture);
	redGhost.setRect(destRect);
	redGhost.setType(TYPE_RED_GHOST);


	tmpTexture = loadTexture(render, "D:\\data\\workspace\\C++\\SDL\\games\\Debug\\Ghost_Green.png", &destRect, GHOSTS_TEXTURE_RATIO);
	greenGhost.setTexture(tmpTexture);
	greenGhost.setRect(destRect);
	greenGhost.setType(TYPE_GREEN_GHOST);

	tmpTexture = loadTexture(render, "D:\\data\\workspace\\C++\\SDL\\games\\Debug\\Ghost_Orange.png", &destRect, GHOSTS_TEXTURE_RATIO);
	orangleGhost.setTexture(tmpTexture);
	orangleGhost.setRect(destRect);
	orangleGhost.setType(TYPE_ORANGE_GHOST);

	tmpTexture = loadTexture(render, "D:\\data\\workspace\\C++\\SDL\\games\\Debug\\Ghost_Pink.png", &destRect, GHOSTS_TEXTURE_RATIO);
	pinkGhost.setTexture(tmpTexture);
	pinkGhost.setRect(destRect);
	pinkGhost.setType(TYPE_PINK_GHOST);

	spriteArray[0] = &redGhost;
	spriteArray[1] = &greenGhost;
	spriteArray[2] = &orangleGhost;
	spriteArray[3] = &pinkGhost;


	spriteManager.addSprite(&redGhost);
	spriteManager.addSprite(&greenGhost);
	spriteManager.addSprite(&orangleGhost);
	spriteManager.addSprite(&pinkGhost);


	return 0;
}


void resetGhosts() 
{

	for (int i = 0; i < 4; i++)
	{
		int cellInd = rand() % labyrinth->getTotalNodes();
		placeGhostInNode(i, cellInd);
	}

}


void placeGhostInNode(int spriteIndex,int nodeNumber)
{

	if ( spriteIndex < 0 || spriteIndex >= TOTAL_NUMBER_GHOSTS )       return;
	if ( nodeNumber < 0 || nodeNumber >= labyrinth->getTotalNodes() )  return;

	int xPos = 0;
	int yPos = 0;
	GraphNode* assignatedCell = labyrinth->getNode(nodeNumber);;

	for (int dir = SPRITE_DIRECTION_UP; dir <= SPRITE_DIRECTION_LEFT; dir++)
	{

		GraphNode* newDirectionCell = assignatedCell->getNeighbor(dir-1);
		if (newDirectionCell != nullptr)
		{
			
			xPos = LIMIT_AREA_LEFT + (assignatedCell->col * INTERNAL_BORDER_TICKNESS) + 5;
			yPos = LIMIT_AREA_TOP + (assignatedCell->row * INTERNAL_BORDER_TICKNESS) + 5;
			/*spriteArray[spriteIndex]->setCurrentNode(assignatedCell);
			spriteArray[spriteIndex]->setPosition(xPos, yPos);
			spriteArray[spriteIndex]->setDirection(dir);
			spriteArray[spriteIndex]->setDestinationNode(newDirectionCell);*/
			
			GraphSprite* next = (GraphSprite*)spriteArray[spriteIndex];
			next->setCurrentNode(assignatedCell);
			next->setPosition(xPos, yPos);
			next->setDirection(dir);
			next->setDestinationNode(newDirectionCell);


			return;
		}
	}
}

void calculateCellCenters()
{
	//To display the dot of the cell right in the center, and not on 
	//the top left corner...
	int cellCenterX = INTERNAL_BORDER_TICKNESS / 2;
	int cellCenterY = INTERNAL_BORDER_TICKNESS / 2;
	int nodeCount = labyrinth->getTotalNodes();
	printf("total number of Graph Nodes is %d\n", nodeCount);

	for (int i = 0; i < nodeCount; i++)
	{
		GraphNode* currentNode = labyrinth->getNode(i);

		cellRects[i].x = LIMIT_AREA_LEFT + (currentNode->col * INTERNAL_BORDER_TICKNESS) + cellCenterX;
		cellRects[i].y = LIMIT_AREA_TOP + (currentNode->row * INTERNAL_BORDER_TICKNESS) + cellCenterY;

		cellRects[i].w = 4;
		cellRects[i].h = 4;
	}
}

int init()
{
	// retutns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	win = SDL_CreateWindow("GAME", // creates a window
							100,
							 10,
							SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	render = SDL_CreateRenderer(win, -1, render_flags);

	labyrinth = new Labyrinth();
	labyrinth->load();

	loadGameTextures();
	calculateCellCenters();
	resetGhosts();

	return 0;
}

int update()
{
	// Have to fix this method as it doesn't accept GraphSprite
	//spriteManager.detectCollisions(spriteArray, 4);

	spriteManager.updateSprites();

	spriteManager.detectCollisions();
	//updateSprites

	//  Implement the update in a spriteManager instead...
	//for (int i = 0; i < TOTAL_NUMBER_GHOSTS; i++)    spriteArray[i]->update();
	
	return 0;
}

int redrawScreen()
{

	SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(render);

	drawLabyrinth();

	if (displayCell)			drawGraphCells();
	if (displayCellLinks)		drawGraphCellsLinks();
	if (displayCellCoords)		drawGraphCellsCoords();

	for (int i = 0; i < 4; i++)    spriteArray[i]->draw(render);

	spriteManager.drawSprites(render);


	//spriteManager

	// triggers the double buffers for multiple rendering
	SDL_RenderPresent(render);

	return 0;
}

void drawGraphCells() {
 
	SDL_SetRenderDrawColor(render, 0xF8, 0xB0, 0x90, 0xFF);
	SDL_RenderFillRects(render, cellRects, 165);
}

void drawGraphCellsLinks()
{
	int cellCenterX = INTERNAL_BORDER_TICKNESS / 2;
	int cellCenterY = INTERNAL_BORDER_TICKNESS / 2;
	int nodeCount = labyrinth->getTotalNodes();

	int dx[] = { 0,15,0,-15 };
	int dy[] = { -15,0,15,0 };
	for (int i = 0; i < nodeCount; i++)
	{
		GraphNode* currentNode = labyrinth->getNode(i);
		 
		int xCenterPos = LIMIT_AREA_LEFT + (currentNode->col * INTERNAL_BORDER_TICKNESS) + cellCenterX;
		int yCenterPos = LIMIT_AREA_TOP + (currentNode->row * INTERNAL_BORDER_TICKNESS) + cellCenterY;

		for (int i = 0; i < 4; i++)
		{
			GraphNode* sameDirectionCell = currentNode->getNeighbor(i);
			if (sameDirectionCell != nullptr)
			{

				if (sameDirectionCell != currentNode)
					SDL_SetRenderDrawColor(render, 0x18, 0x18, 0xFF, 0xFF);
				else
					SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xFF);

			
				SDL_RenderDrawLine(render, xCenterPos, yCenterPos, xCenterPos + dx[i], yCenterPos + dy[i]);
			}

		}

	}

}


void drawGraphCellsCoords()
{
	int cellCenterX = INTERNAL_BORDER_TICKNESS / 2;
	int cellCenterY = INTERNAL_BORDER_TICKNESS / 2;
	int nodeCount = labyrinth->getTotalNodes();

	int dx[] = { 0,10,0,-10 };
	int dy[] = { -10,0,10,0 };
	for (int i = 0; i < nodeCount; i++)
	{
		GraphNode* currentNode = labyrinth->getNode(i);

		int xCenterPos = LIMIT_AREA_LEFT + (currentNode->col * INTERNAL_BORDER_TICKNESS);
		int yCenterPos = LIMIT_AREA_TOP + (currentNode->row * INTERNAL_BORDER_TICKNESS);

		for (int i = 0; i < 4; i++)
		{
			GraphNode* sameDirectionCell = currentNode->getNeighbor(i);
			if (sameDirectionCell != nullptr)
			{
				SDL_SetRenderDrawColor(render, 0x18, 0x18, 0xFF, 0xFF);
				SDL_RenderDrawLine(render, xCenterPos, yCenterPos, xCenterPos + dx[i], yCenterPos + dy[i]);
			}

		}

	}
}
void drawLabyrinth()
{
	SDL_SetRenderDrawColor(render, 0x18, 0x18, 0xFF, 0xFF);


	//Top Border
	SDL_RenderDrawLine(render, LIMIT_AREA_LEFT, LIMIT_AREA_TOP, LIMIT_AREA_RIGHT, LIMIT_AREA_TOP);

	//Right External Border
	SDL_RenderDrawLine(render, LIMIT_AREA_RIGHT, LIMIT_AREA_TOP, LIMIT_AREA_RIGHT, LIMIT_AREA_BOTTOM);
	
	//bottomo Border
	SDL_RenderDrawLine(render, LIMIT_AREA_RIGHT, LIMIT_AREA_BOTTOM, LIMIT_AREA_LEFT, LIMIT_AREA_BOTTOM);

	//Left External Border
	SDL_RenderDrawLine(render, LIMIT_AREA_LEFT, LIMIT_AREA_BOTTOM, LIMIT_AREA_LEFT, LIMIT_AREA_TOP);


	SDL_Rect zoneToColor;

	zoneToColor.w = INTERNAL_BORDER_TICKNESS;
	zoneToColor.h = INTERNAL_BORDER_TICKNESS;


	zoneToColor.y = LIMIT_AREA_TOP;
	for (int row = 0; row < TOTAL_ROWS; row++)
	{
		zoneToColor.x = LIMIT_AREA_LEFT;
		for (int col = 0; col < TOTAL_COLUMNS; col++)
		{
			//This line been changed to pull the cells info from the Labyrinth class
			// instead of an internal int array in main2.cpp...
			if (labyrinth->getLabyrinthCell(col, row) != 0)    SDL_RenderFillRect(render, &zoneToColor);

			zoneToColor.x += INTERNAL_BORDER_TICKNESS;
		}

		zoneToColor.y += INTERNAL_BORDER_TICKNESS;
	}


}


void handleMouseClick(int x, int y)
{


	int cellCenterX = INTERNAL_BORDER_TICKNESS / 2;
	int cellCenterY = INTERNAL_BORDER_TICKNESS / 2;


	//int correctedX = x - LIMIT_AREA_LEFT - cellCenterX;
	//int correctedY = y - LIMIT_AREA_TOP - cellCenterY;
	int correctedX = x - LIMIT_AREA_LEFT;
	int correctedY = y - LIMIT_AREA_TOP;

	//If the click is outside the Canvas, then there is no
	//point to do the rest of the tests...
	if (correctedX < 0 || correctedY < 0)	return;


	//The corrected coords, once divided by the cell size,
	//will give an idea in which cell zone the click been...
	//But all the clicks slightly to the left, or over a target
	//will be penalized, as the divide will point (error) to the neighor cell.
	//Hence, here is why a modulo is used:
	//A small modulo tells that the click is close to the right or at the bottom of a cell (GOOD)
	//But a gib modulo, close to the INTERNAL_BORDER_TICKNESS, tells on the other hand that the 
	//click is in fact very close to the next cell, so we need to adjust the calculation..
	int actualCol = correctedX / INTERNAL_BORDER_TICKNESS;
	int actualRow = correctedY / INTERNAL_BORDER_TICKNESS;
	int moduloX = correctedX % INTERNAL_BORDER_TICKNESS;
	int moduloY = correctedY % INTERNAL_BORDER_TICKNESS;


	if (moduloX >= 40) actualCol++;
	if (moduloY >= 40) actualRow++;

	int expectedX = actualCol * INTERNAL_BORDER_TICKNESS + cellCenterX;
	int expectedY = actualRow * INTERNAL_BORDER_TICKNESS + cellCenterY;
	GraphNode* node = nullptr;



	printf("Click [%d,%d]\tCorrected [%d,%d]\tExpected [%d,%d]\n", x, y, correctedX, correctedY, expectedX, expectedY);
	if ( abs(expectedX - correctedX) < 10  && abs(expectedY - correctedY) < 10)
	{
		printf("Col & Row -->  %d,%d\n", actualCol, actualRow);

		
		node = labyrinth->getNodeForCoords(actualCol, actualRow);

		if (node != nullptr)
		{
			printf("A node can be found at [%d,%d], has %d neighbour(s)\n", actualCol, actualRow, node->getNeighborCount());
		}
		else
			printf("No node found for coords [%d,%d]\n", actualCol, actualRow);

	}
	

}


int cleanUp()
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();

	delete labyrinth;

	return 0;
}



int main(int argc, char* argv[])
{

	init();
	
	// annimation loop
	while (!close) {
		SDL_Event event;

		// Events mangement
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_QUIT:
				// handling of close button
				close = 1;
				break;

			case SDL_MOUSEMOTION:
				//printf("Mouse Move %d,%d\n", event.motion.x, event.motion.y);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					handleMouseClick(event.motion.x, event.motion.y);
				}
				break;

			case SDL_KEYDOWN:
				// keyboard API for key pressed
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					//dest.y -= speed / 30;
					break;
				case SDL_SCANCODE_LEFT:
					//dest.x -= speed / 30;
					break;
				case SDL_SCANCODE_DOWN:
					//dest.y += speed / 30;
					break;
				case SDL_SCANCODE_RIGHT:
					//dest.x += speed / 30;
					break;

				case SDL_SCANCODE_R:
					resetGhosts();
					break;


				case SDL_SCANCODE_F1:
					displayCell = !displayCell;
					break;
				case SDL_SCANCODE_F2:
					displayCellLinks = !displayCellLinks;
					break;
				case SDL_SCANCODE_F3:
					displayCellCoords = !displayCellCoords;
					break;
				case SDL_SCANCODE_ESCAPE:
					// handling of close button
					//cl.draw(render); 
					close = 1;
					break;
				default:
					break;
				}
			}
		}


		update();

		redrawScreen();


		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}


	cleanUp();

	return 0;
}