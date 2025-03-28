#include "Game.h"

#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Timer.h"
#include "MenuScene.h"
#include "GameScene.h"

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Space Foosball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Failed to initialize Mixer: " << SDL_GetError() << std::endl;
	}
	Mix_AllocateChannels(NUMBER_OF_SOUND_CHANNEL);

	if (TTF_Init() == -1) {
		std::cout << "Failed to initialize TTF: " << SDL_GetError() << std::endl;
	}

	getSceneManager()->addScene(std::make_shared<MenuScene>(_renderer, "Menu"));
	getSceneManager()->addScene(std::make_shared<GameScene>(_renderer, "Game"));
	getSceneManager()->setCurrentScene("Menu");
}

void Game::playMucic(const std::string& path) {
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	if (music == NULL) {
		printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
		Mix_CloseAudio();
		return;
	}
	if (Mix_PlayMusic(music, -1) == -1) {
		printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
		Mix_FreeMusic(music);
		Mix_CloseAudio();
		SDL_Quit();
		return;
	}
}

void Game::start() {
	playMucic("assets/musics/Space_theme2.mp3");

	timer.init();
	timer.setFramerate(MAX_FPS);

	while (_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				_running = false;
				Mix_CloseAudio();
				SDL_DestroyRenderer(_renderer);
				SDL_DestroyWindow(_window);
				SDL_Quit();
				return;
			}
			getSceneManager()->handleEvent(event);
		}
		getSceneManager()->update();
		getSceneManager()->draw(_renderer);

		SDL_RenderPresent(_renderer);
		timer.update();
	}

	Mix_CloseAudio();
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

SceneManager* Game::getSceneManager() {
	static SceneManager sceneManager;
	return &sceneManager;
}
