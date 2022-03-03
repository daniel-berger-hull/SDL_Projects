#include "Sprite.h"



long Sprite::nextID = 1;




Sprite::Sprite()
{
	this->id = nextID++;
	this->dest.x = 200;
	this->dest.y = 200;

	printf("Constructor of %p , #id (%d) called\n", this, this->id);
}


Sprite::~Sprite()
{
	printf("Destructor of %p called\n", this);
	
	SDL_DestroyTexture( this->texture );
}




bool Sprite::isReady()
{ 
	if (this->texture == NULL) return false;
	
	return true;
}


SDL_Texture* Sprite::getTexture()
{
	return this->texture;
}

void  Sprite::setTexture(SDL_Texture* tex)
{
	this->texture = tex;
}


SDL_Rect  Sprite::getRect()
{
	return this->dest;
}

void  Sprite::setRect(SDL_Rect rect)
{
	this->dest.x = rect.x;
	this->dest.y = rect.y;
	this->dest.w = rect.w;
	this->dest.h = rect.h;
}


void Sprite::setPosition(int x, int y)
{
	if (x < 0)			   x = 0;
	if (x > CANVAS_WIDTH)  x = CANVAS_WIDTH;

	if (y < 0)				y = 0;
	if (y > CANVAS_HEIGHT)  y = CANVAS_HEIGHT;


	this->dest.x = x;
	this->dest.y = y;
}

int Sprite::getDirection()
{
	return this->direction;
}
void Sprite::setDirection(int d)
{
	if (d >= SPRITE_NO_DIRECTION && d <= SPRITE_DIRECTION_LEFT)  
		this->direction = d;
	else 
		this->direction = SPRITE_NO_DIRECTION;
}


void Sprite::reverseDirection()
{
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


int Sprite::getType()
{
	return this->type;
}

void Sprite::setType(int t)
{
	if (t < TYPE_NOT_DEFINED)  t = TYPE_NOT_DEFINED;
	if (t > TYPE_PINK_GHOST)   t = TYPE_PINK_GHOST;

	this->type = t;
}



void Sprite::build(SDL_Renderer* render, const char* imageFileName)
{

	SDL_Surface* surface = IMG_Load( imageFileName );

	this->texture = SDL_CreateTextureFromSurface(render, surface);

	SDL_FreeSurface(surface);

	int width, heith;

//	SDL_QueryTexture( this->texture, NULL, NULL, &(this->dest.w), &(this->dest.w) );
	SDL_QueryTexture(this->texture, NULL, NULL, &width, &heith);


	width /= 15;
	heith /= 15;

	// adjust height and width of our image box.
	this->dest.w /= width;
	this->dest.h /= heith;

	// sets initial x-position of object
	this->dest.x = (CANVAS_WIDTH - this->dest.w) / 2;

	// sets initial y-position of object
	this->dest.y = (CANVAS_HEIGHT - this->dest.h) / 2;
}


void Sprite::draw(SDL_Renderer* render)
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	

	switch (this->direction)
	{
		case(SPRITE_DIRECTION_LEFT):
			flip = SDL_FLIP_HORIZONTAL;
			break;
	}


	SDL_RenderCopyEx(render,
		this->texture,
		NULL, &(this->dest),
		NO_ROTATION, NULL, flip);


	switch (this->type)
	{
		case(TYPE_RED_GHOST):
			SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0xFF);
			break;

		case(TYPE_GREEN_GHOST):
			SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF);
			break;

		case(TYPE_ORANGE_GHOST):
			SDL_SetRenderDrawColor(render, 0xFF, 0x7F, 0x26, 0xFF);
			break;
		
		case(TYPE_PINK_GHOST):
			SDL_SetRenderDrawColor(render, 0xFF, 0xAE, 0xC9, 0xFF);
			break;

		default:
			SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
			break;


	}


	if (DEBUG_DISPLAY_SPRITES_BOUNDARY)
	{
		int x1 = this->dest.x;
		int x2 = this->dest.x + this->dest.w;
		int y1 = this->dest.y;
		int y2 = this->dest.y + this->dest.h;

		SDL_RenderDrawLine(render, x1, y1, x2, y1);
		SDL_RenderDrawLine(render, x2, y1, x2, y2);
		SDL_RenderDrawLine(render, x2, y2, x1, y2);
		SDL_RenderDrawLine(render, x1, y2, x1, y1);
	}
	

}

void Sprite::update()
{
	int dx = 0;
	int dy = 0;

	
	switch (this->direction)
	{
		case(SPRITE_DIRECTION_UP):
			dy = -1;
		break;

		case(SPRITE_DIRECTION_RIGHT):
			dx = 1;
			break;

		case(SPRITE_DIRECTION_DOWN):
			dy = 1;
			break;

		case(SPRITE_DIRECTION_LEFT):
			dx = -1;
			break;

	}

	this->dest.x += dx;
	this->dest.y += dy;



	// right boundary
	if (this->dest.x + this->dest.w > LIMIT_AREA_RIGHT)
	{
		this->dest.x = LIMIT_AREA_RIGHT - this->dest.w;
		this->direction = SPRITE_DIRECTION_LEFT;
	}

	// left boundary
	if (this->dest.x < LIMIT_AREA_LEFT)
	{
		this->dest.x = LIMIT_AREA_LEFT;
		this->direction = SPRITE_DIRECTION_RIGHT;
	}

	// bottom boundary
	if (this->dest.y + this->dest.h > LIMIT_AREA_BOTTOM)
	{
		this->dest.y = LIMIT_AREA_BOTTOM - this->dest.h;
		this->direction = SPRITE_DIRECTION_UP;
	}

	// upper boundary
	if (this->dest.y < LIMIT_AREA_TOP)
	{
		this->dest.y = LIMIT_AREA_TOP;
		this->direction = SPRITE_DIRECTION_DOWN;
	}

}




