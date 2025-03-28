#pragma once
#include <SDL_image.h>

#include "Global.h"

class GIF {
protected:
	IMG_Animation* _animation;
    int _currentFrame;
	int _delay;
    float _scale;
    float _speed;
	int _avarageDelay;
public:
    GIF(SDL_Renderer* renderer, std::string path, float scale, float speed);
	~GIF();
    void draw(SDL_Renderer* renderer, Vector2 position);

	void setSpeed(float speed);
	float getSpeed();
	void setScale(float scale);
	float getScale();
	int getWidth();
	int getHeight();
};
