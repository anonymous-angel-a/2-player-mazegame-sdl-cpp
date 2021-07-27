#pragma once
#include <iostream>
#include "MazeObject.h"
#include "Cell.h"

class Player :
	public MazeObject
{
public:
	static SDL_Texture* playerTexture;
	std::shared_ptr<Cell> startRoom;
	int playerLives = 10;
	int score = 0;
	bool hasKey = false;
	Player(std::shared_ptr<Cell> setRoom, SDL_Renderer* renderer);
	Player();
	~Player();
	void SetPlayerToStart(std::shared_ptr<Cell> room);
	void ResetPlayer(std::shared_ptr<Cell> room, int lives);
	void SetStartRoom(std::shared_ptr<Cell> room);
};




