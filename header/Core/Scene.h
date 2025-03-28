#pragma once
#include "Global.h"

class Scene {
private:
	std::string _name;

public:
	Scene(const std::string& name) : _name(name) {}
	virtual ~Scene() {};

	std::string getName() {
		return _name;
	}

	virtual void startScene() = 0;
	virtual void endScene() = 0;
	virtual void handleEvent(SDL_Event event) = 0;
	virtual void draw(SDL_Renderer* renderer) = 0;
	virtual void update() = 0;
};
