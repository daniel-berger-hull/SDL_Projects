#pragma once

#include <stdio.h>

#include <list>


#include <SDL.h>
#include "Sprite.h"



class SpriteManager
{
	public:
		int  getSpriteCount()						{    return spritesList.size();		};

		void addSprite(Sprite* newSprite);
		void updateSprites();
		void drawSprites(SDL_Renderer* render);

		void detectCollisions();
		void detectCollisions(Sprite** spriteArray, int spriteTotal);


	private:
		std::list<Sprite*> spritesList;
};

