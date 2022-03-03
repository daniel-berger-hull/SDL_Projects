#pragma once


#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "Global.h"


#define   SPRITE_NO_DIRECTION      0
#define   SPRITE_DIRECTION_UP      1
#define   SPRITE_DIRECTION_RIGHT   2
#define   SPRITE_DIRECTION_DOWN    3
#define   SPRITE_DIRECTION_LEFT    4


#define	 NO_ROTATION				0.0


#define	TYPE_NOT_DEFINED		  0
#define	TYPE_RED_GHOST			  1
#define	TYPE_GREEN_GHOST		  2
#define	TYPE_ORANGE_GHOST		  3
#define	TYPE_PINK_GHOST			  4



class Sprite
{
	public:
		Sprite();
		~Sprite();

		void build( SDL_Renderer* render, const char* imageFileName );
		void draw( SDL_Renderer* render );
		virtual void update();

		bool isReady();

		long         getId()								{ return id; };
		SDL_Texture* getTexture();
		void         setTexture( SDL_Texture* tex );


		SDL_Rect  getRect();
		void	  setRect(SDL_Rect rect);
		void      setPosition(int x, int y);
		int		  getDirection();
		void	  setDirection(int d);
		virtual void      reverseDirection();

		int		  getType();
		void	  setType(int t);



	
	protected:
		long			id = TYPE_NOT_DEFINED;
		SDL_Texture*    texture = NULL;
		SDL_Rect		dest;
		int direction = SPRITE_NO_DIRECTION;

		char* textureFile = NULL;
		int   type		  = TYPE_NOT_DEFINED;

		static long nextID;
		

};


