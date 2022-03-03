
#include <SDL.h>
#include <stdio.h>

#include <cmath>

#include <Windows.h>


//Screen dimension constants
const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 100;

typedef struct {
	float x, y;
} vector2;


int perlinValues[SCREEN_WIDTH][SCREEN_HEIGHT];

//Starts up SDL and creates window
bool initSDL();
void close();
int  redrawScreen();


float   interpolate(float a0, float a1, float w);
vector2 randomGradient(int ix, int iy);
float   dotGridGradient(int ix, int iy, float x, float y);
float   perlin(float x, float y);






SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;


DWORD  lastTimeTime = 0;
DWORD  currentTime = 0;
int counter = 0;



static int TestThread(void* ptr)
{
	int cnt;

	for (cnt = 0; cnt < 10; ++cnt) {
		printf("Thread counter: %d\n", cnt);
		SDL_Delay(50);
	}

	return cnt;
}



void testPerlin()
{


	printf("Testin Perlin Noise:\n");


	for (float x = 0.0; x < 10.0; x+=0.1)
	{
		int xIndex = (int)(x * 10.0);
		printf("%d\n", xIndex);
		for (float y = 0.0; y < 10.0; y+=0.1)
		{
			float result = perlin(x, y);

			//printf("%2.4f  ", result);
			int yIndex = (int)(y * 10.0);

			
			perlinValues[xIndex][yIndex] = ((int)(result * 128.0) + 127) % 256;



		}

		//printf("\n");
	}

	 

}


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
		gWindow = SDL_CreateWindow("Perlin Noise", // creates a window
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

			printf("Simple SDL_CreateThread test:\n");



			testPerlin();
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



int redrawScreen()
{

	int width = gScreenSurface->w;
	int height = gScreenSurface->h;

	unsigned int* video_mem = (unsigned int*)gScreenSurface->pixels;

	Uint32 color;
	Uint32 alpha = (0xff << 24);



	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			unsigned char accumulatedValue = 0;

		
			// There appears to have the most significant bytes as alpha, and then the red, green and blue values...
//			color =  (Alphs << 24) | (red << 16) | (green << 8) | (blue << 0); 
//			i.e pure red is --> color = alpha | (0xff << 16) | (0x00 << 8) | 0x00;  
			//color = alpha | ((accumulatedValue % 256) << 16) | ((accumulatedValue % 256) << 8) | accumulatedValue;

			accumulatedValue %= 256;
			
			
//			color = alpha | (accumulatedValue << 16) | (accumulatedValue << 8) | accumulatedValue;
			color = alpha | (perlinValues[x][y] << 16) | (perlinValues[x][y] << 8) | perlinValues[x][y];



			
			//video_mem[x + y * width] = 0xffff00ff;
			video_mem[x + y * width] = color;
		}

	SDL_UpdateWindowSurface(gWindow);


	DWORD current = GetTickCount();
	DWORD delta = current - lastTimeTime;
	lastTimeTime = current;


	//printf("%d\n", delta);
	//SDL_Delay(2);
	

	return 0;
}



float interpolate(float a0, float a1, float w) {
	/* // You may want clamping by inserting:
	 * if (0.0 > w) return a0;
	 * if (1.0 < w) return a1;
	 */
	return (a1 - a0) * w + a0;
	/* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
	 * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
	 *
	 * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
	 * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
	 */
}


vector2 randomGradient(int ix, int iy) {
	// No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2; // rotation width
	unsigned a = ix, b = iy;
	a *= 3284157443; b ^= a << s | a >> w - s;
	b *= 1911520717; a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
	vector2 v;
	v.x = sin(random); v.y = cos(random);
	return v;
}


// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
	// Get gradient from integer coordinates
	vector2 gradient = randomGradient(ix, iy);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx * gradient.x + dy * gradient.y);
}


// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
	// Determine grid cell coordinates
	int x0 = (int)x;
	int x1 = x0 + 1;
	int y0 = (int)y;
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = interpolate(n0, n1, sx);

	value = interpolate(ix0, ix1, sy);
	return value;
}



int main(int argc, char* args[])
{

	//Start up SDL and create window
	/*if (!initSDL())
	{
		printf("Failed to initialize!\n");
	}*/
	if (!initSDL())
	{
		printf("Failed to initialize!\n");
		close();
		return -1;
	}


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

	

	//Free resources and close SDL
	close();

	return 0;
}

