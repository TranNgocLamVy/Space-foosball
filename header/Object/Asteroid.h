#pragma once
#include <SDL_mixer.h>
#include <vector>

#include "Global.h"
#include "Sprite.h"
#include "SpaceShip.h"
#include "Function.h"

class Asteroid {
private:
	std::unique_ptr<Sprite> _asteroidSprite;
	float _spriteAngle;
	int _radius;

	std::unique_ptr<Sprite> _indicator;

	Vector2 _position;
	Vector2 _velocity;
	Vector2 _minVelocity;
	Vector2 _maxVelocity;

	bool _isSpinning;
	float _spinTime;
	float _velocityAngle;
	std::unique_ptr<Mix_Chunk, SDL_Deleter> _tickSound;

	std::vector<std::unique_ptr<Mix_Chunk, SDL_Deleter>> _asteroidSounds;

public:
	Asteroid(SDL_Renderer* renderer);
	~Asteroid();
	void draw(SDL_Renderer* renderer);
	void update(std::vector<SDL_Rect> rects, std::vector<std::shared_ptr<SpaceShip>> spaceShips);
	void restart();
};

