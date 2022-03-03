// MetaBall.cpp : 
// This file contains an experiment with the metaball concept, where
// spheres will move on the screen and as they get closer, their respective circonference 
// will merge.
// What is particular with this type of project, is that it is not the objects themselves who 
// are calculated and drawn, but ALL the pixels of the screen are calculated, based on their
// interactions and proximity with the metaballs. It is more aligned with CAD application,
// like Blender...
//
// Nov 14, 2021: With this project, there is an envelope melting effect as expected with the
//               metaballs, but the color is black, while it should be a level of gray instead
//               This may be due to one of the many modifications I had to add
//               to have a least something working (the modulo % and if > than something)
//				 WILL need to recheck this...


#include <SDL.h>
#include <stdio.h>

#include <iostream>
#include <cmath>

#include <Windows.h>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//Starts up SDL and creates window
bool initSDL();
void close();
void updateBalls();
int  redrawScreen();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;



DWORD  lastTimeTime = 0;
DWORD  currentTime = 0;
int counter = 0;


struct MetaBall {
	int xPos;
	int yPos;
	int dx = 1;
	int dy = 1;
	int radius;
};

MetaBall balls[4];

bool initSDL()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Galois Random Number", // creates a window
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
			printf("Bytes Per Pixel =   %d\n", gScreenSurface->format->BytesPerPixel);
			printf("pitch =   %d\n", gScreenSurface->pitch);
			printf("width =   %d\n", gScreenSurface->w);
			printf("height =   %d\n", gScreenSurface->h);
			printf("Initialisation completed...\n");


			lastTimeTime = GetTickCount();

			balls[0].xPos = 40;
			balls[0].yPos = 40;
			balls[0].dx = -5;
			balls[0].dy = 2;
			balls[0].radius = 35;

			balls[1].xPos = 120;
			balls[1].yPos = 30;
			balls[1].dx = 2;
			balls[1].dy = 6;
			balls[1].radius = 35;

			balls[2].xPos = 450;
			balls[2].yPos = 40;
			balls[2].dx = -3;
			balls[2].dy = 1;
			balls[2].radius = 35;

			balls[3].xPos = 70;
			balls[3].yPos = 300;
			balls[3].dx = 6;
			balls[3].dy = 0;
			balls[3].radius = 35;
		}
	}

	return success;
}

void close()
{
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;


	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void updateBalls()
{

	for (int index = 0; index < 4; index++)
	{
		balls[index].xPos += balls[index].dx;
		balls[index].yPos += balls[index].dy;

		if (balls[index].xPos <= 0 || balls[index].xPos > SCREEN_WIDTH)    balls[index].dx *= -1;
		if (balls[index].yPos <= 0 || balls[index].yPos > SCREEN_HEIGHT)   balls[index].dy *= -1;
	}
}


int redrawScreen()
{

	int width = gScreenSurface->w;
	int height = gScreenSurface->h;

	unsigned int* video_mem = (unsigned int*)gScreenSurface->pixels;

	Uint32 color;
	Uint32 alpha = (0xff << 24);


	//General Equation of a circle   is:
			//        F(x, y) = (x - x0) ^ 2 + (y - y0) ^ 2 = R ^ 2

	for (int x = 1; x < width; x++)
		for (int y = 1; y < height; y++)
		{
			unsigned char accumulatedValue = 0;
	
			for (int index = 0; index < 4; index++)
			{
				int   squaredRadiusTest = balls[index].radius * balls[index].radius;
				float squaredRadius = (float)(balls[index].radius * balls[index].radius);

				int distanceSquare = ((x - balls[index].xPos) * (x - balls[index].xPos)) +
					((y - balls[index].yPos) * (y - balls[index].yPos));

				float distance = sqrt((float)distanceSquare);

				// Outside the radius...
				//if (distanceSquare > squaredRadiusTest)
				//if (distance > squaredRadius)
				if (distance > (float)balls[index].radius)
				{
					float ratio = 150.0 * (float)balls[index].radius / sqrt((float)distanceSquare);

					accumulatedValue += (unsigned char)ratio;
					if (accumulatedValue >= 256)
					{
						accumulatedValue = 255;
						break;
					}
				}
				else
				{
					accumulatedValue = 255;
					break;
				}

			}


			// There appears to have the most significant bytes as alpha, and then the red, green and blue values...
//			color =  (Alphs << 24) | (red << 16) | (green << 8) | (blue << 0); 
//			i.e pure red is --> color = alpha | (0xff << 16) | (0x00 << 8) | 0x00;  
			//color = alpha | ((accumulatedValue % 256) << 16) | ((accumulatedValue % 256) << 8) | accumulatedValue;

			accumulatedValue %= 256;
			color = alpha | (accumulatedValue  << 16) | (accumulatedValue  << 8) | accumulatedValue;


			video_mem[x + y * width] = 0xffff00ff;
			video_mem[x + y * width] = color;
		}

	SDL_UpdateWindowSurface(gWindow);


	DWORD current = GetTickCount();
	DWORD delta = current - lastTimeTime;
	lastTimeTime = current;


	printf("%d\n", delta);
	//SDL_Delay(2);
	updateBalls();

	return 0;
}





int main(int argc, char* args[])
{

	//Start up SDL and create window
	if (!initSDL())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//User presses a key
				else if (e.type == SDL_KEYDOWN)
				{
					//Select surfaces based on key press
					switch (e.key.keysym.scancode)
					{
					case SDL_SCANCODE_ESCAPE:
						// handling of close button
						//cl.draw(render); 
						quit = true;
						break;
					case SDL_SCANCODE_UP:
						//gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
						printf("Down\n");
						break;

					case SDL_SCANCODE_DOWN:
						//gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
						printf("Up\n");
						break;


					}

				}
			}

			redrawScreen();
			// calculates to 60 fps
			//SDL_Delay(1000 / 60);
			//SDL_Delay(5);
		}

	}

	//Free resources and close SDL
	close();

	return 0;
}

