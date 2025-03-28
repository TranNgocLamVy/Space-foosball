#pragma once
#include <vector>
#include <functional>
#include <SDL_mixer.h>

#include "Global.h"
#include "Sprite.h"
#include "Function.h"

class SpaceShip {
private:
	// Sprite
    std::unique_ptr<Sprite> _shipSprite;
    std::unique_ptr<Sprite> _shieldSprite;

	// Sfx
	std::vector<std::unique_ptr<Mix_Chunk, SDL_Deleter>> _crashSounds;
	std::unique_ptr<Mix_Chunk, SDL_Deleter> _shieldSound;

	// Movement
    int _angle;
    Vector2 _position;
    Vector2 _velocity;
    Vector2 _maxVelocity;
    Vector2 _acceleration;


	bool _isCrashed;
    bool _isMovingUp;
    bool _isMovingDown;
	bool _isUsingSkill;
	float _slowDownAcceleration;
	bool _isCollide;

	int _shieldRadius;
	int _shieldCooldown;
	int _shieldDuration;
	bool _isShielded;
	bool _hitAsteroid;



	std::function<void(SDL_Event event, SpaceShip* self)> _customHandleEvent;

public:
    SpaceShip(SDL_Renderer* renderer, const std::string& path, float scale, Vector2 position, int angle, int hp, std::function<void(SDL_Event event, SpaceShip* self)> customHandleEvent);
    ~SpaceShip();

	// Main
    void draw(SDL_Renderer* renderer);
	void drawShield(SDL_Renderer* renderer);
    void handleEvent(SDL_Event event);
    void update();

	// Movement
	Vector2 getVelocity();
	void setMoveUp(bool isMovingUp);
	bool isMovingUp();
	void setMoveDown(bool isMovingDown);
	bool isMovingDown();
	bool isCollide();
	void setCollide(bool isCollide);
    void stop();

	// Position and Size
    Vector2 getPosition();
    int getWidth();
    int getHeight();
	SDL_Rect getRect();

	// Shield
	void ActivateShield();
    bool isShielded();
	void setIsShielded(bool isShielded);
	bool isHitAsteroid();
	void setHitAsteroid(bool hitAsteroid);
	int getShieldRadius();
	bool isUsingSkill();
	void setUsingSkill(bool isUsingSkill);

	void restart();
};