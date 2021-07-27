#pragma once
#include "Player.h"
SDL_Texture* Player::playerTexture;
Player::Player(std::shared_ptr<Cell> setRoom, SDL_Renderer* renderer):
MazeObject::MazeObject(setRoom)
{
	startRoom = setRoom;
	playerTexture = IMG_LoadTexture(objRenderer, "res/images/princess.png");
	if (playerTexture == NULL)
	{
		printf("SDL could not load the Texture from ");
		printf("res/images/princess.png");
		return;
	}
	curObjTexture = playerTexture;
}

Player::Player()
{
}

Player::~Player()
{
}
void Player::SetPlayerToStart(std::shared_ptr<Cell> room) {
	hasKey = false;
	SetStartRoom(room);

}
void Player::ResetPlayer(std::shared_ptr<Cell> room, int lives) {
	playerLives = lives;
	hasKey = false;
	SetStartRoom(room);

}
void Player::SetStartRoom(std::shared_ptr<Cell> room)
{
	startRoom = room;
	SetObjRoom(room);
}