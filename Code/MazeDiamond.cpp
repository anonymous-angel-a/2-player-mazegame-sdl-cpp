#include "Mazediamond.h"
SDL_Texture* Mazediamond::diamondTexture;
Mazediamond::Mazediamond(std::shared_ptr<Cell> setRoom):
	MazeObject::MazeObject(setRoom) {
	if (diamondTexture == NULL)
	{
		diamondTexture = IMG_LoadTexture(objRenderer, "res/images/Diamond.png");
		if (diamondTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf("res/images/Diamond.png");
			return;
		}
	}
	curObjTexture = diamondTexture;
}
Mazediamond::~Mazediamond() {

}