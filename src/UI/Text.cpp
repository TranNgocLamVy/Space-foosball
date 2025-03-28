#include "Text.h"

#include <SDL_ttf.h>

Text::Text(SDL_Renderer* renderer, TTF_Font* font,const std::string& text, SDL_Color color, Vector2 position) {
	int positionX = static_cast<int>(position.x);
	int positionY = static_cast<int>(position.y);
	_textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	_textTexture = SDL_CreateTextureFromSurface(renderer, _textSurface);
	_textRect = { positionX - _textSurface->w / 2, positionY - _textSurface->h / 2, _textSurface->w, _textSurface->h };
}

void Text::handleEvent(SDL_Event event) {}

void Text::draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, _textTexture, NULL, &_textRect);
}

void Text::drawAt(SDL_Renderer* renderer, Vector2 position) {
	_textRect.x = static_cast<int>(position.x - _textSurface->w * 0.5f);
	_textRect.y = static_cast<int>(position.y - _textSurface->h * 0.5f);
	SDL_RenderCopy(renderer, _textTexture, NULL, &_textRect);
}

void Text::update() {}