#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

bool quit = false;

float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

float x, y, playerAngle, oldAngle;
float pos_X, pos_Y;
float xDir, xDirOld, yDir, yDirOld, playerSpeed = 400.0f;
int playerLevel = 1;
int playerScore = 0;

SDL_Rect playerPos;
SDL_Point center;

#include <vector>
#include "bullet.h"
vector<Bullet> bulletList;
Mix_Chunk* laser;

void CreateBulet()
{
	for (int x = 0; x < bulletList.size(); x++)
	{
		if (bulletList[x].active == false)
		{
			Mix_PlayChannel(-1, laser, 0);

			bulletList[x].active = true;

			bulletList[x].posRect.x = pos_X;
			bulletList[x].posRect.y = pos_Y;

			bulletList[x].pos_X = pos_X;
			bulletList[x].pos_Y = pos_Y;

			bulletList[x].xDir = xDirOld;
			bulletList[x].yDir = yDirOld;

			bulletList[x].Reposition();
			break;
		}
	}
}

#include <cstdlib>
#include "largeRock.h"

vector<LargeRock> largeRockList;

#include "smallRock.h"
vector<SmallRock> smallRockList;

Mix_Chunk* explosion;

int main(int argc, char* argv[])
{
	srand(time(NULL));

	SDL_Window* window;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Space Rocks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* surface = IMG_Load("./Assets/Background.png");
	SDL_Texture* bkgd;
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect bkgdPos;
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	//.....................................................

	surface = IMG_Load("./Assets/Player.png");
	SDL_Texture* player;
	player = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 52;
	playerPos.h = 18;

	center.x = playerPos.w / 2;
	center.y = playerPos.h / 2;

	pos_X = playerPos.x;
	pos_Y = playerPos.y;
	xDir = 1;
	yDir = 0;
	xDirOld = 1;
	yDirOld = 0;

	//....................................................

	SDL_Event event;

	for (int x = 0; x < 10; x++)
	{
		Bullet tempBullet(renderer, -1000.0f, -1000.0f);
		bulletList.push_back(tempBullet);
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	laser = Mix_LoadWAV("./Assets/projectile.wav");
	explosion = Mix_LoadWAV("./Assets/explosion.wav");

	//....................................................

	int numberOfLargeRocks = 1, numberOfSmallRocks = 2;
	int totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;
	int totalRocksDestroyed = 0;

	for (int x = 0; x < numberOfLargeRocks; x++)
	{
		LargeRock tempRock(renderer, -1000.0f, -1000.0f);
		largeRockList.push_back(tempRock);
	}

	for (int x = 0; x < numberOfSmallRocks; x++)
	{
		SmallRock tempRock(renderer, -1000.0f, -1000.0f);
		smallRockList.push_back(tempRock);
	}

	for (int x = 0; x < numberOfLargeRocks; x++)
	{
		largeRockList[x].Reposition();
	}

	//...........................................
	cout << "You have reached level: " << playerLevel << endl;


	while (!quit)
	{
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			switch (event.type)
			{
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_SPACE:
							CreateBulet();
							break;
						default:
							break;
					}
			}
		}

		//.................................................
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			xDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			xDir = 1.0f;
		}
		else
		{
			xDir = 0.0f;
		}

		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			yDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			yDir = 1.0f;
		}
		else
		{
			yDir = 0.0f;
		}
		//.................................................
		if (xDir != 0 || yDir != 0)
		{
			x = playerPos.x - xDir;
			y = playerPos.y - yDir;

			playerAngle = atan2(yDir, xDir) * 180 / 3.14;

			oldAngle = playerAngle;
			xDirOld = xDir;
			yDirOld = yDir;
		}
		else
		{
			oldAngle = playerAngle;
		}

		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;

		playerPos.x = (int)(pos_X + 0.5f);
		playerPos.y = (int)(pos_Y + 0.5f);

		if (playerPos.x < (0 - playerPos.w))
		{
			playerPos.x = 1024;
			pos_X = playerPos.x;
		}

		if (playerPos.x >1024)
		{
			playerPos.x = (0 - playerPos.w);
			pos_X = playerPos.x;
		}

		if (playerPos.y < (0 - playerPos.w))
		{
			playerPos.y = 768;
			pos_Y = playerPos.y;
		}

		if (playerPos.y > 768)
		{
			playerPos.y = (0 - playerPos.w);
			pos_Y = playerPos.y;
		}

		for (int x = 0; x < bulletList.size(); x++)
		{
			if (bulletList[x].active == true)
			{
				bulletList[x].Update(deltaTime);
			}
		}

		for (int x = 0; x < largeRockList.size(); x++)
		{
			if (largeRockList[x].active == true)
			{
				largeRockList[x].Update(deltaTime);
			}
		}

		for (int x = 0; x < smallRockList.size(); x++)
		{
			if (smallRockList[x].active == true)
			{
				smallRockList[x].Update(deltaTime);
			}
		}

		for (int x = 0; x < bulletList.size(); x++)
		{
			if (bulletList[x].active == true)
			{
				for (int y = 0; y < largeRockList.size(); y++)
				{
					if (SDL_HasIntersection(&bulletList[x].posRect, &largeRockList[y].posRect))
					{
						Mix_PlayChannel(-1, explosion, 0);
						playerScore += 50;
						cout << "Current player score: " << playerScore << endl;
						int smallRockCounter = 0;

						for (int z = 0; z < smallRockList.size(); z++)
						{
							if (smallRockList[z].active == false)
							{
								smallRockList[z].Reposition(largeRockList[y].posRect.x, largeRockList[y].posRect.y);

								smallRockCounter++;
							}

							if (smallRockCounter == 2)
							{
								break;
							}
						}

						largeRockList[y].Deactivate();
						bulletList[x].Deactivate();
						totalRocksDestroyed++;
						
					}
				}
			}
		}

		for (int x = 0; x < bulletList.size(); x++)
		{
			if (bulletList[x].active == true)
			{
				for (int y = 0; y < smallRockList.size(); y++)
				{
					if (SDL_HasIntersection(&bulletList[x].posRect, &smallRockList[y].posRect))
					{
						Mix_PlayChannel(-1, explosion, 0);
						playerScore += 100;
						cout << "Current player score: " << playerScore << endl;
						smallRockList[y].Deactivate();
						bulletList[x].Deactivate();
						totalRocksDestroyed++;
						if (totalRocksDestroyed >= totalNumberOfRocks)
						{
							totalRocksDestroyed = 0;
							numberOfLargeRocks++;
							numberOfSmallRocks += 2;
							totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

							largeRockList.clear();
							smallRockList.clear();

							for (int x = 0; x < numberOfLargeRocks; x++)
							{
								LargeRock tempRock(renderer, -1000.0f, -1000.0f);
								largeRockList.push_back(tempRock);
							}

							for (int x = 0; x < numberOfSmallRocks; x++)
							{
								SmallRock tempRock(renderer, -1000.0f, -1000.0f);
								smallRockList.push_back(tempRock);
							}

							for (int x = 0; x < numberOfLargeRocks; x++)
							{
								largeRockList[x].Reposition();
							}
							//
							playerLevel++;
							cout << "You have reached level: " << playerLevel << endl;
						}
					}
				}
			}
		}

		//....................................................

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

		for (int x = 0; x < bulletList.size(); x++)
		{
			if (bulletList[x].active == true)
			{
				bulletList[x].Draw(renderer);
			}
		}

		for (int x = 0; x < largeRockList.size(); x++)
		{
			if (largeRockList[x].active == true)
			{
				largeRockList[x].Draw(renderer);
			}
		}

		for (int x = 0; x < smallRockList.size(); x++)
		{
			if (smallRockList[x].active == true)
			{
				smallRockList[x].Draw(renderer);
			}
		}

		SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}