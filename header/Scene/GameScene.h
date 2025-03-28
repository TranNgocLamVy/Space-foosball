#pragma once
#include "Global.h"

#include "Scene.h"
#include "SpriteObject.h"
#include "Player.h"
#include "Asteroid.h"
#include "GameManager.h"

class GameScene : public Scene {
private:
	SDL_Texture* _background;
	SDL_Renderer* _renderer;
	std::vector<std::unique_ptr<UI>> _uis;
	std::vector<std::unique_ptr<UI>> _pauseMenuUIs;
	std::vector<std::unique_ptr<UI>> _endMenuUIs;
	std::vector<std::unique_ptr<SpriteObject>> _spriteObjects;
	std::vector<std::unique_ptr<SpriteObject>> _walls;
	std::unique_ptr<Player> _player1;
	std::unique_ptr<Player> _player2;
	std::unique_ptr<Asteroid> _asteroid;
	Mix_Chunk* _ambientSound;
	GameManager* _gameManager;

public:
	GameScene(SDL_Renderer* renderer,const std::string& name);
	~GameScene() override;
	void startScene() override;
	void endScene() override;
	void handleEvent(SDL_Event event) override;
	void draw(SDL_Renderer* renderer) override;
	void update() override;
	void addUI(UI* ui);
	void addPauseMenuUI(UI* ui);
	void addEndMenuUI(UI* ui);
	void addSpriteObject(SpriteObject* spriteObject);
	void addWall(SpriteObject* wall);
	static GameManager* getGameManager();
	void restart();
};