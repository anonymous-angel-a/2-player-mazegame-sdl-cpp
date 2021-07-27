#pragma once
#include "MazeObject.h"
#include "Cell.h"

class MazeGuard :
	public MazeObject
{
public:
	static SDL_Texture* guardTexture;
	std::shared_ptr<Cell> guardCurRoomPtr;
	std::shared_ptr<Cell> guardCenterRoomPtr;
	bool dir = true;
	MazeGuard(std::shared_ptr<Cell> setRoom);
	~MazeGuard();
};