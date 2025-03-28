#pragma once
#include <functional>
#include <memory>

#include "Global.h"
#include "Sprite.h"
#include "GIF.h"

enum class SpriteType {
	Sprite,
	GIF,
};

class SpriteObject {
private:
	std::unique_ptr<Sprite> _sprite;
	std::unique_ptr<GIF> _gif;

	std::function<void(SpriteObject* self)> _behaviour;
	Vector2 _position;
	Vector2 _velocity;
	SpriteType _type;

public:
	SpriteObject(SDL_Renderer* renderer, Sprite* sprite, Vector2 position, std::function<void(SpriteObject* self)> behaviour);
	SpriteObject(SDL_Renderer* renderer, GIF* gif, Vector2 position, std::function<void(SpriteObject* self)> behaviour);
	~SpriteObject();
	void hanleEvent(SDL_Event event);
	void draw(SDL_Renderer* renderer);
	void update();
	void setPosition(Vector2 position);
	Vector2 getPosition();
	int getWidth();
	int getHeight();
	SDL_Rect getRect();
	void setAngle(float rotation);
	float getAngle();
};

