#pragma once
#include <stdc++.h>
#include "SDL.h"
#include <vector>
#include <algorithm>
#include "SDL_image.h"
#include <cmath>
#include <memory>
#include <string>


enum RoomType { None,Start,Final,Key, Diamond, Guard };
class Cell {
private:
	const int width = 1024;
	const int height = 768;
	int GameArea_W = 640;
	int GameArea_H = 640;
	int GameArea_xOff = 0;
	int GameArea_yOff = (height - GameArea_W);
public:
	bool visited = false;
	std::vector<RoomType> roomTypes;
	int row = 0;
	int col = 0;
	SDL_Renderer* roomRenderer = NULL;
	SDL_Rect roomRect;

	// only need to manipulate 2 walls for each cell since cells share walls
	bool northWall = true;
	bool eastWall = true;
	bool southWall = true;
	bool westWall = true;
	bool bomb = false;

	bool equal_Cell(Cell c1,Cell c2){
		if(c1.row==c2.row && c1.col==c2.col){
			return true;
		}
		return false;
	}
	void MakeRoomRect(int h, int w) {
		roomRect = {
			col * (GameArea_W / w),
			GameArea_yOff+ row * (GameArea_H / h),
			GameArea_W / w, GameArea_H / h
		};
	}
	void MakeRoomRect(int h, int w, int GameArea_w, int GameArea_h, int GameArea_y, int GameArea_x) {
		roomRect = {
			GameArea_x + col * (GameArea_w / w),
			GameArea_y + row * (GameArea_h / h),
			GameArea_w / w, GameArea_h / h
		};
	}

};

