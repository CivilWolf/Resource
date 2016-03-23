#include "enemy.h"
Enemy::Enemy(SDL_Renderer*renderer,string filePath)
{
	string enemyPath = filePath +"Ebig.png";
	SDL_Surface *surface = IMG_Load(enemyPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
	int w, h;
	SDL_QueryTexture(texture,NULL,NULL,&w,&h);
	posRect.w = w;
	posRect.h = h;
	Reset();
	xDir = 0;
	yDir = 1;
	angle = rand()%(360);

	center.x = posRect.w/2;
	center.y = posRect.h/2;


	direction = rand()% 4;




	string miniPath;
	miniPath = filePath + "Esmall.png";
	for (int i = 0; i < 10; i++)
	{
		SmallEnemy tmpBullet(renderer, miniPath, -1000, -1000);
		miniList.push_back(tmpBullet);

	}
}


void Enemy::Reset()
{
	if (direction == 0)
	{
		speed = rand() % (5) + 1;
		speed *= 100;
		posRect.x = rand() % (1024 - posRect.w) + 1;
		posRect.y = 1200 - posRect.h;

		pos_X = posRect.x;
		pos_Y = posRect.y;
	}

	else if (direction == 1)
	{
		speed = rand() % (5) + 1;
		speed *= 100;

		posRect.x = 0 - posRect.w;
		posRect.y = rand() % (1024 - posRect.w) + 1;

		pos_X = posRect.x;
		pos_Y = posRect.y;
	}

	else if (direction == 2)
	{
		speed = rand() % (5) + 1;
		speed *= 100;
		posRect.x = rand() % (1024 - posRect.w) + 1;
		posRect.y = 0 - posRect.h;

		pos_X = posRect.x;
		pos_Y = posRect.y;
	}

	else if (direction == 3)
	{
		speed = rand() % (5) + 1;
		speed *= 100;
		posRect.x = 1024 - posRect.w;
		posRect.y =  rand() % (1024 - posRect.h) + 1;

		pos_X = posRect.x;
		pos_Y = posRect.y;
	}



}
void Enemy::Update(float deltaTime)
{


	if (direction == 0)
	{
		pos_Y -= (speed*yDir)*deltaTime;
		posRect.y = (int)(pos_Y - 0.5f);
		if (posRect.y < 0)
		{
			direction = rand() % 4;
			Reset();
		}
		angle += 0.1;
	}

	else if (direction == 1)
	{
		pos_X += (speed*yDir)*deltaTime;
		posRect.x = (int)(pos_X + 0.5f);
		if (posRect.x < 0)
		{
			direction = rand() % 4;
			Reset();
		}
		angle += 0.1;
	}


	else if (direction == 2)
	{
		pos_Y += (speed*yDir)*deltaTime;
		posRect.y = (int)(pos_Y + 0.5f);
		if (posRect.y > 900)
		{
			direction = rand() % 4;
			Reset();
		}
		angle += 0.1;
	}

	else if (direction == 3)
	{
		pos_X -= (speed*yDir)*deltaTime;
		posRect.x = (int)(pos_X + 0.5f);
		if (posRect.x < 0)
		{
			direction = rand() % 4;
			Reset();
		}
		angle += 0.1;
	}


}



void Enemy::CreateMini()
{
	for (int i = 0; i < miniList.size(); i++)
	{
		if (miniList[i].active == false)
		{
			//Mix_PlayChannel(-1, arrow, 0);
			miniList[i].active = true;
			miniList[i].posRect.x = (pos_X + (posRect.w / 2));
			miniList[i].posRect.x = (miniList[i].posRect.x - (miniList[i].posRect.w / 2));
			miniList[i].posRect.y = posRect.y;
			miniList[i].pos_X = pos_X;
			miniList[i].pos_Y = pos_Y;
			break;
		}
	}
}


void Enemy::Draw(SDL_Renderer*renderer)
{
	SDL_RenderCopyEx(renderer,texture,NULL,&posRect,angle,&center,SDL_FLIP_NONE);
}

Enemy::~Enemy()
{
	//SDL_DestroyTexture(texture)
}
