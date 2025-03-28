#include "GameScene.h"

#include "Game.h"
#include "Sprite.h"
#include "GIF.h"
#include "SpaceShip.h"
#include "Button.h"
#include "Text.h"

GameScene::GameScene(SDL_Renderer* renderer, const std::string& name) : Scene(name), _renderer(renderer) {
	_gameManager = getGameManager();

	_background = loadBackground(_renderer, std::string("assets/backgrounds/Space_Background.png"));

	addSpriteObject(new SpriteObject(_renderer, new Sprite(_renderer, "assets/sprites/Ship/Main_ship1.png", 1), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT / 2), [](SpriteObject* self) {}));
	addSpriteObject(new SpriteObject(_renderer, new Sprite(_renderer, "assets/sprites/Ship/Main_ship2.png", 1), Vector2(0, WINDOW_HEIGHT / 2), [](SpriteObject* self) {}));

	
	Sprite* wall1 = new Sprite(_renderer, "assets/sprites/Background/Wall.png", 1);
	addWall(new SpriteObject(_renderer, wall1, Vector2(WINDOW_WIDTH / 2, BORDER_WIDTH - 8), [](SpriteObject* self) {}));

	Sprite* wall2 = new Sprite(_renderer, "assets/sprites/Background/Wall.png", 1);
	wall1->setAngle(180);
	addWall(new SpriteObject(_renderer, wall2, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT - BORDER_WIDTH + 8), [](SpriteObject* self) {}));

	addUI(new Button(_renderer, "assets/sprites/UI/Pause.png", Vector2(40, 42), 50, 50, [this]() { _gameManager->pause(); }));

	addPauseMenuUI(new Button(_renderer, "RESUME", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 75), 400, 100, [this]() { _gameManager->resume(); }));
	addPauseMenuUI(new Button(_renderer, "RESTART", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50), 400, 100, [this]() { restart(); }));
	addPauseMenuUI(new Button(_renderer, "TO MENU", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 175), 400, 100, []() { Game::getSceneManager()->changeScene("Menu"); }));

	addEndMenuUI(new Button(_renderer, "PLAY AGAIN", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 75), 400, 100, [this]() { restart(); }));
	addEndMenuUI(new Button(_renderer, "TO MENU", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50), 400, 100, []() { Game::getSceneManager()->changeScene("Menu"); }));

	int distance = 240;
	std::vector<Vector2> player1ShipPositions = {
		Vector2(150.0f, WINDOW_HEIGHT * 0.5),
		Vector2(150.0f + distance * 1, WINDOW_HEIGHT * 0.5),
		Vector2(150.0f + distance * 3, WINDOW_HEIGHT * 0.5),
	}; 
	SDL_Color blue = { 58, 167, 222, 255 };
	_player1 = std::make_unique<Player>(_renderer, "assets/sprites/Ship/Ship3.png", "P1", player1ShipPositions, "assets/sprites/UI/Indicator1.png", blue,  SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_SPACE);

	std::vector<Vector2> player2ShipPositions = {
		Vector2(150.0f + distance * 2, WINDOW_HEIGHT / 2),
		Vector2(150.0f + distance * 4, WINDOW_HEIGHT / 2),
		Vector2(150.0f + distance * 5, WINDOW_HEIGHT / 2),
	};
	SDL_Color red = { 221, 58, 58, 255 };
	_player2 = std::make_unique<Player>(_renderer, "assets/sprites/Ship/Ship2.png", "P2", player2ShipPositions, "assets/sprites/UI/Indicator2.png", red, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RSHIFT);
	_player2->setCurrentSpaceShip(2);

	_asteroid = std::make_unique<Asteroid>(_renderer);

	_ambientSound = Mix_LoadWAV("assets/sfxs/Ambient.mp3");
	Mix_VolumeChunk(_ambientSound, 128 / 5);
}

GameScene::~GameScene() {
	_uis.clear();
	_spriteObjects.clear();
	
	_background = nullptr;
}

void GameScene::startScene() {
	Mix_PlayChannel(-1, _ambientSound, -1);
	restart();
}

void GameScene::endScene() {
	int num_channels = Mix_AllocateChannels(-1);
	for (int i = 0; i < num_channels; i++) {
		if (Mix_Playing(i) && Mix_GetChunk(i) == _ambientSound) {
			Mix_HaltChannel(i);
		}
	}
}

void GameScene::handleEvent(SDL_Event event) {
	if (_gameManager->isPaused()) {
		for (int i = 0; i < _pauseMenuUIs.size(); i++) {
			_pauseMenuUIs[i]->handleEvent(event);
		}
		return;
	} else if (_gameManager->isEnded()) {
		for (int i = 0; i < _endMenuUIs.size(); i++) {
			_endMenuUIs[i]->handleEvent(event);
		}
		return;
	}

	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->handleEvent(event);
	}
	
	_player1->handleEvent(event);
	_player2->handleEvent(event);
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_SPACE && !_gameManager->isPlayer1Ready()) {
			_gameManager->setPlayer1Ready(true);
		} else if (event.key.keysym.sym == SDLK_RSHIFT && !_gameManager->isPlayer2Ready()) {
			_gameManager->setPlayer2Ready(true);
		}
		if (_gameManager->isPlayer1Ready() && _gameManager->isPlayer2Ready()) {
			_gameManager->start();
		}
	}
	
}

void GameScene::update() {
	if (_gameManager->isPaused()) {
		for (int i = 0; i < _pauseMenuUIs.size(); i++) {
			_pauseMenuUIs[i]->update();
		}
		return;
	} else if (_gameManager->isEnded()) {
		for (int i = 0; i < _endMenuUIs.size(); i++) {
			_endMenuUIs[i]->update();
		}
		return;
	}
	_gameManager->update();
	for (int i = 0; i < _spriteObjects.size(); i++) {
		_spriteObjects[i]->update();
	}
	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->update();
	}

	_player1->update();
	_player2->update();
	std::vector<SDL_Rect> rects;
	for (int i = 0; i < _walls.size(); i++) {
		rects.push_back(_walls[i]->getRect());
	}
	std::vector<std::shared_ptr<SpaceShip>> player1Ships = _player1->getSpaceShips();
	std::vector<std::shared_ptr<SpaceShip>> player2Ships = _player2->getSpaceShips();
	std::vector<std::shared_ptr<SpaceShip>> spaceShips;
	spaceShips.insert(spaceShips.end(), player1Ships.begin(), player1Ships.end());
	spaceShips.insert(spaceShips.end(), player2Ships.begin(), player2Ships.end());
	_asteroid->update(rects, spaceShips);
}

void GameScene::draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, _background, NULL, NULL);

	_asteroid->draw(renderer);

	for (int i = 0; i < _spriteObjects.size(); i++) {
		_spriteObjects[i]->draw(renderer);
	}

	for (int i = 0; i < _walls.size(); i++) {
		_walls[i]->draw(renderer);
	}

	_player1->draw(renderer);
	_player2->draw(renderer);

	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->draw(renderer);
	}

	_gameManager->draw(renderer);

	if (_gameManager->isEnded()) {
		SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
		SDL_RenderFillRect(renderer, &rect);
		TTF_Font* font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 80);
		SDL_Color color = { 255, 255, 255, 255 };
		std::string text = "";
		int winner = _gameManager->getWinner();
		if (winner == 1) {
			text = "PLAYER 1 WINS";
			color = { 58, 167, 222, 255 };
		}
		else if (winner == 2) {
			text = "PLAYER 2 WINS";
			color = { 221, 58, 58, 255 };
		}
		else {
			text = "DRAW";
			color = { 255, 255, 255, 255 };
		}
		Text* pausedText = new Text(renderer, font, text, color, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 200));
		pausedText->draw(renderer);
		delete pausedText;
		TTF_CloseFont(font);

		for (int i = 0; i < _endMenuUIs.size(); i++) {
			_endMenuUIs[i]->draw(renderer);
		}
	}

	if (_gameManager->isPaused()) {
		SDL_Rect rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
		SDL_RenderFillRect(renderer, &rect);
		TTF_Font* font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 80);
		SDL_Color color = { 255, 255, 255, 255 };
		std::string text = "GAME PAUSED";
		Text* pausedText = new Text(renderer, font, text, color, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 200));
		pausedText->draw(renderer);
		delete pausedText;
		TTF_CloseFont(font);

		for (int i = 0; i < _pauseMenuUIs.size(); i++) {
			_pauseMenuUIs[i]->draw(renderer);
		}
	}
}

void GameScene::addUI(UI* ui) {
	_uis.push_back(std::unique_ptr<UI>(ui));
}

void GameScene::addPauseMenuUI(UI* ui) {
	_pauseMenuUIs.push_back(std::unique_ptr<UI>(ui));
}

void GameScene::addEndMenuUI(UI* ui) {
	_endMenuUIs.push_back(std::unique_ptr<UI>(ui));
}

void GameScene::addSpriteObject(SpriteObject* spriteObject) {
	_spriteObjects.push_back(std::unique_ptr<SpriteObject>(spriteObject));
}

void GameScene::addWall(SpriteObject* wall) {
	_walls.push_back(std::unique_ptr<SpriteObject>(wall));
}

GameManager* GameScene::getGameManager() {
	static GameManager gameManager;
	return &gameManager;
}

void GameScene::restart() {
	_gameManager->restart();
	_player1->restart();
	_player2->restart();
	_asteroid->restart();
}