#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Bullet
{
public:
	bool active;
	SDL_Texture* texture;
	SDL_Rect posRect;
	float xDir, yDir, speed, pos_X, pos_Y;
	SDL_Point bulletCenter;
	SDL_Point playerCenter;

	Bullet(SDL_Renderer* renderer, float x, float y);

	void Reposition();
	void Update(float deltaTime);
	void Draw(SDL_Renderer* renderer);
	void Deactivate();
	~Bullet();
};