#include "MenuScene.h"

#include <random>
#include <functional>

#include "Vector2.h"
#include "Function.h"

MenuScene::MenuScene(SDL_Renderer* renderer, const std::string& name) : Scene(name), _renderer(renderer) {
	_background = loadBackground(_renderer, std::string("assets/backgrounds/Space_Background.png"));

	TTF_Font* normalFont = TTF_OpenFont("assets/fonts/Stepalange.ttf", 40);
	addUI(new Text(_renderer, normalFont, "TEAM GOOD GAME", WHITE, Vector2(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 30)));

	TTF_Font* bigFont = TTF_OpenFont("assets/fonts/Stepalange.ttf", 120);
	addUI(new Text(_renderer, bigFont, "SPACE FOOSBALL", WHITE, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 150)));

	addUI(new Button(_renderer, "PLAY GAME", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 400, 100, []() { Game::getSceneManager()->changeScene("Game"); }));
	addUI(new Button(_renderer, "QUIT", 60, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 150), 400, 100, []() { exit(0); }));

	addSpriteObject(new SpriteObject(_renderer, new GIF(_renderer, "assets/gifs/Planet.gif", 3, 0.75), Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), [](SpriteObject* self) {}));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randomX(-100, WINDOW_WIDTH + 100);
	std::uniform_int_distribution<int> randomY(0, WINDOW_HEIGHT);
	std::uniform_int_distribution<int> randomSpeed(20, 30);
	std::uniform_int_distribution<int> randomRotationSpeed(10, 20);
	std::uniform_int_distribution<int> randomSize(15, 25);
	for (int i = 1; i <= 14; i++) {
		std::string path = "assets/sprites/Asteroid/Asteroid" + std::to_string(i) + ".png";
		Vector2 position = Vector2(randomX(gen) * 1.0f, randomY(gen) * 1.0f);
		Vector2 velocity = Vector2(-randomSpeed(gen) * 1.0f, 0);
		int rotationSpeed = randomRotationSpeed(gen) * (rand() % 2 == 0 ? 1 : -1);
		float scale = randomSize(gen) * 0.1f;
		addAsteroid(path, scale, position, velocity, rotationSpeed);
	}


	TTF_CloseFont(normalFont);
	TTF_CloseFont(bigFont);
}

MenuScene::~MenuScene() {
	_uis.clear();
	_spriteObjects.clear();
	SDL_DestroyTexture(_background);
}

void MenuScene::startScene() {
	
}

void MenuScene::endScene() {
	
}

void MenuScene::handleEvent(SDL_Event event) {
	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->handleEvent(event);
	}
}

void MenuScene::draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, _background, NULL, NULL);

	for (int i = 0; i < _spriteObjects.size(); i++) {
		_spriteObjects[i]->draw(renderer);
	}

	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->draw(renderer);
	}
}

void MenuScene::update() {
	for (int i = 0; i < _spriteObjects.size(); i++) {
		_spriteObjects[i]->update();
	}
	for (int i = 0; i < _uis.size(); i++) {
		_uis[i]->update();
	}
}

void MenuScene::addUI(UI* ui) {
	_uis.push_back(std::unique_ptr<UI>(ui));
}

void MenuScene::addSpriteObject(SpriteObject* spriteObject) {
	_spriteObjects.push_back(std::unique_ptr<SpriteObject>(spriteObject));
}

void MenuScene::addAsteroid(const std::string& path, float scale, Vector2 position, Vector2 velocity, int rotationSpeed) {
	std::function<void(SpriteObject* self)> behaviour = [velocity, rotationSpeed](SpriteObject* self) {
		Vector2 position = self->getPosition();
		int width = self->getWidth();
		int height = self->getHeight();
		int deltaTime = timer.getAverageDeltaTime();
		position += velocity * static_cast<float>(deltaTime) * 0.001f;
		float rotation = self->getAngle();
		rotation += rotationSpeed * deltaTime * 0.001f;
		if (rotation > 360) {
			rotation = 0;
		}
		self->setAngle(rotation);
		if (position.x < -width * 0.5f) {
			position.x = WINDOW_WIDTH + width * 0.5f;
		}
		self->setPosition(position);
	};
	addSpriteObject(
		new SpriteObject(
			_renderer,
			new Sprite(_renderer, path, scale),
			position,
			behaviour
		)
	);
}