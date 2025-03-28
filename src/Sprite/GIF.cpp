#include "GIF.h"

#include "Function.h"
#include "Vector2.h"
#include "Timer.h"

GIF::GIF(SDL_Renderer* renderer, std::string path, float scale, float speed) {
	_animation = loadGIFAnimation(path);
	_currentFrame = 0;
	_delay = 0; 
	_scale = scale;
	_speed = speed;
	
	if (!_animation) return;

	int sumDelay = 0;
	for (int i = 0; i < _animation->count; i++) {
		sumDelay += _animation->delays[i];
	}
	_avarageDelay = sumDelay / _animation->count;
}

GIF::~GIF() {
	IMG_FreeAnimation(_animation);
	_animation = nullptr;
}

void GIF::draw(SDL_Renderer* renderer, Vector2 position) {
	if (!_animation) return;

	int deltaTime = timer.getAverageDeltaTime();
	_delay -= deltaTime * _speed;
	while (_delay <= 0) {
		_currentFrame++;
		_delay += _avarageDelay;
		_currentFrame %= _animation->count;
		if (_currentFrame == 0) _currentFrame++;
	}
	SDL_Surface* frame = _animation->frames[_currentFrame];
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, frame);
	SDL_Rect rect{ position.x - (frame->w * _scale / 2), position.y - (frame->h * _scale / 2), frame->w * _scale, frame->h * _scale};
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
}

void GIF::setSpeed(float speed) {
	_speed = speed;
}

float GIF::getSpeed() {
	return _speed;
}

void GIF::setScale(float scale) {
	_scale = scale;
}

float GIF::getScale() {
	return _scale;
}

int GIF::getWidth() {
	if (!_animation) return 0;
	return _animation->w * _scale;
}

int GIF::getHeight() {
	if (!_animation) return 0;
	return _animation->h * _scale;
}





