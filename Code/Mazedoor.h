#pragma once
#include "MazeObject.h"
class MazeDoor :
	public MazeObject
{
private:
	static SDL_Texture* doorTexture;
public:
	MazeDoor(std::shared_ptr<Cell> setRoom);
	~MazeDoor();
};
