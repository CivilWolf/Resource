// Example program:
// Using SDL2 to create an application window

#if defined(__APPLE__)

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "SDL2_mixer/SDL_mixer.h"

#endif
#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#endif
#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include <direct.h>
#include "SDL_image.h"
#include "SDL_mixer.h"
#define getcwd _getcwd

#endif
#if defined(__linux__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>

#include "enemyTank.h"
#include "jewel.h"

//#include "player.h"
//#include <string>
using namespace std;

int bSpeed = 100;

SDL_Rect bkgd1Pos, bkgd2Pos;

//set temp variables to hold movement
float BG1pos_X = 0, BG1pos_Y = 0;

//set temp variables to hold movement
float BG2pos_X = 0, BG2pos_Y = -1024;

//code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//move the background
void UpdateBackground(float deltaTime) {
	//Update
	//Update Background 1
	BG1pos_Y += (bSpeed * 1) * deltaTime;
	//Set the new bkgd1 position
	bkgd1Pos.y = (int) (BG1pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if (bkgd1Pos.y >= 1024) {
		bkgd1Pos.y = -1024;
		BG1pos_Y = bkgd1Pos.y;
	}

	//Update Background 2
	BG2pos_Y += (bSpeed * 1) * deltaTime;
	//Set the new bkgd1 position
	bkgd2Pos.y = (int) (BG2pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if (bkgd2Pos.y >= 1024) {
		bkgd2Pos.y = -1024;
		BG2pos_Y = bkgd2Pos.y;
	}
}
//new Joystick vars

//analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;
//joystick direction vars
float xDir, yDir;
//CURSOR FLOAT VARS FOR MOVEMENT
float pos_X, pos_Y;
SDL_Rect cursorPos, activePos;
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {
	//check joystick 0 - firstplayer
	if (event.which == 0) {
		//check X axis
		if (event.axis == 0) {
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;

			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			}
			else {
				xDir = 0.0f;
			}
		}
		if (event.axis == 1) {
			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;

			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			}
			else {
				yDir = 0.0f;
			}
		}
	}
}
void UpdateCursor(float deltaTime) {
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	//assign SDL_Rect X and Y
	cursorPos.x = (int) (pos_X + 0.5f);
	cursorPos.y = (int) (pos_Y + 0.5f);
	//update active position of cursor - collision box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	//off the screen in X
	if (cursorPos.x < 0) {
		cursorPos.x = 0;
		pos_X = cursorPos.x;

	}
	if (cursorPos.x > 1024 - cursorPos.w) {
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;

	}
	//off the screen in Y
	if (cursorPos.y < 0) {
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}
	if (cursorPos.y > 1024 - cursorPos.h) {
		cursorPos.y = 1024 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;


#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "explode.h"

vector<Enemy> enemyList;
vector<Explode> explodeList;


void MakeExplosion(int x, int y)
{
	for (int i = 0; i < explodeList.size(); i++)
	{
		if (explodeList[i].active == false)
		{
			explodeList[i].active = true;
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;
			break;
		}
	}
}






int main(int argc, char* argv[]) {

	srand(time(NULL));

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow("An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			1024,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

#if defined(__APPLE__)
	{
		cout << "Running on Apple" << endl;
		//get the current working directory
		string currentWorkingDirectory(getcwd(NULL, 0));

		//create a string linking to the mac's images folder
		string images_dir = currentWorkingDirectory + "/Resources/Images/";
	}
#endif

#if defined(__linux__)
	{
		cout << "Running on Linux" << endl;
		//get the current working directory
		string currentWorkingDirectory(getcwd(NULL, 0));

		//create a string linking to the mac's images folder
		string images_dir = currentWorkingDirectory + "/Resources/Images/";
		string audio_dir = currentWorkingDirectory + "/Resources/Audio/";
	}
#endif

#if defined(_WIN32) || (_WIN64)
	{
		cout << "Running on Windows" << endl;
		//get the current working directory
		string currentWorkingDirectory(getcwd(NULL, 0));

		//create a string linking to the mac's images folder
		string images_dir = currentWorkingDirectory + "\\Resources\\Images\\";
		string audio_dir = currentWorkingDirectory + "\\Resources\\Audio\\";
	}
#endif

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		//quit = true;
		return 1;
	}

	SDL_Renderer*renderer = NULL;

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	string currentWorkingDirectory(getcwd(NULL, 0));
	string images_dir = currentWorkingDirectory + "/Resources/Images/";
	string audio_dir = currentWorkingDirectory + "/Resources/Audio/";
	//CreateBackground
	string BKGDpath = images_dir + "Back.png";




	//create a SDL surface to hold the background images
	SDL_Surface *surface = IMG_Load((BKGDpath).c_str());

	//create SDL_Texture
	SDL_Texture*bkgd1;
	//place surface info into the texture bkgd1;
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	//create SDL_Texture
	SDL_Texture*bkgd2;
	//place surface info into the texture bkgd1;
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	//free SDL surface
	SDL_FreeSurface(surface);

	//Create the SDL_Rectangle for the texture's position and size -x,y,w,h

	//set the X,T,W, and H for the Rectangle
	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 1024;

	//Create the SDL_Rectangle for the texture's position and size -x,y,w,h

	//set the X,T,W, and H for the Rectangle
	bkgd2Pos.x = 0;
	bkgd2Pos.y = -768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 1024;

	//**************************************Create Background -End*************************************

	//*************************************Create Cursor -Start**************************************
	////create a SDL surface to hold the background images
	//surface = IMG_Load((images_dir + "Cursor.png").c_str());

	////create SDL_Texture
	//SDL_Texture*cursor;
	////place surface info into the texture bkgd1;
	//cursor = SDL_CreateTextureFromSurface(renderer, surface);

	////free SDL surface
	//SDL_FreeSurface(surface);

	////Create the SDL_Rectangle for the texture's position and size -x,y,w,h
	////SDL_Rect cursorPos, activePos;
	////set the X,T,W, and H for the Rectangle
	//cursorPos.x = 10;
	//cursorPos.y = 10;
	//cursorPos.w = 64;
	//cursorPos.h = 64;

	//activePos.x = 10;
	//activePos.y = 10;
	//activePos.w = 10;
	//activePos.h = 10;

	//int cursorSpeed = 400;
	//*************************************Create Cursor -End**************************************

	
	//*************************************Create Instructions Menu -End**************************************

	////Create the SDL_Rectangle for the texture's position and size -x,y,w,h
	//	SDL_Rect cursorPos;
	//	//set the X,T,W, and H for the Rectangle
	//	cursorPos.x = 0;
	//	cursorPos.y = 0;
	//	cursorPos.w = 64;
	//	cursorPos.h = 64;

	////create cursor
	//string CURSORpath = images_dir + "/Cursor.png";

	//surface = IMG_Load(CURSORpath.c_str());
	//SDL_Texture*cursor;
	//cursor= SDL_CreateTextureFromSurface(renderer,surface);

	//int bSpeed = 100;

	/*
	 //////////////////////////////////////////////
	 //The surface contained by the window
	 SDL_Surface* screenSurface = NULL;

	 //Get window surface
	 screenSurface = SDL_GetWindowSurface(window);

	 //Fill the surface white
	 SDL_FillRect(screenSurface, NULL,
	 SDL_MapRGB(screenSurface->format, 0, 42, 254));

	 //Update the surface
	 SDL_UpdateWindowSurface(window);
	 */


	SDL_GameControllerEventState(SDL_ENABLE);
	//Setup a Game Controller Variable
	SDL_GameController* gGameController0 = NULL;

	//Open Game Controller
	gGameController0 = SDL_GameControllerOpen(0);

	SDL_GameController* gGameController1 = NULL;

	//Open Game Controller
	gGameController1 = SDL_GameControllerOpen(0);
	//Turn on Game Controller Events
	

	//SDL Event to handle input
	SDL_Event event;

	//setup variables for the game states
	enum GameState {
		MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE
	};

	//setup the initial state
	GameState gameState = PLAYERS1;

	//boolean values to control movement though the states
	bool menu = false, instructions = false, players1 = false, players2 = false, win = false, lose = false, quit = false;

	//////////////////////////////////////////////



	//Open Audio Channel
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	Mix_Music *bgm = Mix_LoadMUS((audio_dir+ "ambience.wav").c_str());
	if(!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm,-1);
	}

	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "Slop.wav").c_str());

	bool alreadyOver = false;

	//create players
	Player player1 =Player(renderer,images_dir.c_str(),audio_dir.c_str(),250.0,500.0);

	for (int i = 0; i < 20; i++)
	{
		Explode tmpExplode(renderer, images_dir, -1000, -1000);
		explodeList.push_back(tmpExplode);
	}


	EnemyTank eTank1 = EnemyTank(renderer, images_dir.c_str(),audio_dir.c_str(),200.0f,100.0f);
	EnemyTank eTank2 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 100.0f);
	EnemyTank eTank3 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 900.0f, 900.0f);
	EnemyTank eTank4 = EnemyTank(renderer, images_dir.c_str(), audio_dir.c_str(), 200.0f, 900.0f);






	//jewel Hud

	SDL_Texture*Jewelsbkgd = IMG_LoadTexture(renderer,(images_dir +"Jewels/"+ "jewelsBKGD.png").c_str());
	SDL_Rect JewelsbkgdRect;
	JewelsbkgdRect.x = 370;
	JewelsbkgdRect.y = 10;
	JewelsbkgdRect.w = 284;
	JewelsbkgdRect.h = 91;

	SDL_Texture*purple = IMG_LoadTexture(renderer,(images_dir+"Jewels/"+ "jewelsPurple.png").c_str());
	SDL_Rect purplePos;
	purplePos.x = 370;
	purplePos.y = 10;
	purplePos.w = 284;
	purplePos.h = 91;

	SDL_Texture *red = IMG_LoadTexture(renderer,(images_dir+"Jewels/"+"jewelsRed.png").c_str());
	SDL_Rect redPos;
	redPos.x = 370;
	redPos.y = 10;
	redPos.w = 284;
	redPos.h = 91;

	SDL_Texture*blue = IMG_LoadTexture(renderer,(images_dir+"Jewels/"+"jewelsBlue.png").c_str());

	SDL_Rect bluePos;
	bluePos.x = 370;
	bluePos.y = 10;
	bluePos.w = 284;
	bluePos.h = 91;

	bool havePurple = false;
	bool haveRed = false;
	bool haveBlue = false;

	Jewel purpleJewel = Jewel(renderer,images_dir.c_str(),0,200.0f,200.0f);
	Jewel redJewel = Jewel(renderer,images_dir.c_str(),1,500.0f,200.0f);
	Jewel blueJewel = Jewel(renderer,images_dir.c_str(),2,200.0f,500.0f);


	Jewel folder1 = Jewel(renderer,images_dir.c_str(),3,600.0f,350.0f);
	Jewel folder2 = Jewel(renderer,images_dir.c_str(),3,1200.0f,600.0f);
	Jewel folder3 = Jewel(renderer,images_dir.c_str(),3,550.0f,1000.0f);
	Jewel folder4 = Jewel(renderer,images_dir.c_str(),3,1400.0f,1200.0f);


	SDL_Texture*folderB = IMG_LoadTexture(renderer,(images_dir + "FolderStuff/" + "folderBKGD.png").c_str());
	SDL_Texture*folderM = IMG_LoadTexture(renderer,(images_dir + "FolderStuff/" + "movingBar.png").c_str());
	SDL_Texture*folderF = IMG_LoadTexture(renderer,(images_dir + "FolderStuff/" + "folderFront.png").c_str());


SDL_Rect folderRect;
folderRect.x = 310;
folderRect.y = 110;
folderRect.w = 368;
folderRect.h = 65;

SDL_Rect movingRect;
movingRect.x = 401;
movingRect.y = 125;
movingRect.w = 272;
movingRect.h = 27;

float currentFolders = 0.0f;
float maxFolders = 100.0f;


	// The window is open: could enter program loop here (see SDL_PollEvent())
	while (!quit) {
		switch (gameState) {

		
		case PLAYERS1: {

			enemyList.clear();

			//reset the player
			player1.Reset();

			players1 = true;

			for(int i = 0; i< 6;i++)
			{
				Enemy tmpEnemy(renderer,images_dir);
				enemyList.push_back(tmpEnemy);
			}
			cout << "The Game State is Players 1" << endl;
			cout << endl;
			while (players1) {

				//set up framerate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if (SDL_PollEvent(&event)) {
					//check to see if the SDL Window is closed - player clicks X in the window
					if (event.type == SDL_QUIT) {
						quit = true;
						players1 = false;
						break;
					}
					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_X) {
								players1 = false;
								gameState = WIN;

							}
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_Y) {
								players1 = false;
								gameState = LOSE;

							}
							//send button press info to player 1
							if(player1.active)
							{
								player1.OnControllerButton(event.cbutton);
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						player1.OnControllerAxis(event.caxis);
						break;
					}
				}

				UpdateBackground(deltaTime);
				if(player1.active)
				{
				player1.Update(deltaTime,renderer, images_dir.c_str());
				}



				////move background
				//if ((player1.posRect.x >= 1024 - player1.posRect.w))// &&(player1.Xvalue > 8000))
				//{
				//	X_pos -= (tank1.speed) * deltaTime;

				//	if((bkgdRect.x > -1024))
				//	{
				//		bkgdRect.x = 
				//	}
				//}









				eTank1.Update(deltaTime, player1.posRect);
				eTank2.Update(deltaTime, player1.posRect);
				eTank3.Update(deltaTime, player1.posRect);
				eTank4.Update(deltaTime, player1.posRect);




				for(int i  = 0; i < enemyList.size();i++)
				{
					enemyList[i].Update(deltaTime);
				}


				if(player1.active == true)
				{
					for(int i = 0; i< player1.bulletList.size();i++)
					{
						if(player1.bulletList[i].active == true)
						{

							if (SDL_HasIntersection(&eTank1.eTankRect, &player1.bulletList[i].posRect))
							{
								player1.bulletList[i].Reset();
								if (eTank1.active == true)
								{
									eTank1.RemoveHealth();
								}
								break;
							}
							if (SDL_HasIntersection(&eTank2.eTankRect, &player1.bulletList[i].posRect))
							{
								player1.bulletList[i].Reset();
								if (eTank2.active == true)
								{
									eTank2.RemoveHealth();
								}
								break;
							}
							if (SDL_HasIntersection(&eTank3.eTankRect, &player1.bulletList[i].posRect))
							{
								player1.bulletList[i].Reset();
								if (eTank3.active == true)
								{
									eTank3.RemoveHealth();
								}
								break;
							}
							if (SDL_HasIntersection(&eTank4.eTankRect, &player1.bulletList[i].posRect))
							{
								player1.bulletList[i].Reset();
								if (eTank4.active == true)
								{
									eTank4.RemoveHealth();
								}
								break;
							}

							for(int j = 0; j < enemyList.size();j++)
							{
								if(SDL_HasIntersection(&player1.bulletList[i].posRect,&enemyList[j].posRect))
								{
									Mix_PlayChannel(-1,explosionSound,0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									enemyList[j].Reset();
									enemyList[j].CreateMini();
									player1.bulletList[i].Reset();
									player1.playerScore += 50;
									if (player1.playerScore >= 1000)
									{
										players1 = false;
										gameState = PLAYERS1;
									}
									
								}
							}
						}
					}
					for(int i = 0; i < enemyList.size(); i++)
					{
						if(SDL_HasIntersection(&player1.posRect,&enemyList[i].posRect))
						{
							Mix_PlayChannel(-1,explosionSound,0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							enemyList[i].Reset();
							//enemyList[i].CreateMini();
							//player1.playerLives -=1;
							player1.eBulletHit();
							if(player1.playerLives <= 0)
							//if (player1.playerHealth <= 0)
							{
								players1 = false;
								gameState = PLAYERS1;
								break;
							}
						}
					}
				}
				//player 1 active check ends

				for (int i = 0; i < explodeList.size(); i++)
				{
					if (explodeList[i].active == true)
					{
						explodeList[i].Update(deltaTime);
					}
				}




				if (SDL_HasIntersection(&player1.posRect, &eTank1.eTankRect))
				{
					player1.eTankHit();
				}
				if (SDL_HasIntersection(&player1.posRect, &eTank2.eTankRect))
				{
					player1.eTankHit();
				}
				if (SDL_HasIntersection(&player1.posRect, &eTank3.eTankRect))
				{
					player1.eTankHit();
				}
				if (SDL_HasIntersection(&player1.posRect, &eTank4.eTankRect))
				{
					player1.eTankHit();
				}



				if(SDL_HasIntersection(&player1.posRect,&purpleJewel.jewelRect))
				{
					havePurple = true;
					purpleJewel.active = false;
					purpleJewel.jewelRect.x = -5000;
					purpleJewel.jewelRect.y = -5000;
				}
				if(SDL_HasIntersection(&player1.posRect,&redJewel.jewelRect))
				{
					haveRed = true;
					redJewel.active = false;
					redJewel.jewelRect.x = -5000;
					redJewel.jewelRect.y = -5000;
				}
				if(SDL_HasIntersection(&player1.posRect,&blueJewel.jewelRect))
				{
					haveBlue = true;
					blueJewel.active = false;
					blueJewel.jewelRect.x = -5000;
					blueJewel.jewelRect.y = -5000;
				}


				if(SDL_HasIntersection(&player1.posRect, &folder1.jewelRect))
				{
					folder1.active = false;
					folder1.jewelRect.x = -5000;
					folder1.jewelRect.y = -5000;
					currentFolders += 100/4;

				}

				if(SDL_HasIntersection(&player1.posRect, &folder2.jewelRect))
				{
					folder2.active = false;
					folder2.jewelRect.x = -5000;
					folder2.jewelRect.y = -5000;
					currentFolders += 100/4;

				}
				if(SDL_HasIntersection(&player1.posRect, &folder3.jewelRect))
				{
					folder3.active = false;
					folder3.jewelRect.x = -5000;
					folder3.jewelRect.y = -5000;
					currentFolders += 100/4;

				}
				if(SDL_HasIntersection(&player1.posRect, &folder4.jewelRect))
				{
					folder4.active = false;
					folder4.jewelRect.x = -5000;
					folder4.jewelRect.y = -5000;
					currentFolders += 100/4;

				}

				movingRect.w = currentFolders/maxFolders*272;

				//Start Drawing
				//Clear SDL Renderer
				SDL_RenderClear(renderer);



				//Draw the b image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				//Draw the b image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);


				SDL_RenderCopy(renderer,Jewelsbkgd,NULL,&JewelsbkgdRect);

				if(haveRed)
				SDL_RenderCopy(renderer,red,NULL,&redPos);
				if(haveBlue)
				SDL_RenderCopy(renderer,blue,NULL,&bluePos);
				if(havePurple)
				SDL_RenderCopy(renderer,purple,NULL,&purplePos);

				SDL_RenderCopy(renderer,folderB,NULL,&folderRect);
				SDL_RenderCopy(renderer,folderM,NULL,&movingRect);
				SDL_RenderCopy(renderer,folderF,NULL,&folderRect);

				if(purpleJewel.active)
				{
					purpleJewel.Draw(renderer);
				}

				if(redJewel.active)
				{
					redJewel.Draw(renderer);
				}
				if(blueJewel.active)
				{
					blueJewel.Draw(renderer);
				}


				if(folder1.active)
				{
					folder1.Draw(renderer);
				}
				if(folder2.active)
				{
					folder2.Draw(renderer);
				}
				if(folder3.active)
				{
					folder3.Draw(renderer);
				}
				if(folder4.active)
				{
					folder4.Draw(renderer);
				}


				for(int i = 0; i < enemyList.size();i++)
				{
					enemyList[i].Draw(renderer);
				}



				//Draw the menu image
				//SDL_RenderCopy(renderer, oneN, NULL, &onePos);

				player1.Draw(renderer);

				eTank1.Draw(renderer);
				eTank2.Draw(renderer);
				eTank3.Draw(renderer);
				eTank4.Draw(renderer);

				for (int i = 0; i < explodeList.size(); i++)
				{
					if (explodeList[i].active == true)
					{
						explodeList[i].Draw(renderer);
					}
				}

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
		}
			break;	//end players1 case

		}
	}

	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}

