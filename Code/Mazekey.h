#pragma once
#include "MazeObject.h"
class Mazekey :
	public MazeObject
{
private:
public:
	static SDL_Texture* keyTexture;
	Mazekey(std::shared_ptr<Cell> setRoom);
	~Mazekey();
};
