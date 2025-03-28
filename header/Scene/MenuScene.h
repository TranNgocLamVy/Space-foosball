#pragma once
#include "Global.h"

#include "Scene.h"
#include "Button.h"
#include "Text.h"
#include "Game.h"
#include "Sprite.h"
#include "GIF.h"
#include "SpriteObject.h"

/*Declare MenuScene class*/
class MenuScene : public Scene {
private:
	SDL_Texture* _background;
	SDL_Renderer* _renderer;
	std::vector<std::unique_ptr<UI>> _uis;
	std::vector<std::unique_ptr<SpriteObject>> _spriteObjects;


public:
	MenuScene(SDL_Renderer* renderer,const std::string& name);
	~MenuScene() override;
	void startScene() override;
	void endScene() override;
	void handleEvent(SDL_Event event) override;
	void draw(SDL_Renderer* renderer) override;
	void update() override;
	void addUI(UI* ui);
	void addSpriteObject(SpriteObject* spriteObject);
	void addAsteroid(const std::string& path, float scale, Vector2 position, Vector2 velocity, int rotationSpeed);
};
