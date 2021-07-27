#pragma once
#include "MazeObject.h"
class Mazediamond :
	public MazeObject
{
private:
	static SDL_Texture* diamondTexture;
public:
	Mazediamond(std::shared_ptr<Cell> setRoom);
	~Mazediamond();
	bool consumed = false;
};
