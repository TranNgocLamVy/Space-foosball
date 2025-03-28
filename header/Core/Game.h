#pragma once

#include "GLobal.h"
#include "SceneManager.h"

class Game {
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	bool _running = true;

public:
	Game();
	~Game() {};
	static void playMucic(const std::string& path);
	void start();
	static SceneManager* getSceneManager();
};

