#include "SpriteManager.h"



void SpriteManager::addSprite(Sprite* newSprite)
{
	if (newSprite == nullptr)  return;

	spritesList.push_back(newSprite);
}


void SpriteManager::updateSprites()
{

	for (Sprite* next : spritesList )
		next->update();
	//for (int i = 0; i < spritesList.size();i++)
		//spritesList[i]->update();
}

void SpriteManager::drawSprites(SDL_Renderer* render)
{
	for (Sprite* next : spritesList)
		next->draw(render);
}


void SpriteManager::detectCollisions()
{
	if (spritesList.size() <= 0)	 return;


	for (Sprite* currentSprite : spritesList)
	{
		SDL_Rect  currentRect = currentSprite->getRect();

		int xMin = currentRect.x;
		int xMax = xMin + currentRect.w;
		int yMin = currentRect.y;
		int yMax = yMin + currentRect.h;

		SDL_Rect  getRect();
		void	  setRect(SDL_Rect rect);

		for (Sprite* compareTo : spritesList)
		{
			if (compareTo == currentSprite)		continue;

			//Sprite* compareTo = spriteArray[j];
			SDL_Rect compareRect = compareTo->getRect();

			// Exclude the situations where the coords can't have a possible collision...
			if (xMax < compareRect.x)						continue;
			if (xMin > (compareRect.x + compareRect.w))		continue;

			if (yMax < compareRect.y)						continue;
			if (yMin > (compareRect.y + compareRect.h))		continue;


			currentSprite->reverseDirection();
			compareTo->reverseDirection();

			/*printf("Collision detected on sprite %d and %d at coords %d,%d \n", currentSprite->getId(),
																				compareTo->getId(),
																				currentRect.x, currentRect.y);*/


		
		}

	}


}



void SpriteManager::detectCollisions(Sprite** spriteArray, int spriteTotal)
{
	if (spriteArray == NULL) return;
	if (spriteTotal <= 0)	 return;


	for (int i = 0; i < spriteTotal-1; i++)
	{
		Sprite* currentSprite = spriteArray[i];
		SDL_Rect  currentRect = currentSprite->getRect();

		int xMin = currentRect.x;
		int xMax = xMin + currentRect.w;
		int yMin = currentRect.y;
		int yMax = yMin + currentRect.h;




		SDL_Rect  getRect();
		void	  setRect(SDL_Rect rect);


		for (int j = i + 1; j < spriteTotal; j++)
		{
			Sprite* compareTo = spriteArray[j];
			SDL_Rect compareRect = compareTo->getRect();

			// Exclude the situations where the coords can't have a possible collision...
			if ( xMax < compareRect.x  )				   continue;
			if ( xMin > (compareRect.x + compareRect.w))   continue;

			if ( yMax < compareRect.y  )				   continue;
			if ( yMin > (compareRect.y + compareRect.h) )  continue;

			// then by here, 
			
			//currentSprite->setDirection(SPRITE_NO_DIRECTION);
			currentSprite->reverseDirection();
			compareTo->reverseDirection();

			printf("Collision %d  and %d \n", i, j);

//			printf("Collision %d  and %d direction = [ %d ]\n", i, currentSprite->getDirection());
//			printf("\t --> Potential Collision with  %d\n", j);
		}




	}
		
	
}

