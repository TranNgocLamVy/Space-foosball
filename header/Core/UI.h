#pragma once
#include "Global.h"

class UI {
public:
	UI() {};
	~UI() {};

	virtual void handleEvent(SDL_Event event) = 0;
	virtual void draw(SDL_Renderer* renderer) = 0;
	virtual void update() = 0;
};