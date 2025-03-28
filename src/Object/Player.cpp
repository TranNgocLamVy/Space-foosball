#include "Player.h"

#include <SDL_ttf.h>
#include <functional>

#include "GameScene.h"

Player::Player(SDL_Renderer* renderer, const std::string& path, const std::string& name, std::vector<Vector2> shipPositions, const std::string& indicatorPath, SDL_Color color, SDL_KeyCode keyUp, SDL_KeyCode keyDown, SDL_KeyCode keyLeft, SDL_KeyCode keyRight, SDL_KeyCode keySkill) {
	_isChangeShip = false;
	
	std::function<void(SDL_Event event, SpaceShip* self)> customHandleEvent = [keyUp, keyDown, keySkill](SDL_Event event, SpaceShip* self) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == keyUp) {
				self->setMoveUp(true);
			}
			if (event.key.keysym.sym == keyDown) {
				self->setMoveDown(true);
			}
			if (event.key.keysym.sym == keySkill) {
				GameManager* gameManager = GameScene::getGameManager();
				if (gameManager->isPlayer1Ready() && gameManager->isPlayer2Ready() && !self->isUsingSkill()) {
					self->ActivateShield();
					self->setUsingSkill(true);
				}
			}
		}
		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == keyUp) {
				self->setMoveUp(false);
			}
			if (event.key.keysym.sym == keyDown) {
				self->setMoveDown(false);
			}
			if (event.key.keysym.sym == keySkill) {
				self->setUsingSkill(false);
			}
		}
	};

	TTF_Font* normalFont = TTF_OpenFont("assets/fonts/Stepalange.ttf", 20);
	_name = std::make_unique<Text>(renderer, normalFont, name, color, Vector2(0,0));

	_keyLeft = keyLeft;
	_keyRight = keyRight;
	_keySkill = keySkill;
	_indicator = new Sprite(renderer, indicatorPath.c_str(), 1);

	for (int i = 0; i < shipPositions.size(); i++) {
		_spaceShips.push_back(std::make_shared<SpaceShip>(renderer, path, 0.5, shipPositions[i], 0, 100, customHandleEvent));
	}
	
	_currentSpaceShip = _spaceShips[0];
	_currentSpaceShipIndex = 0;


	TTF_CloseFont(normalFont);
}

Player::~Player() {
	_spaceShips.clear();
	_currentSpaceShip = nullptr;
}

void Player::draw(SDL_Renderer* renderer) {
	for (int i = 0; i < _spaceShips.size(); i++) {
		_spaceShips[i]->draw(renderer);
	}
	if (_currentSpaceShip == nullptr) {
		return;
	}
	_currentSpaceShip->drawShield(renderer);

	Vector2 position = _currentSpaceShip->getPosition();
	int height = _currentSpaceShip->getHeight();
	_indicator->draw(renderer, Vector2(position.x, position.y - height + 15));
	_name->drawAt(renderer, Vector2(position.x, position.y - height - 10));
}

void Player::update() {
	for (int i = 0; i < _spaceShips.size(); i++) {
		_spaceShips[i]->update();
	}
}

void Player::setCurrentSpaceShip(int index) {
	_currentSpaceShip = _spaceShips[index];
	_currentSpaceShipIndex = index;
}

void Player::handleEvent(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == _keyLeft && !_isChangeShip) {
			_currentSpaceShipIndex--;
			if (_currentSpaceShipIndex < 0) {
				_currentSpaceShipIndex = _spaceShips.size() - 1;
			}
			bool isMovingUp = _currentSpaceShip->isMovingUp();
			bool isMovingDown = _currentSpaceShip->isMovingDown();
			bool isUsingSkill = _currentSpaceShip->isUsingSkill();
			_currentSpaceShip->stop();
			_currentSpaceShip = _spaceShips[_currentSpaceShipIndex];
			_currentSpaceShip->setMoveUp(isMovingUp);
			_currentSpaceShip->setMoveDown(isMovingDown);
			_currentSpaceShip->setUsingSkill(isUsingSkill);
			_isChangeShip = true;
		}
		if (event.key.keysym.sym == _keyRight && !_isChangeShip) {
			_currentSpaceShipIndex++;
			if (_currentSpaceShipIndex >= _spaceShips.size()) {
				_currentSpaceShipIndex = 0;
			}
			bool isMovingUp = _currentSpaceShip->isMovingUp();
			bool isMovingDown = _currentSpaceShip->isMovingDown();
			bool isUsingSkill = _currentSpaceShip->isUsingSkill();
			_currentSpaceShip->stop();
			_currentSpaceShip = _spaceShips[_currentSpaceShipIndex];
			_currentSpaceShip->setMoveUp(isMovingUp);
			_currentSpaceShip->setMoveDown(isMovingDown);
			_currentSpaceShip->setUsingSkill(isUsingSkill);
			_isChangeShip = true;
		}
	}
	else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == _keyLeft || event.key.keysym.sym == _keyRight) {
			_isChangeShip = false;
		}
	}
	_currentSpaceShip->handleEvent(event);
}

std::shared_ptr<SpaceShip> Player::getCurrentSpaceShip() {
	return _currentSpaceShip;
}

std::vector<std::shared_ptr<SpaceShip>> Player::getSpaceShips() {
	return _spaceShips;
}

void Player::restart() {
	for (int i = 0; i < _spaceShips.size(); i++) {
		_spaceShips[i]->restart();
	}
	_currentSpaceShip = _spaceShips[0];
	_currentSpaceShipIndex = 0;
}