#pragma once
#include <vector>

#include "Global.h"
#include "Scene.h"
#include "Timer.h"

class SceneManager {
private:
	std::vector<std::shared_ptr<Scene>> _scenes;
	std::shared_ptr<Scene> _currentScene = nullptr;
	std::shared_ptr<Scene> _nextScene = nullptr;
	bool _isTransitioning = false;
	int _transitionDuration;  // in milliseconds
	int _transitionDirection; // 0 = no transition, 1 = fade in, 2 = fade out
	int _transitionAlpha;     // 0 = transparent, 255 = opaque

public:
	SceneManager();
	~SceneManager();
	void addScene(std::shared_ptr<Scene> scene);
	void setCurrentScene(const std::string& name);
	void changeScene(const std::string& name);
	void draw(SDL_Renderer* renderer);
	void handleEvent(SDL_Event event);
	void update();
	void clear();
};

