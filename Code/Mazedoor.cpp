#include "Mazedoor.h"
SDL_Texture* MazeDoor::doorTexture;
MazeDoor::MazeDoor(std::shared_ptr<Cell> setRoom) :
	MazeObject::MazeObject(setRoom) {
	if (doorTexture == NULL)
	{
		doorTexture = IMG_LoadTexture(objRenderer, "res/images/door.png");
		if (doorTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf("res/images/door.png");
			return;
		}
	}
	curObjTexture = doorTexture;
}
MazeDoor::~MazeDoor()
{
}