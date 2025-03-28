#pragma once
#include "Global.h"

class Sprite {
protected:
	SDL_Texture* _texture;
	float _scale;
	int _width;
	int _height;
	Uint8 _alpha;
	float _angle;

public:
	Sprite(SDL_Renderer* renderer,const std::string& path, float scale);
	~Sprite();
	virtual void draw(SDL_Renderer* renderer, Vector2 position);
	void setScale(float scale);
	float getScale();
	void setAngle(float rotation);
	float getAngle();
	void setAlpha(Uint8 alpha);
	Uint8 getAlpha();
	int getWidth();
	int getHeight();

	SDL_Rect getRect();
};

