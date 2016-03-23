#include <string>
#include <iostream>

using namespace std;

#include <vector>

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#endif
#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#endif
#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include <direct.h>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#define getcwd _getcwd

#endif

class Jewel
{
public:
	bool active;
	SDL_Texture*jewel;
	SDL_Rect jewelRect;

	float posJ_X,posJ_Y;

	Jewel(SDL_Renderer*renderer,string filePath,int color, float x, float y);

	void Draw(SDL_Renderer*renderer);

	void Update(SDL_Renderer*renderer);

	void TankMoveX(float tankSpeed,float deltaTime);
	void TankMoveY(float tankSpeed, float deltaTime);


};
