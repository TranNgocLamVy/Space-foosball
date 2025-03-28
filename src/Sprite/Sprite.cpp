#include "Sprite.h"

#include "Function.h"

Sprite::Sprite(SDL_Renderer* renderer,const std::string& path, float scale) {
	_scale = scale;
	_angle = 0;
	_alpha = 255;

	_texture = loadTexture(renderer, path, _width, _height );
}

Sprite::~Sprite() {
	if (_texture) {
		SDL_DestroyTexture(_texture);
	}
	_texture = nullptr;
}

void Sprite::draw(SDL_Renderer* renderer, Vector2 position) {
	SDL_Rect rect{ position.x - (_width * _scale / 2), position.y - (_height * _scale / 2), _width * _scale, _height * _scale };
	SDL_RenderCopyEx(renderer, _texture, NULL, &rect, _angle, NULL, SDL_FLIP_NONE);
	// Draw rect with white color
	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);*/
}

void Sprite::setScale(float scale) {
	_scale = scale;
}

float Sprite::getScale() {
	return _scale;
}

void Sprite::setAngle(float angle) {
	_angle = angle;
}

float Sprite::getAngle() {
	return _angle;
}

void Sprite::setAlpha(Uint8 alpha) {
	_alpha = alpha;
	SDL_SetTextureAlphaMod(_texture, _alpha);
}

Uint8 Sprite::getAlpha() {
	return _alpha;
}

int Sprite::getWidth() {
	return _width * _scale;
}

int Sprite::getHeight() {
	return _height * _scale;
}

SDL_Rect Sprite::getRect() {
	int width = _width * _scale;
	int height = _height * _scale;
	return { 0, 0, width, height };
}