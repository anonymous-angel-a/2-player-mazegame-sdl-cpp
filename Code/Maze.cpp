#pragma once
#include "Maze.h"
#include "Cell.h"
#include "Mazekey.h"
#include "Mazedoor.h"
#include "MazeGuard.h"
#include <ctime>
#include <stdc++.h>
#include <SDL.h>
#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
using namespace std;

Maze::Maze(const int inputHeight, const int inputWidth)
  : height{ inputHeight }, width{ inputWidth } {
  cout << "Dimensions have been initialised\n";
}

void Maze::CreateKey() {
    int range = width-1;
    int Pos_x = rand() % range;
    range = height-1;
    int Pos_y = rand() % range;
    Cell curRoom = maze[Pos_x][Pos_y];
    std::shared_ptr<Cell> curRoomptr = make_shared<Cell>(curRoom);
    mazeKeyPtr = std::shared_ptr<Mazekey>(new Mazekey(curRoomptr));
    curRoom.roomTypes.push_back(Key);
}

void Maze::CreateDoor() {
    std::shared_ptr<Cell> curRoomptr = FindRoomByPos(width - 1, height - 1);
    mazeDoorPtr = std::shared_ptr<MazeDoor>(new MazeDoor(curRoomptr));
    curRoomptr->roomTypes.push_back(Final);
}
void Maze::CreateDiamonds() {
    int range = 20;
    int num = 15 + rand() % range;
    for (int i = 0; i < num; i++) {
        int range = width;
        int Pos_x = rand() % range;
        range = height;
        int Pos_y = rand() % range;
        if ((Pos_x == 0 && Pos_y == 0) || (Pos_x == finalPos_x && Pos_y == finalPos_y)) {
            i--;
            continue;
        }
        Cell curRoom = maze[Pos_x][Pos_y];
        std::shared_ptr<Cell> curRoomptr = make_shared<Cell>(curRoom);
        std::shared_ptr<Mazediamond> mazeDiamondPtr = std::shared_ptr<Mazediamond>(new Mazediamond(curRoomptr));
        curRoom.roomTypes.push_back(Diamond);
        mazeDiamondPtrs.push_back(mazeDiamondPtr);
    }
}
void Maze::CreateGuards() {
    std::vector<shared_ptr<Cell>> availablerooms;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((maze[i][j].southWall == false && maze[i][j].northWall == false && maze[i][j].eastWall == false) || (maze[i][j].southWall == false && maze[i][j].northWall == false && maze[i][j].westWall == false) || (maze[i][j].westWall == false && maze[i][j].northWall == false && maze[i][j].eastWall == false) || (maze[i][j].southWall == false && maze[i][j].westWall == false && maze[i][j].eastWall == false)) {
                availablerooms.push_back(FindRoomByPos(i, j));
            }
        }
    }
    int range = 5;
    int num = (4 + rand() % range) % availablerooms.size();  
    for (int i = 0; i < num; i++) {
        int index = rand() % availablerooms.size();
        std::shared_ptr<Cell> random = availablerooms[index];
        int Pos_x = random->row;
        int Pos_y = random->col;
        bool available=true;
        for (int j = 0; j < random->roomTypes.size(); j++) {
            if (random->roomTypes[j] == Guard || random->roomTypes[j]==Final) {
                available = false;
                break;
            }
        }
        if (!available) {
            i--;
            continue;
        }
        std::shared_ptr<MazeGuard> mazeGuardPtr = std::shared_ptr<MazeGuard>(new MazeGuard(random));
        random->roomTypes.push_back(Guard);
        if (i % 2 == 0) {
            mazeGuardPtr->dir = false;
        }
        mazeGuardPtrs.push_back(mazeGuardPtr);
    }
}
void Maze::NextMazeCycle() {
    for (int i = 0; i < mazeGuardPtrs.size(); i++) {
        std::shared_ptr<MazeGuard> mazeGuardptr = mazeGuardPtrs[i];
        if (mazeGuardptr->guardCenterRoomPtr->southWall == false && mazeGuardptr->guardCenterRoomPtr->northWall == false && (mazeGuardptr->guardCenterRoomPtr->eastWall == false|| mazeGuardptr->guardCenterRoomPtr->westWall == false)){
            if (mazeGuardptr->dir) {
                if (mazeGuardptr->guardCurRoomPtr->row == (mazeGuardptr->guardCenterRoomPtr->row + 1)) {
                    mazeGuardptr->dir = false;
                }
                else {
                    mazeGuardptr->guardCurRoomPtr = FindRoomByPos(mazeGuardptr->guardCurRoomPtr->row+1, mazeGuardptr->guardCurRoomPtr->col);
                    mazeGuardptr->SetObjRoom(mazeGuardptr->guardCurRoomPtr);
                }
            }
            if (!mazeGuardptr->dir) {
                if (mazeGuardptr->guardCurRoomPtr->row == (mazeGuardptr->guardCenterRoomPtr->row -1)) {
                    mazeGuardptr->dir = true;
                }
                else {
                    mazeGuardptr->guardCurRoomPtr = FindRoomByPos(mazeGuardptr->guardCurRoomPtr->row - 1, mazeGuardptr->guardCurRoomPtr->col);
                    mazeGuardptr->SetObjRoom(mazeGuardptr->guardCurRoomPtr);
                }
            }
        }
        if ((mazeGuardptr->guardCenterRoomPtr->southWall == false || mazeGuardptr->guardCenterRoomPtr->northWall == false) && (mazeGuardptr->guardCenterRoomPtr->eastWall == false && mazeGuardptr->guardCenterRoomPtr->westWall == false)) {
            if (mazeGuardptr->dir) {
                if (mazeGuardptr->guardCurRoomPtr->col == (mazeGuardptr->guardCenterRoomPtr->col + 1)) {
                    mazeGuardptr->dir = false;
                }
                else {
                    mazeGuardptr->guardCurRoomPtr = FindRoomByPos(mazeGuardptr->guardCurRoomPtr->row, mazeGuardptr->guardCurRoomPtr->col+1);
                    mazeGuardptr->SetObjRoom(mazeGuardptr->guardCurRoomPtr);
                }
            }
            if (!mazeGuardptr->dir) {
                if (mazeGuardptr->guardCurRoomPtr->col == (mazeGuardptr->guardCenterRoomPtr->col - 1)) {
                    mazeGuardptr->dir = true;
                }
                else {
                    mazeGuardptr->guardCurRoomPtr = FindRoomByPos(mazeGuardptr->guardCurRoomPtr->row, mazeGuardptr->guardCurRoomPtr->col-1);
                    mazeGuardptr->SetObjRoom(mazeGuardptr->guardCurRoomPtr);
                }
            }

        }
    }
}

void Maze::displayMazeDimensions() const {
  cout << "The width of the maze is " << width << endl;
  cout << "The height of the maze is " << height << endl;
}
std::shared_ptr<Cell> Maze::FindRoomByPos(int x, int y) {
    if (x >= height || y >= width) {
        Cell curRoom = maze[height-1][width-1];
        std::shared_ptr<Cell> Roomptr = make_shared<Cell>(curRoom);
        return Roomptr;
    }
    else {
        Cell curRoom = maze[x][y];
        std::shared_ptr<Cell> Roomptr = make_shared<Cell>(curRoom);
        return Roomptr;
    }
}

// create empty maze full of walls
void Maze::initialiseMaze(SDL_Renderer* Renderer){
    finalPos_x = height - 1; 
    finalPos_y = width - 1;
  for (int i = 0; i < height; i++) {
    vector<Cell> mazeRow;
    for (int j = 0; j < width; j++) {
      Cell cell;
      cell.row = i;
      cell.col = j;
      cell.roomRenderer = Renderer;
      cell.MakeRoomRect(height,width);
      mazeRow.push_back(cell);
    }
    maze.push_back(mazeRow);
  }
  cout << "Maze has been intialised\n";
}
void Maze::initialiseMaze(SDL_Renderer* Renderer, int GameArea_w, int GameArea_h, int GameArea_y, int GameArea_x) {
    finalPos_x = height - 1;
    finalPos_y = width - 1;
    for (int i = 0; i < height; i++) {
        vector<Cell> mazeRow;
        for (int j = 0; j < width; j++) {
            Cell cell;
            cell.row = i;
            cell.col = j;
            cell.roomRenderer = Renderer;
            cell.MakeRoomRect(height, width, GameArea_w, GameArea_w, GameArea_y, GameArea_x);
            mazeRow.push_back(cell);
        }
        maze.push_back(mazeRow);
    }
    cout << "Maze has been intialised\n";
}


void Maze::generateMaze() {
  // set random seed using time
  srand(time(0));

  // create a vector of pointers to Cells
  vector<Cell*> mazeStack;

  // create a vector of pointers to neighbours of cells
  vector<Cell*> neighbours;

  // choosing the initial cell, mark it as visited and push it to stack
  int row = 0;
  int col = 0;
  Cell* currentCell = &maze[row][col];
  currentCell->visited = true;
  mazeStack.push_back(&maze[row][col]);

  // initialising neighbouring cell pointers to something
  Cell* northCell = currentCell;
  Cell* eastCell = currentCell;
  Cell* southCell = currentCell;
  Cell* westCell = currentCell;
  Cell* randomNeighbour = currentCell;

  // while stack is not empty
  while (!mazeStack.empty()) {

    // pop a cell from the stack and make it the current cell
    row = mazeStack.back()->row;
    col = mazeStack.back()->col;
    currentCell = &maze[row][col];
    mazeStack.pop_back();

    // define and check if any neighbours have not been visited
    // also check for out of bound indexes
    if (row > 0 && !maze[row - 1][col].visited) {
      northCell = &maze[row - 1][col];
      neighbours.push_back(northCell);
    }
    if (col < width - 1 && !maze[row][col + 1].visited) {
      eastCell = &maze[row][col + 1];
      neighbours.push_back(eastCell);
    }
    if (row < height - 1 && !maze[row + 1][col].visited) {
      southCell = &maze[row + 1][col];
      neighbours.push_back(southCell);
    }
    if (col > 0 && !maze[row][col - 1].visited) {
      westCell = &maze[row][col - 1];
      neighbours.push_back(westCell);
    }

    // check if any neighbours have not been visited
    if (!neighbours.empty()) {
      // push the current cell to the stack
      mazeStack.push_back(currentCell);
      // choose random unvisited neighbour
      randomNeighbour = neighbours[rand() % neighbours.size()];
      // push chosen cell as visited and push to stack
      randomNeighbour->visited = true;
      mazeStack.push_back(randomNeighbour);
      // remove wall between current cell and chosen cell

      if (randomNeighbour == northCell) {
        currentCell->northWall = false;
      }
      else if (randomNeighbour == eastCell) {
        currentCell->eastWall = false;
      }
      else if (randomNeighbour == southCell) {
        southCell->northWall = false;
      }
      else {
        westCell->eastWall = false;
      }
      neighbours.clear();

    }
  }
  add_walls();
  cout << "Maze has been generated!\n";
}


void Maze::add_walls(){
  for (int i=0;i<height;i++) {
    for (int j=0;j<width;j++) {
      if(j==0 && i!=height-1){
        if(!maze[i+1][j].northWall){
          maze[i][j].southWall=false;
        }
      }
      else if(i==height-1 && j!=0){
        if(!maze[i][j-1].eastWall){
          maze[i][j].westWall=false;
        }
      }
      else if(i!=height-1 || j!=0){
        if(!maze[i+1][j].northWall){
          maze[i][j].southWall=false;
        }
        if(!maze[i][j-1].eastWall){
          maze[i][j].westWall=false;
        }
      }
    }
  }
  return;
}


void Maze::displayMaze(SDL_Renderer* gRenderer, int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    SDL_Rect fillRect = { 0,0,0,0 };
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0) {
                if (j == 0) {
                    if (maze[i][j].eastWall && maze[i + 1][j].northWall) {
                        fillRect = { x,y,SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        fillRect = { x,y,SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        fillRect = { x,y,SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i][j].eastWall) {
                            fillRect = { x + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + 0,SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + 0,y + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
                else if (j == maze[0].size() - 1) {
                    if (maze[i][j - 1].eastWall && maze[i + 1][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)), y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
                else {
                    if (maze[i][j - 1].eastWall && maze[i][j].eastWall && maze[i + 1][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i][j].eastWall) {
                            fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
            }

            else if (i == maze.size() - 1) {
                if (j == 0) {
                    if (maze[i][j].northWall && maze[i][j].eastWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].eastWall) {
                            fillRect = { (x + j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                }
                else if (j == maze[0].size() - 1) {
                    if (maze[i][j - 1].eastWall && maze[i][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                }
                else {
                    if (maze[i][j - 1].eastWall && maze[i][j].northWall && maze[i][j].eastWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].eastWall) {
                            fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                }
            }
            else {
                if (j == 0) {
                    if (maze[i][j].northWall && maze[i][j].eastWall && maze[i + 1][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].eastWall) {
                            fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
                else if (j == maze[0].size() - 1) {
                    if (maze[i][j - 1].eastWall && maze[i][j].northWall && maze[i + 1][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
                else {
                    if (maze[i][j - 1].eastWall && maze[i][j].northWall && maze[i][j].eastWall && maze[i + 1][j].northWall) {
                        fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                        SDL_RenderFillRect(gRenderer, &fillRect);
                    }
                    else {
                        if (maze[i][j - 1].eastWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i][j].eastWall) {
                            fillRect = { x + (j * (SCREEN_WIDTH / width)) + (SCREEN_WIDTH / width) - (SCREEN_WIDTH / (width * 10)),y + i * (SCREEN_HEIGHT / height),SCREEN_WIDTH / (width * 10), SCREEN_HEIGHT / height };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                        if (maze[i + 1][j].northWall) {
                            fillRect = { x + j * (SCREEN_WIDTH / width),y + (i * (SCREEN_HEIGHT / height)) + (SCREEN_HEIGHT / height) - (SCREEN_HEIGHT / (height * 10)),SCREEN_WIDTH / width, SCREEN_HEIGHT / (height * 10) };
                            SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 0xFF);
                            SDL_RenderFillRect(gRenderer, &fillRect);
                        }
                    }
                }
            }
        }

    }
}

  
