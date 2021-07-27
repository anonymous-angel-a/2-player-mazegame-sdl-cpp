#include "MazeObject.h"
MazeObject::MazeObject(std::shared_ptr<Cell> room)
{
	curObjRoom = room;
	objRenderer = room->roomRenderer;
	objPos_x = room->row;
	objPos_y = room->col;
	SetObjectRect(&room->roomRect);

}
MazeObject::MazeObject(const MazeObject& other)
{
	curObjRoom = other.curObjRoom;
	objRenderer = other.objRenderer;
	objPos_x = other.objPos_x;
	objPos_y = other.objPos_y;
	curObjTexture = other.curObjTexture;
	objRect = other.objRect;

}

MazeObject::MazeObject()
{
}

MazeObject::~MazeObject()
{
}
void MazeObject::SetObjRoom(std::shared_ptr<Cell> setRoom) {
	curObjRoom = setRoom;
	objPos_x = setRoom->row;
	objPos_y = setRoom->col;
	SetObjectRect(&setRoom->roomRect);
}
void MazeObject::SetObjectRect(SDL_Rect* rect)
{
	objRect =
	{
		rect->x + rect->w / 8,
		rect->y + rect->h / 8,
		3*(rect->w) / 4,
		3*(rect->h) / 4
	};
}

//Add this object to the renderer
void MazeObject::AddObjToRenderer()
{
	SDL_RenderCopy(objRenderer, curObjTexture, NULL, &objRect);
}
