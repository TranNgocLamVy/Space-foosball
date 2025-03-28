#include "SpriteObject.h"

SpriteObject::SpriteObject(SDL_Renderer* renderer, Sprite* sprite, Vector2 position, std::function<void(SpriteObject* self)> behaviour) {
	_type = SpriteType::Sprite;
	_position = position;
	_behaviour = behaviour;

	_sprite = std::unique_ptr<Sprite>(sprite);
	_gif = nullptr;
}

SpriteObject::SpriteObject(SDL_Renderer* renderer, GIF* gif, Vector2 position, std::function<void(SpriteObject* self)> behaviour) {
	_type = SpriteType::GIF;
	_position = position;
	_behaviour = behaviour;

	_sprite = nullptr;
	_gif = std::unique_ptr<GIF>(gif);
}

SpriteObject::~SpriteObject() {
	_sprite = nullptr;
	_gif = nullptr;
}

void SpriteObject::hanleEvent(SDL_Event event) {

}

void SpriteObject::draw(SDL_Renderer* renderer) {
	switch (_type) {
	case SpriteType::Sprite:
		_sprite->draw(renderer, _position);
		break;
	case SpriteType::GIF:
		_gif->draw(renderer, _position);
		break;
	default:
		break;
	}
}

void SpriteObject::update() {
	_behaviour(this);
}

void SpriteObject::setPosition(Vector2 position) {
	_position = position;
}

Vector2 SpriteObject::getPosition() {
	return _position;
}

int SpriteObject::getWidth() {
	switch (_type) {
	case SpriteType::Sprite:
		return _sprite->getWidth();
		break;
	case SpriteType::GIF:
		return _gif->getWidth();
		break;
	default:
		return -1;
		break;
	}
}

int SpriteObject::getHeight() {
	switch (_type) {
	case SpriteType::Sprite:
		return _sprite->getHeight();
		break;
	case SpriteType::GIF:
		return _gif->getHeight();
		break;
	default:
		return -1;
		break;
	}
}

SDL_Rect SpriteObject::getRect() {
	switch (_type) {
	case SpriteType::Sprite:
		SDL_Rect rect = _sprite->getRect();
		rect.x = static_cast<int>(_position.x - rect.w * 0.5);
		rect.y = static_cast<int>(_position.y - rect.h * 0.5);
		return rect;
		break;
	case SpriteType::GIF:
		return { 0, 0, 0, 0 };
		break;
	default:
		return { 0, 0, 0, 0 };
		break;
	}
}

void SpriteObject::setAngle(float rotation) {
	switch (_type) {
	case SpriteType::Sprite:
		_sprite->setAngle(rotation);
		break;
	case SpriteType::GIF:
		/*_gif->setRotation(rotation);*/
		break;
	default:
		break;
	}
}

float SpriteObject::getAngle() {
	switch (_type) {
	case SpriteType::Sprite:
		return _sprite->getAngle();
		break;
	case SpriteType::GIF:
		return 0;
		break;
	default:
		return 0;
		break;
	}
}