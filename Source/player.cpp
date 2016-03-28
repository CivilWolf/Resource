#include "player.h"

//analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer*renderer,string filePath,string audioPath,float x,float y)
{
	back = IMG_LoadTexture(renderer, (filePath + "health1.png").c_str());
	mid = IMG_LoadTexture(renderer, (filePath + "health2.png").c_str());
	front = IMG_LoadTexture(renderer, (filePath + "health3.png").c_str());


	backR.x = midR.x = frontR.x = 10;
	backR.y = midR.y = frontR.y = 50;
	backR.w = midR.w = frontR.w = 239;
	backR.h = midR.h = frontR.h = 32;


	playerHealth = 100.0f;
	maxHealth = 100.0f;


	active = true;
	speed = 500.0f;

	arrow = Mix_LoadWAV((audioPath+"laser.wav").c_str());


	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 6;

	TTF_Init();
	//TTF_RenderText_Solid(font, tempLives.c_str(), color);
	font = TTF_OpenFont((audioPath+"Berry Rotunda.ttf").c_str(),40);
	scorePos.x = 600; scorePos.y = 40;
		livesPos.x = 10;
		livesPos.y = 40;
	
	//update score method
	UpdateScore(renderer);
	UpdateLives(renderer,filePath);

		if(playerLives == 6)
		playerPath = filePath +"Ship0.png";
		else if (playerLives == 5)
			playerPath = filePath + "Ship1.png";
		else if (playerLives == 4)
			playerPath = filePath + "Ship2.png";
		else if (playerLives == 3)
			playerPath = filePath + "Ship3.png";
		else if (playerLives == 2)
			playerPath = filePath + "Ship4.png";
		else if (playerLives == 1)
			playerPath = filePath + "Ship5.png";
	surface = IMG_Load(playerPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
	posRect.x = x;
	posRect.y = y;
	int w,h;
	SDL_QueryTexture(texture,NULL,NULL,&w,&h);
	posRect.w = w;
	posRect.h = h;
	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;


	xDirOld = 1;
	yDirOld = 0;

	center.x = posRect.w / 2;
	center.y = posRect.h / 2;

	string bulletPath;
	bulletPath = filePath + "bullet.png";
	
	for (int i = 0; i < 10; i++)
	{
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000,0,0);
		bulletList.push_back(tmpBullet);

	}

}

void Player::Reset()
{

		posRect.x = 250.0;
		posRect.y = 500.0;
	

	pos_X = posRect.x;
	pos_Y = posRect.y;
	playerLives = 6;
	playerHealth = 100.0f;
	playerScore = 0;
	xDir = 0;
	yDir = 0;
	active = true;
}


void Player::UpdateLives(SDL_Renderer*renderer, string filePath)
{
	string Result;
	ostringstream convert;
	convert << playerLives;
	Result = convert.str();

	tempLives = "Player Lives: ";// +Result;

		livesSurface = TTF_RenderText_Solid(font,tempLives.c_str(),color);

		if (playerLives == 6)
			playerPath = filePath + "Ship0.png";
		else if (playerLives == 5)
			playerPath = filePath + "Ship1.png";
		else if (playerLives == 4)
			playerPath = filePath + "Ship2.png";
		else if (playerLives == 3)
			playerPath = filePath + "Ship3.png";
		else if (playerLives == 2)
			playerPath = filePath + "Ship4.png";
		else if (playerLives == 1)
			playerPath = filePath + "Ship5.png";

	
	livesTexture = SDL_CreateTextureFromSurface(renderer,livesSurface);
	SDL_QueryTexture(livesTexture,NULL,NULL,&livesPos.w,&livesPos.h);
	SDL_FreeSurface(livesSurface);


	surface = IMG_Load(playerPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	//SDL_QueryTexture(texture, NULL, NULL, &w, &h);


	oldLives = playerLives;

	if(playerLives <= 0)
	{
		active = false;
		posRect.x = posRect.y = -2000;
		pos_X = pos_Y = -2000;
	}




}

void Player::UpdateScore(SDL_Renderer*renderer)
{
	string Result;
	ostringstream convert;
	convert << playerScore;
	Result = convert.str();

	tempScore = "Player Score:" +Result;

		scoreSurface = TTF_RenderText_Solid(font,tempScore.c_str(),color);

	scoreTexture = SDL_CreateTextureFromSurface(renderer,scoreSurface);
	SDL_QueryTexture(scoreTexture,NULL,NULL,&scorePos.w,&scorePos.h);
	SDL_FreeSurface(scoreSurface);
	oldScore = playerScore;
}


void Player::Update(float deltaTime,SDL_Renderer *renderer, string filePath)
{
	pos_X +=(speed * xDir)*deltaTime;
	pos_Y +=(speed * yDir) *deltaTime;
	posRect.x = (int)(pos_X+0.5f);
	posRect.y = (int)(pos_Y+0.5f);





	if (xDir != 0 || yDir != 0)
	{
		x = posRect.x - xDir;
		y = posRect.y - yDir;
		playerangle = atan2(xDir, -yDir) * 180 / 3.14;
		oldAngle = playerangle;
		xDirOld = xDir;
		yDirOld = yDir;
	}
	else
	{
		playerangle = oldAngle;
	}


	if(posRect.x < 0)
	{
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x >1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		pos_X=posRect.x;
	}
	if(posRect.y < 0)
	{
		posRect.y = 0;
		pos_Y = posRect.y;
	}
	if(posRect.y >1024 - posRect.h)
	{
		posRect.y = 1024 -posRect.h;
		pos_Y = posRect.y;
	}


	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Update(deltaTime);
		}
	}

	if(playerScore != oldScore)
	{
		UpdateScore(renderer);
	}
	if(playerLives != oldLives)
		{
			UpdateLives(renderer,filePath);
		}
}



void Player::eTankHit()
{
	playerHealth -= .025f;
	midR.w = playerHealth / maxHealth * 239;

}

void Player::eBulletHit()
{
	playerHealth -= 5;
	midR.w = playerHealth / maxHealth * 239;

}



void Player::Draw(SDL_Renderer*renderer)
{
	SDL_RenderCopyEx(renderer,texture,NULL,&posRect, playerangle, &center, SDL_FLIP_NONE);
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active)
		{
			bulletList[i].Draw(renderer);
		}
	}
	SDL_RenderCopy(renderer,scoreTexture,NULL,&scorePos);
	SDL_RenderCopy(renderer,livesTexture,NULL,&livesPos);


	SDL_RenderCopy(renderer, back, NULL, &backR);
	SDL_RenderCopy(renderer, mid, NULL, &midR);
	SDL_RenderCopy(renderer, front, NULL, &frontR);
}


void Player::CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			Mix_PlayChannel(-1,arrow,0);
			/*bulletList[i].active = true;
			bulletList[i].posRect.x = (pos_X + (posRect.w / 2));
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w / 2));
			bulletList[i].posRect.y = posRect.y;
			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_Y = pos_Y;*/

			bulletList[i].active = true;

			bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
			bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));
			bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
			bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2);
			bulletList[i].pos_X = bulletList[i].posRect.x;
			bulletList[i].pos_Y = bulletList[i].posRect.y;

			if (xDir != 0 || yDir != 0)
			{
				bulletList[i].xDir = xDirOld;
				bulletList[i].yDir = yDirOld;
			}
			else
			{
				bulletList[i].xDir = xDirOld;
				bulletList[i].yDir = yDirOld;
			}
			break;
		}
	}
}


void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0)
	{
		if(event.button == 0)
		{
			CreateBullet();

		}
	}
}
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0)
	{
		if(event.axis == 0)
		{
			if(event.value <-JOYSTICK_DEAD_ZONE)
			{
				xDir=-1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}
		if(event.axis == 1)
		{
			if(event.value <-JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}
			else
			{
				yDir = 0.0f;
			}
		}
	}
}

void Player::OnKeyboardPress(const SDL_KeyboardEvent event)
{
	if(event.type == SDL_KEYDOWN)
	{
		if (event.keysym.sym == SDLK_w)
		{
			yDir = -1.0f;
		}
		if (event.keysym.sym == SDLK_s)
		{
			yDir = 1.0f;
		}
		else
		{
			xDir = 0.0f;
			yDir = 0.0f;
		}
	}
}
Player::~Player()
{
	SDL_DestroyTexture(texture);
}




