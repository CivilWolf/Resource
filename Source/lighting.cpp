#include "lighting.h"

Color::Color()
{
r = 0;
g = 0;
b = 0;
a = 255;
}



	LightEmitter::LightEmitter(float numRays, float rad)
	{
		center.x = 0;
		center.y = 0;
		color.r = 0;
		color.g = 0;
		color.b = 0;
		radius = rad;
		numrays = numRays;


	}

	void LightEmitter::Update(SDL_Point newcenter)
	{
		center = newcenter;
	}
	void LightEmitter::Draw(SDL_Renderer*renderer)
	{
		float angle = 0;
		SDL_Point point;
		SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
		for(int i = 0; i < numrays; i++)
		{
			point.x = center.x + radius*cos( angle );
			point.y = -center.y + radius*sin( angle );
			SDL_RenderDrawLine(renderer,center.x,center.y,point.x,point.y);
			angle++;
		}
	}

