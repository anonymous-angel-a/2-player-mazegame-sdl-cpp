#include "MazeGuard.h"
SDL_Texture* MazeGuard::guardTexture;
MazeGuard::MazeGuard(std::shared_ptr<Cell> setRoom):
	guardCenterRoomPtr(setRoom), guardCurRoomPtr(setRoom), MazeObject(setRoom){
	if (guardTexture == NULL)
	{
		guardTexture = IMG_LoadTexture(objRenderer, "res/images/guard.png");
		if (guardTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf("res/images/guard.png");
			return;
		}
	}
	curObjTexture = guardTexture;
}

MazeGuard::~MazeGuard()
{
}

