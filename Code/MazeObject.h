#pragma once
#include "SDL_rect.h"
#include "SDL_image.h"
#include "Cell.h"
#include <cmath>
#include <memory>


enum objectTypes { key, door, diamond, guard};
class MazeObject
{
public:
	std::shared_ptr<Cell> curObjRoom;
	SDL_Texture* curObjTexture = NULL;
	SDL_Renderer* objRenderer = NULL;
	SDL_Rect objRect;
	int objPos_x;
	int objPos_y;

	//Constructors and Destructors
	MazeObject(std::shared_ptr<Cell> room);
	MazeObject(const MazeObject& other);
	MazeObject();
	~MazeObject();
	virtual void NextCycle()
	{}

	//Sets the object's variables based off its room
	void SetObjRoom(std::shared_ptr<Cell> setRoom);

	//Sets the Rect Area for the Object
	void SetObjectRect(SDL_Rect* rect);

	//Add this object to the renderer
	void AddObjToRenderer();
};

