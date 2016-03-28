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

class Color
{
public:
	float r,g,b,a;
	Color();
};

class LightRay
{
public:

};


class LightEmitter
{
public:
	float radius;
	float numrays;
	SDL_Point center;
	Color color;

	LightEmitter(float numRays,float rad);
	void Update(SDL_Point newcenter);
	void Draw(SDL_Renderer*renderer);
};
