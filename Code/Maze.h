#ifndef MAZE_H_
#define MAZE_H_
#pragma once
#include <stdc++.h>
#include "Cell.h"
#include <vector>
#include <SDL.h>
#include "Player.h"
#include "Mazekey.h"
#include "Mazedoor.h"
#include "MazeDiamond.h"
#include "MazeGuard.h"

using namespace std;

class Maze {
private:
	void add_walls();

public:
	int height;
	int width;
	int startPos_x = 0;
	int startPos_y = 0;
	int finalPos_x;
	int finalPos_y;
	SDL_Renderer* mazeRenderer = NULL;
	vector<vector<Cell>> maze;
	std::shared_ptr<Mazekey> mazeKeyPtr;
	std::shared_ptr<MazeDoor> mazeDoorPtr;
	std::vector<shared_ptr<Mazediamond>> mazeDiamondPtrs;
	std::vector<shared_ptr<MazeGuard>> mazeGuardPtrs;
	Maze(const int inputHeight = 10, const int inputWidth = 10);
	void displayMazeDimensions() const;	 
	void initialiseMaze(SDL_Renderer* roomRenderer);
	shared_ptr<Cell> FindRoomByPos(int x, int y);
	void NextMazeCycle();
	void displayMaze(SDL_Renderer* gRenderer,int x, int y, int SCREEN_WIDTH,int SCREEN_HEIGHT);
	void generateMaze();
	void CreateKey();
	void CreateDoor();
	void CreateDiamonds();
	void CreateGuards();
	void initialiseMaze(SDL_Renderer* roomRenderer, int GameArea_w, int GameArea_h, int GameArea_y, int GameArea_x);
};

#endif