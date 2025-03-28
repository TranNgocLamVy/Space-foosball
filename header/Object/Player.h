#pragma once
#include <vector>
#include <memory>

#include "Global.h"
#include "SpaceShip.h"
#include "Text.h"

class Player {
private:
	std::unique_ptr<Text> _name;
	std::vector<std::shared_ptr<SpaceShip>> _spaceShips;
	std::shared_ptr<SpaceShip> _currentSpaceShip;
	int _currentSpaceShipIndex;
	Sprite* _indicator;
	SDL_KeyCode _keyLeft;
	SDL_KeyCode _keyRight;
	SDL_KeyCode _keySkill;
	bool _isChangeShip;

public:
	Player(SDL_Renderer* renderer, const std::string& path, const std::string& name, std::vector<Vector2> shipPositions, const std::string& indicatorPath, SDL_Color color, SDL_KeyCode keyUp, SDL_KeyCode keyDown, SDL_KeyCode keyLeft, SDL_KeyCode keyRight, SDL_KeyCode keySkill);
	~Player();
	void draw(SDL_Renderer* renderer);
	void handleEvent(SDL_Event event);
	void update();
	void setCurrentSpaceShip(int index);
	std::shared_ptr<SpaceShip> getCurrentSpaceShip();
	std::vector<std::shared_ptr<SpaceShip>> getSpaceShips();
	void restart();
};

