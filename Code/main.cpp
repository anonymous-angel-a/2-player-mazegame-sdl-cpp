#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include "maze.h"
#include "Player.h"
using namespace std;

// screen dimensions
const int width = 1024;
const int height = 768;
int GameArea_Width = 640;
int GameArea_Height = 640;
int GameArea_xOffset = 0;
int GameArea_yOffset = (height - GameArea_Width);


// images, sound and text files
const char* frontscreenfilename = "res/images/castle.jpg";
const char* titletextfilename = "res/text/Beyond_Wonderland.ttf";
const char* textfilename = "res/text/MagicSchoolOne.ttf";
const char* soundfilename = "res/sounds/sparkle.wav";
const char* soundfilename2 = "res/sounds/key.wav";
const char* soundfilename3 = "res/sounds/error.wav";


SDL_Rect FullScreen{ 0, 0, width, height };

//for single player
SDL_Rect GameAreaFillRect{ GameArea_xOffset,
							GameArea_yOffset,
							GameArea_Width,
			GameArea_Height };

// for double player
SDL_Rect GameAreaFillRect1{ GameArea_xOffset,
							GameArea_yOffset,
							width/2-10,
			GameArea_Height-150 };

SDL_Rect GameAreaFillRect2{ GameArea_xOffset + width/2+10,
							GameArea_yOffset,
							width/2-10,
			GameArea_Height - 150 };



int GameTitle_Width = width;
int GameTitle_Height = height - GameArea_Height;
int GameTitle_xOffset = 0;
int GameTitle_yOffset = 0;
int GameTitle_FontSize = 72;

SDL_Rect GameTitleFillRect
{
	GameTitle_xOffset,
	GameTitle_yOffset,
	GameTitle_Width,
	GameTitle_Height
};

//Game Window
SDL_Window* gwindow = NULL;
SDL_Surface* gsurface = nullptr;
//Game Renderer
SDL_Renderer* renderer = NULL;
//sound effects
Mix_Chunk *sparkle= NULL;
Mix_Chunk *keysound =NULL;
Mix_Chunk *boom=NULL;



enum GameState{FrontMenu, About, Menu, Single, Double, GameOver, Win};
GameState curGameState;

//initialisation functions
bool loadaudio() {
	bool success = true;
	sparkle = Mix_LoadWAV("res/sounds/sparkle.wav");
	if (sparkle == NULL)
	{
		printf("Failed to load sparkle sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	keysound = Mix_LoadWAV("res/sounds/key.wav");
	if (sparkle == NULL)
	{
		printf("Failed to load key sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	boom = Mix_LoadWAV("res/sounds/boom.wav");
	if (sparkle == NULL)
	{
		printf("Failed to load boom sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

bool init()
{
	bool success = true;
	int imgFlags = IMG_INIT_PNG;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gwindow = SDL_CreateWindow("A Simple Maze Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (gwindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
			if (TTF_Init() == -1)
			{
				printf("SDL TTF could not initialize");
				success = false;
			} 
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				success = false;
			}
			else {
			
				gsurface = SDL_GetWindowSurface(gwindow);
				success = loadaudio();
			}
			
			
		}
	}
	return success;
}


//show full screen texture, returns true if player presses Esc key or closes the SDL window.
void DrawText(string text, SDL_Rect* rect, int size, const char* filename){
	TTF_Font* font = TTF_OpenFont(filename, size);
	if (font == NULL)
	{
		printf("SDL could not load the Font from ");
		printf(filename);
		return;
	}
	else {
		SDL_Color color = { 255, 255, 255, 255 }; //black
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_Rect DisplayRect = { rect->x+ (rect->w- surface->w)/2, rect->y + (rect->h - surface->h) / 2, surface->w, surface->h };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, rect);
		SDL_RenderCopy(renderer, texture, NULL, &DisplayRect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, rect);
		SDL_RenderPresent(renderer);
	}


}
void DrawTextwithwidth(string text, SDL_Rect* rect, int size, const char* filename, int w) {
	TTF_Font* font = TTF_OpenFont(filename, size);
	if (font == NULL)
	{
		printf("SDL could not load the Font from ");
		printf(filename);
		return;
	}
	else {
		SDL_Color color = { 255, 255, 255, 255 }; //black
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, w);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_Rect DisplayRect = { rect->x + (rect->w - surface->w) / 2, rect->y + (rect->h - surface->h) / 2, surface->w, surface->h };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, rect);
		SDL_RenderCopy(renderer, texture, NULL, &DisplayRect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, rect);
		SDL_RenderPresent(renderer);
	}
}

bool frontscreen() {
	bool success = false;
	//showing front screen image
	SDL_Texture* screenTexture = IMG_LoadTexture(renderer, frontscreenfilename);
	if (screenTexture == NULL)
	{
		printf("SDL could not load the front screen Texture from ");
		printf(frontscreenfilename);
		return true;
	}
	SDL_RenderCopy(renderer, screenTexture, NULL, &FullScreen);
	SDL_RenderPresent(renderer);
	SDL_Rect rect{ (width - 800 )/2, height - 100, 800,100 };
	DrawText("Press any key to continue, Press ESC to exit", &rect, 50, textfilename);
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	//responding to inputs from keyboard
	SDL_Event* startEvent = new SDL_Event;
	do {
		SDL_WaitEvent(startEvent);
	} while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT);

	if (startEvent->key.keysym.sym == SDLK_ESCAPE || startEvent->type == SDL_QUIT)
	{
		success = true;
	}
	return success;
}
void showscreen(int r, int g, int b) {  // show screen with a particular color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &FullScreen);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}
bool menuscreen() { // show menu screen
	bool success= false;
	showscreen(100, 100, 100);
	SDL_Rect rect1{ width / 2 - 175, (GameTitle_Height + height)/2 -250, 350,100 };
	SDL_Rect rect2{ width / 2 - 175, (GameTitle_Height + height) / 2 - 50, 350,100 };
	SDL_Rect rect3{ width / 2 - 175, (GameTitle_Height + height) / 2 + 150, 350,100 };
	//SDL_Rect rect4{ width / 2 - 250, height - GameArea_Height, 500, 75 };
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	//DrawText("Select an option", &rect4, 60, textfilename);
	DrawText("Play Single Player", &rect1, 60, textfilename);
	DrawText("Play Multi Player", &rect2, 60, textfilename);
	DrawText("Exit", &rect3, 60, textfilename);
	SDL_Event* startEvent = new SDL_Event;
	while (true) {
		SDL_WaitEvent(startEvent);
		if (startEvent->type == SDL_QUIT) {
			success = true; break;
		}
		if (startEvent->type == SDL_MOUSEBUTTONDOWN) {
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			if (mx<rect1.x || mx>(rect1.x + 350)) continue;
			if (my > rect1.y && my < (rect1.y + 100)) {
				curGameState = Single;
				break;
			} else if (my > rect2.y && my < (rect2.y + 100)){
				curGameState = Double;
			break;
		}
			else if (my > rect3.y && my < (rect3.y + 100)) { success = true; break; }
	}
}
	return success;
}
// shows game over screen
bool GameOverScreen() {

	bool success = false;
	showscreen(100, 100, 100);
	SDL_Rect rect1{ width / 2 - 250, (height - GameArea_Height + height - 100) / 2 -100 , 500, 200 };
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	DrawTextwithwidth("Game over!!", &rect1, 60, textfilename,400);
	SDL_Rect rect{ (width - 800) / 2, height - 100, 800,100 };
	DrawText("Press R to return to Menu, Press ESC to exit", &rect, 50, textfilename);
	SDL_Event* startEvent = new SDL_Event;
	do {
		SDL_WaitEvent(startEvent);
	} while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT);
	if (startEvent->key.keysym.sym == SDLK_r) {
		curGameState = Menu;
	}
	else if (startEvent->key.keysym.sym == SDLK_ESCAPE) {
		success = true;
	}
	return success;
}
bool AboutScreen() {
	bool success = false;
	showscreen(100, 100, 100);
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	SDL_Rect rect{ (width - 800) / 2, height - 100, 800,100 };
	SDL_Rect rect1{ width / 2 - 250, (height - GameArea_Height + height - 100) /2 -200 , 500, 400 };
	DrawText("Press any key to continue, Press ESC to exit", &rect, 50, textfilename);
	DrawTextwithwidth("The princess wants to reach her castle but there is a maze she has to cross. She has to find a key and the door to castle but the maze is filled with monsters and also diamonds. The princess loves diamonds and wants to collect as many as possible while avoiding the monsters. Help the princess reach her castle !!! ", &rect1, 33, textfilename,400);
	SDL_Event* startEvent = new SDL_Event;
	do {
		SDL_WaitEvent(startEvent);
	} while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT);

	if (startEvent->key.keysym.sym == SDLK_ESCAPE || startEvent->type == SDL_QUIT)
	{
		success = true;
	}
	return success;
}

void drawstatistics(int& score, int& steps, int& lives) {
	SDL_Rect rect1{ (GameArea_Width)+50, GameTitle_Height + 50, 200,100 };
	SDL_Rect rect2{ (GameArea_Width)+50, GameTitle_Height + 160, 200,100 };
	SDL_Rect rect3{ (GameArea_Width)+50, GameTitle_Height + 270, 200,100 };
	DrawText("Score: " + to_string(score), &rect1, 30, textfilename);
	DrawText("Steps: " + to_string(steps), &rect2, 30, textfilename);
	DrawText("Lives: " + to_string(lives), &rect3, 30, textfilename);
}
//for single player	
void GameScreen(shared_ptr<Maze> maze, std::shared_ptr<Player> player, int& score, int& steps, int& lives) {
	showscreen(100, 100, 100);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &GameAreaFillRect);
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	SDL_Rect rect{ (GameArea_Width) + 50, GameTitle_Height + 380, 200,100 };
	SDL_Rect rect1{ (GameArea_Width) + 50, GameTitle_Height + 490, 200,100 };
	DrawText("R to reset", &rect, 30, textfilename);
	DrawText("Esc to Menu", &rect1, 30, textfilename);
	drawstatistics(score, steps, lives);
	maze->displayMaze(renderer, 0, GameTitle_Height, GameArea_Width, GameArea_Height);
	player->AddObjToRenderer();
	maze->mazeDoorPtr->AddObjToRenderer();
	for (int i = 0; i < maze->mazeDiamondPtrs.size(); i++) {
		if (maze->mazeDiamondPtrs[i]->consumed == false) { maze->mazeDiamondPtrs[i]->AddObjToRenderer(); }
	}
	for (int i = 0; i < maze->mazeGuardPtrs.size(); i++) {
		maze->mazeGuardPtrs[i]->AddObjToRenderer();
	}
	if (!player->hasKey) {
		maze->mazeKeyPtr->AddObjToRenderer();
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}
//for double player
void GameScreenDouble(shared_ptr<Maze> maze1, shared_ptr<Maze> maze2, std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, int lives1, int score1, int lives2, int score2) {
	showscreen(100, 100, 100);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &GameAreaFillRect1);
	SDL_RenderFillRect(renderer, &GameAreaFillRect2);
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	SDL_Rect rect{ (width - 800) / 2, height - 50, 800,50 };
	DrawText("Press R to reset, Press ESC to exit", &rect, 50, textfilename);
	SDL_Rect rect1{ GameAreaFillRect1.x,GameAreaFillRect1.y + GameAreaFillRect1.h+10, width/2-10, 70};
	DrawText("Player 1: Lives= "+ to_string(lives1) + ",score= " + to_string(score1), &rect1, 50, textfilename);
	SDL_Rect rect2{ GameAreaFillRect2.x,GameAreaFillRect2.y + GameAreaFillRect2.h + 10, width / 2 - 10, 70 };
	DrawText("Player 2: Lives= " + to_string(lives2) + ",score= " + to_string(score2), &rect2, 50, textfilename);


	maze1->displayMaze(renderer, GameAreaFillRect1.x, GameAreaFillRect1.y, GameAreaFillRect1.w, GameAreaFillRect1.h);
	maze1->mazeKeyPtr->AddObjToRenderer();
	for (int i = 0; i < maze1->mazeDiamondPtrs.size(); i++) {
		if (maze1->mazeDiamondPtrs[i]->consumed == false) { maze1->mazeDiamondPtrs[i]->AddObjToRenderer(); }
	}
	for (int i = 0; i < maze1->mazeGuardPtrs.size(); i++) {
		maze1->mazeGuardPtrs[i]->AddObjToRenderer();
	}
	maze1->mazeDoorPtr->AddObjToRenderer();
	player1->AddObjToRenderer();

	maze2->displayMaze(renderer, GameAreaFillRect2.x, GameAreaFillRect2.y, GameAreaFillRect2.w, GameAreaFillRect2.h);
	maze2->mazeKeyPtr->AddObjToRenderer();
	for (int i = 0; i < maze2->mazeDiamondPtrs.size(); i++) {
		if (maze2->mazeDiamondPtrs[i]->consumed == false) { maze2->mazeDiamondPtrs[i]->AddObjToRenderer(); }
	}
	for (int i = 0; i < maze2->mazeGuardPtrs.size(); i++) {
		maze2->mazeGuardPtrs[i]->AddObjToRenderer();
	}
	maze2->mazeDoorPtr->AddObjToRenderer();
	player2->AddObjToRenderer();

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}
bool winscreen() {
	bool success = false;
	showscreen(100, 100, 100);
	SDL_Rect rect1{ width / 2 - 250, (height - GameArea_Height + height - 100) / 2 - 100 , 500, 200 };
	DrawText("The Enchanted Castle", &GameTitleFillRect, 100, titletextfilename);
	DrawTextwithwidth("Congratulations!! The princess has finally reached her castle, thanks to you.", &rect1, 50, textfilename,400);
	SDL_Rect rect{ (width - 800) / 2, height - 100, 800,100 };
	DrawText("Press R to return to Menu, Press ESC to exit", &rect, 50, textfilename);
	SDL_Event* startEvent = new SDL_Event;
	do {
		SDL_WaitEvent(startEvent);
	} while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT);
	if (startEvent->key.keysym.sym == SDLK_r) {
		curGameState = Menu;
	}
	else if (startEvent->key.keysym.sym == SDLK_ESCAPE) {
		success = true;
	}
	return success;
}
int main(int argc, char** args) {

	if (!init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}

	curGameState = FrontMenu;
	bool exit = false;
	SDL_Event* gameEvent = new SDL_Event;
	while (!exit && gameEvent->type != SDL_QUIT) {
		if (curGameState == FrontMenu)
		{
			exit = frontscreen();
			if (exit) break;
			curGameState = About;
		}
		if (curGameState == About) {
			exit = AboutScreen();
			if (exit) break;
			curGameState = Menu;
		}
		if (curGameState == Menu)
		{
			exit = menuscreen();
			if (exit) break;
		}
		if (curGameState == Win) {
			exit = winscreen();
			if (exit) break;
		}
		if (curGameState == GameOver) {
			exit = GameOverScreen();
			if (exit) break;
		}
		if (curGameState == Single) {
			int steps = 0;
			int score = 0;
			int lives = 10;
			std::shared_ptr<Maze> maze = std::shared_ptr<Maze>(new Maze());
			maze->initialiseMaze(renderer);
			maze->generateMaze();
			maze->CreateKey();
			maze->CreateDoor();
			maze->CreateDiamonds();
			maze->CreateGuards();
			std::shared_ptr<Player> curPlayer = std::shared_ptr<Player>(new Player(maze->FindRoomByPos(maze->startPos_x, maze->startPos_y), renderer));
			GameScreen(maze, curPlayer, score, steps, lives);
			while (!exit && gameEvent->type != SDL_QUIT && curGameState == Single) {
				SDL_WaitEvent(gameEvent);
				if (gameEvent->type == SDL_KEYDOWN) {

					if (gameEvent->key.keysym.sym == SDLK_ESCAPE) {
						curGameState = Menu;
						break;
					}

					if (gameEvent->key.keysym.sym == SDLK_r && curPlayer->playerLives > 1) {
						curPlayer->playerLives--;
						lives--;
						curPlayer->SetPlayerToStart(maze->FindRoomByPos(maze->startPos_x, maze->startPos_y));
					}
					else if (gameEvent->key.keysym.sym == SDLK_DOWN ||
						gameEvent->key.keysym.sym == SDLK_LEFT ||
						gameEvent->key.keysym.sym == SDLK_RIGHT ||
						gameEvent->key.keysym.sym == SDLK_UP) {
						maze->NextMazeCycle();
						int Pos_y = curPlayer->objPos_y;
						int Pos_x = curPlayer->objPos_x;
						switch (gameEvent->key.keysym.sym)
						{
						case SDLK_UP:
							if (curPlayer->curObjRoom->northWall == false)
							{
								Pos_x--;

							}
							break;

						case SDLK_RIGHT:
							if (curPlayer->curObjRoom->eastWall == false)
							{
								Pos_y++;

							}
							break;

						case SDLK_DOWN:
							if (curPlayer->curObjRoom->southWall == false)
							{
								Pos_x++;
							}
							break;

						case SDLK_LEFT:
							if (curPlayer->curObjRoom->westWall == false)
							{
								Pos_y--;
							}
							break;

						default:
							std::cout << "Not a valid move!" << std::endl;
							return false;
							break;
						}

						if (Pos_x == curPlayer->objPos_x && curPlayer->objPos_y == Pos_y) {
							std::cout << "Not a valid move" << endl;
							continue;
						}
						else {
							steps++;
							std::shared_ptr<Cell> room = maze->FindRoomByPos(Pos_x, Pos_y);
							if (Pos_x != maze->finalPos_x || Pos_y != maze->finalPos_y) { curPlayer->SetObjRoom(room); }
							//check for key
							if (maze->mazeKeyPtr->objPos_x == Pos_x && maze->mazeKeyPtr->objPos_y == Pos_y) {
								curPlayer->hasKey = true;
								Mix_PlayChannel(-1, keysound, 0);
							}
							//check for door
							if (Pos_x == maze->finalPos_x && Pos_y == maze->finalPos_y && curPlayer->hasKey) {
								curGameState = Win;
								break;
							}
							//check for diamonds
							for (int i = 0; i < maze->mazeDiamondPtrs.size(); i++) {
								if (maze->mazeDiamondPtrs[i]->objPos_x == Pos_x && maze->mazeDiamondPtrs[i]->objPos_y == Pos_y && maze->mazeDiamondPtrs[i]->consumed == false) {
									maze->mazeDiamondPtrs[i]->consumed = true;
									Mix_PlayChannel(-1, sparkle, 0);
									score += 100;
								}
							}
							//check for guards
							for (int i = 0; i < maze->mazeGuardPtrs.size(); i++) {
								if (maze->mazeGuardPtrs[i]->objPos_x == Pos_x && maze->mazeGuardPtrs[i]->objPos_y == Pos_y) {
									Mix_PlayChannel(-1, boom, 0);
									if (curPlayer->playerLives == 1) {
										curGameState = GameOver;
										break;
									}
									curPlayer->playerLives--;
									lives--;
									curPlayer->SetPlayerToStart(maze->FindRoomByPos(maze->startPos_x, maze->startPos_y));
									break;
								}
							}
						}
					}
					GameScreen(maze, curPlayer, score, steps, lives);

				}

			}
		}
		if (curGameState == Double) {
			int score1 = 0;
			int score2 = 0;
			int lives1 = 10;
			int lives2 = 10;
			bool player1 = true;
			std::shared_ptr<Maze> maze1 = std::shared_ptr<Maze>(new Maze());
			maze1->initialiseMaze(renderer, GameAreaFillRect1.w, GameAreaFillRect1.h, GameAreaFillRect1.y, GameAreaFillRect1.x);
			maze1->generateMaze();
			maze1->CreateKey();
			maze1->CreateDoor();
			maze1->CreateDiamonds();
			maze1->CreateGuards();
			std::shared_ptr<Player> curPlayer1 = std::shared_ptr<Player>(new Player(maze1->FindRoomByPos(maze1->startPos_x, maze1->startPos_y), renderer));

			std::shared_ptr<Maze> maze2 = std::shared_ptr<Maze>(new Maze());
			maze2->initialiseMaze(renderer, GameAreaFillRect2.w, GameAreaFillRect2.h, GameAreaFillRect2.y, GameAreaFillRect2.x);
			maze2->generateMaze();
			maze2->CreateKey();
			maze2->CreateDoor();
			maze2->CreateDiamonds();
			maze2->CreateGuards();
			std::shared_ptr<Player> curPlayer2 = std::shared_ptr<Player>(new Player(maze2->FindRoomByPos(maze1->startPos_x, maze2->startPos_y), renderer));

			GameScreenDouble(maze1, maze2, curPlayer1, curPlayer2, lives1, score1, lives2, score2);
			while (!exit && gameEvent->type != SDL_QUIT && curGameState == Double) {
				SDL_WaitEvent(gameEvent);
				if (gameEvent->type == SDL_KEYDOWN) {
					int x = 0;


					if (gameEvent->key.keysym.sym == SDLK_ESCAPE) {
						curGameState = Menu;
						break;
					}
					if (player1) {
						player1 = false;
						if (gameEvent->key.keysym.sym == SDLK_r && curPlayer1->playerLives > 1) {
							curPlayer1->playerLives--;
							lives1--;
							curPlayer1->SetPlayerToStart(maze1->FindRoomByPos(maze1->startPos_x, maze1->startPos_y));
						}
						else if (gameEvent->key.keysym.sym == SDLK_DOWN ||
							gameEvent->key.keysym.sym == SDLK_LEFT ||
							gameEvent->key.keysym.sym == SDLK_RIGHT ||
							gameEvent->key.keysym.sym == SDLK_UP) {
							maze1->NextMazeCycle();
							int Pos_y = curPlayer1->objPos_y;
							int Pos_x = curPlayer1->objPos_x;
							switch (gameEvent->key.keysym.sym)
							{
							case SDLK_UP:
								if (curPlayer1->curObjRoom->northWall == false)
								{
									Pos_x--;

								}
								break;

							case SDLK_RIGHT:
								if (curPlayer1->curObjRoom->eastWall == false)
								{
									Pos_y++;

								}
								break;

							case SDLK_DOWN:
								if (curPlayer1->curObjRoom->southWall == false)
								{
									Pos_x++;
								}
								break;

							case SDLK_LEFT:
								if (curPlayer1->curObjRoom->westWall == false)
								{
									Pos_y--;
								}
								break;

							default:
								std::cout << "Not a valid move!" << std::endl;
								return false;
								break;
							}

							if (Pos_x == curPlayer1->objPos_x && curPlayer1->objPos_y == Pos_y) {
								std::cout << "Not a valid move" << endl;
								continue;
							}
							else {
								std::shared_ptr<Cell> room = maze1->FindRoomByPos(Pos_x, Pos_y);
								if (Pos_x != maze1->finalPos_x || Pos_y != maze1->finalPos_y) { curPlayer1->SetObjRoom(room); }
								//check for key
								if (maze1->mazeKeyPtr->objPos_x == Pos_x && maze1->mazeKeyPtr->objPos_y == Pos_y) {
									curPlayer1->hasKey = true;
									Mix_PlayChannel(-1, keysound, 0);
								}
								//check for door
								if (Pos_x == maze1->finalPos_x && Pos_y == maze1->finalPos_y && curPlayer1->hasKey) {
									curGameState = Win;
									break;
								}
								//check for diamonds
								for (int i = 0; i < maze1->mazeDiamondPtrs.size(); i++) {
									if (maze1->mazeDiamondPtrs[i]->objPos_x == Pos_x && maze1->mazeDiamondPtrs[i]->objPos_y == Pos_y && maze1->mazeDiamondPtrs[i]->consumed == false) {
										maze1->mazeDiamondPtrs[i]->consumed = true;
										Mix_PlayChannel(-1, sparkle, 0);
										score1 += 100;
									}
								}
								//check for guards
								for (int i = 0; i < maze1->mazeGuardPtrs.size(); i++) {
									if (maze1->mazeGuardPtrs[i]->objPos_x == Pos_x && maze1->mazeGuardPtrs[i]->objPos_y == Pos_y) {
										Mix_PlayChannel(-1, boom, 0);
										if (curPlayer1->playerLives == 1) {
											curGameState = GameOver;
											break;
										}
										curPlayer1->playerLives--;
										lives1--;
										curPlayer1->SetPlayerToStart(maze1->FindRoomByPos(maze1->startPos_x, maze1->startPos_y));
										break;
									}
								}

							}
						}
					}
						else {
							player1 = true;
							if (gameEvent->key.keysym.sym == SDLK_r && curPlayer2->playerLives > 1) {
								curPlayer2->playerLives--;
								lives2--;
								curPlayer2->SetPlayerToStart(maze2->FindRoomByPos(maze2->startPos_x, maze2->startPos_y));
							}
							else if (gameEvent->key.keysym.sym == SDLK_DOWN ||
								gameEvent->key.keysym.sym == SDLK_LEFT ||
								gameEvent->key.keysym.sym == SDLK_RIGHT ||
								gameEvent->key.keysym.sym == SDLK_UP) {
								maze2->NextMazeCycle();
								int Pos_y = curPlayer2->objPos_y;
								int Pos_x = curPlayer2->objPos_x;
								switch (gameEvent->key.keysym.sym)
								{
								case SDLK_UP:
									if (curPlayer2->curObjRoom->northWall == false)
									{
										Pos_x--;

									}
									break;

								case SDLK_RIGHT:
									if (curPlayer2->curObjRoom->eastWall == false)
									{
										Pos_y++;

									}
									break;

								case SDLK_DOWN:
									if (curPlayer2->curObjRoom->southWall == false)
									{
										Pos_x++;
									}
									break;

								case SDLK_LEFT:
									if (curPlayer2->curObjRoom->westWall == false)
									{
										Pos_y--;
									}
									break;

								default:
									std::cout << "Not a valid move!" << std::endl;
									return false;
									break;
								}

								if (Pos_x == curPlayer2->objPos_x && curPlayer2->objPos_y == Pos_y) {
									std::cout << "Not a valid move" << endl;
									continue;
								}
								else {
									std::shared_ptr<Cell> room = maze2->FindRoomByPos(Pos_x, Pos_y);
									if (Pos_x != maze2->finalPos_x || Pos_y != maze2->finalPos_y) { curPlayer2->SetObjRoom(room); }
									//check for key
									if (maze2->mazeKeyPtr->objPos_x == Pos_x && maze2->mazeKeyPtr->objPos_y == Pos_y) {
										curPlayer2->hasKey = true;
										Mix_PlayChannel(-1, keysound, 0);
									}
									//check for door
									if (Pos_x == maze2->finalPos_x && Pos_y == maze2->finalPos_y && curPlayer2->hasKey) {
										curGameState = Win;
										break;
									}
									//check for diamonds
									for (int i = 0; i < maze2->mazeDiamondPtrs.size(); i++) {
										if (maze2->mazeDiamondPtrs[i]->objPos_x == Pos_x && maze2->mazeDiamondPtrs[i]->objPos_y == Pos_y && maze2->mazeDiamondPtrs[i]->consumed == false) {
											maze2->mazeDiamondPtrs[i]->consumed = true;
											Mix_PlayChannel(-1, sparkle, 0);
											score2 += 100;
										}
									}
									//check for guards
									for (int i = 0; i < maze2->mazeGuardPtrs.size(); i++) {
										if (maze2->mazeGuardPtrs[i]->objPos_x == Pos_x && maze2->mazeGuardPtrs[i]->objPos_y == Pos_y) {
											Mix_PlayChannel(-1, boom, 0);
											if (curPlayer2->playerLives == 1) {
												curGameState = GameOver;
												break;
											}
											curPlayer2->playerLives--;
											lives2--;
											curPlayer2->SetPlayerToStart(maze2->FindRoomByPos(maze2->startPos_x, maze2->startPos_y));
											break;
										}
									}

								}
							}
						}
					GameScreenDouble(maze1, maze2, curPlayer1, curPlayer2, lives1, score1, lives2, score2);

				}


			}

		}
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gwindow);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	// End the program
	return 0;
}
