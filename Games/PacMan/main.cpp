#include <stdio.h>	

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>



#include "..\DrawExample\Global.h"

#include "..\DrawExample\Sprite.h"


/*
#define	SCREEN_WIDTH   1000
#define SCREEN_HEIGHT  700

*/






SDL_Window* win;
SDL_Renderer* render;
SDL_Surface* surface;
//SDL_Surface* surface2;

SDL_Texture* tex;
SDL_Texture* tex2;

SDL_Rect dest;
SDL_Rect dest2;


// controls annimation loop
int close = 0;
int speed = 300;
SDL_Point points[20];

Sprite redGhost;
Sprite greenGhost;



int init()
{
	// retutns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	
	win = SDL_CreateWindow("GAME", // creates a window
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	// creates a renderer to render our images
	render = SDL_CreateRenderer(win, -1, render_flags);



	/*

	// please provide a path for your image
	surface = IMG_Load("Ghost_Red.png");

	// loads image to our graphics hardware memory.
	tex = SDL_CreateTextureFromSurface(render, surface);


	// clears main-memory
	SDL_FreeSurface(surface);

	// connects our texture with dest to control position
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	// adjust height and width of our image box.
	dest.w /= 15;
	dest.h /= 15;

	// sets initial x-position of object
	dest.x = (SCREEN_WIDTH - dest.w) / 2;

	// sets initial y-position of object
	dest.y = (SCREEN_HEIGHT - dest.h) / 2;

	surface = IMG_Load("Ghost_Green.png");

	// loads image to our graphics hardware memory.
	tex2 = SDL_CreateTextureFromSurface(render, surface);

	// clears main-memory
	SDL_FreeSurface(surface);

	// connects our texture with dest to control position
	SDL_QueryTexture(tex2, NULL, NULL, &dest2.w, &dest2.h);

	// adjust height and width of our image box.
	dest2.w /= 15;
	dest2.h /= 15;

	// sets initial x-position of object
	dest2.x = (SCREEN_WIDTH - dest2.w) / 2;

	// sets initial y-position of object
	dest2.y = (SCREEN_HEIGHT - dest2.h) / 2;

	*/

	 redGhost.build( render, "Ghost_Red.png");
	 greenGhost.build(render, "Ghost_Green.png");




	for (int i = 0; i < 20; i++)
	{
		points[i] = { 20 + (i * 2),20 + (i * 2) };
	}

	return 0;
}

int update()
{


	redGhost.update();
	greenGhost.update();





	

	/*

	dest.x++;

	// right boundary
	if (dest.x + dest.w > SCREEN_WIDTH)
		dest.x = 0;

	// left boundary
	if (dest.x < 0)
		dest.x = 0;

	// bottom boundary
	if (dest.y + dest.h > SCREEN_HEIGHT)
		dest.y = SCREEN_HEIGHT - dest.h;

	// upper boundary
	if (dest.y < 0)
		dest.y = 0;
		*/
	return 0;
}

int redrawScreen()
{

	SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(render);

	
	SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF);
	status = SDL_RenderDrawPoints(render, points, 20);


	redGhost.draw( render );
	greenGhost.draw(render );





	// triggers the double buffers
	// for multiple rendering
	SDL_RenderPresent(render);
	return 0;
}

int cleanUp()
{

	// destroy texture
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(tex2);


	// destroy renderer
	SDL_DestroyRenderer(render);

	// destroy window
	SDL_DestroyWindow(win);

	// close SDL
	SDL_Quit();

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

			case SDL_KEYDOWN:
				// keyboard API for key pressed
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					dest.y -= speed / 30;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					dest.x -= speed / 30;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					dest.y += speed / 30;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					dest.x += speed / 30;
					break;
				case SDL_SCANCODE_ESCAPE:
					// handling of close button
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
