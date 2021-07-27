#include "MazeKey.h"
SDL_Texture* Mazekey::keyTexture;

Mazekey::Mazekey(std::shared_ptr<Cell> setRoom):
	MazeObject::MazeObject(setRoom) {
	if (keyTexture == NULL)
	{
		keyTexture = IMG_LoadTexture(objRenderer, "res/images/key.png");
		if (keyTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf("res/images/key.png");
			return;
		}
	}
	curObjTexture = keyTexture;
}
Mazekey::~Mazekey()
{
}