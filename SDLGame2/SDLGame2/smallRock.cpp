#include "smallRock.h"
#include <iostream>
#include <cstdlib>

using namespace std;

SmallRock::SmallRock(SDL_Renderer* renderer, float x, float y)
{
	active = false;
	speed = 400.0;
	SDL_Surface* surface = IMG_Load("./Assets/AsteroidSmall.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;
	posRect.x = x;
	posRect.y = y;
	pos_X = x;
	pos_Y = y;
	xDir = 0;
	yDir = 0;

	rockAngle = 0.0f;
	rockCenter.x = posRect.w / 2;
	rockCenter.y = posRect.h / 2;
}

void SmallRock::Reposition(float x, float y)
{
	active = true;

	posRect.x = x;
	posRect.y = y;
	pos_X = x;
	pos_Y = y;
	speed = rand() % 2 + 3;
	speed *= 100;
	int upOrDown = rand() % 2 + 1;
	int leftOrRight = rand() % 2 + 1;

	if (upOrDown == 1)
	{
		if (leftOrRight == 1)
		{
			xDir = -1;
			yDir = -1;
		}
		else
		{
			xDir = 1;
			yDir = -1;
		}
	}
	else 
	{
		if (leftOrRight == 1)
		{
			xDir = -1;
			yDir = 1;
		}
		else
		{
			xDir = 1;
			yDir = 1;
		}
	}
}

void SmallRock::Deactivate()
{
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void SmallRock::Update(float deltaTime)
{
	if (active)
	{
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;
		posRect.x = (int)(pos_X + 0.05f);
		posRect.y = (int)(pos_Y + 0.05f);

		rockAngle += .1;

		if (posRect.x < (0 - posRect.w))
		{
			posRect.x = 1024;
			pos_X = posRect.x;
		}

		if (posRect.x > 1024)
		{
			posRect.x = (0 - posRect.w);
			pos_X = posRect.x;
		}

		if (posRect.y < (0 - posRect.w))
		{
			posRect.y = 768;
			pos_Y = posRect.y;
		}

		if (posRect.y > 768)
		{
			posRect.y = (0 - posRect.w);
			pos_Y = posRect.y;
		}
	}
}

void SmallRock::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);
}

SmallRock::~SmallRock()
{
	//SDL_DestroyTexture(texture);
}