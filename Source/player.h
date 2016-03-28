#if defined(__APPLE__)

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>


#endif
#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#endif
#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#endif
#if defined(__linux__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>

#include <sstream>
//#include <string>
using namespace std;
#include <vector>
#include "bullet.h"

class Player{
public:


	SDL_Texture *back, *mid, *front;

	SDL_Rect backR, midR, frontR;
	float playerHealth,maxHealth;

	void eTankHit();

	void eBulletHit();




	bool active;

	int playerScore,oldScore,playerLives,oldLives;

	//Variable for font
	TTF_Font *font;
	SDL_Color color = {255,255,255,255};
	//SDL_Color colorP2 = {255,255,255,255};
	SDL_Surface *scoreSurface,*livesSurface;
	SDL_Texture *scoreTexture,*livesTexture;
	SDL_Rect scorePos,livesPos;
	string tempScore,tempLives;

	SDL_Point center;



	Mix_Chunk *arrow;

	vector<Bullet> bulletList;

	string playerPath;
	SDL_Surface*surface;
	SDL_Texture*texture;
	SDL_Rect posRect;
	float xDir, xDirOld;
	float yDir, yDirOld;
	float speed;
	float pos_X,pos_Y;


	float x, y, playerangle;

	float oldAngle;

	Player(SDL_Renderer*renderer,string filePath, string audioPath,float x,float y);

	void Update(float deltaTime,SDL_Renderer *renderer, string filePath);
	void Draw(SDL_Renderer*renderer);
	void OnControllerAxis(const SDL_ControllerAxisEvent event);
	void OnControllerButton(const SDL_ControllerButtonEvent event);
	void OnKeyboardPress(const SDL_KeyboardEvent event);
	~Player();

	//update score method
	void UpdateScore(SDL_Renderer*renderer);
	void UpdateLives(SDL_Renderer*renderer,string filePath);

	void Reset();

private:
	void CreateBullet();

};
