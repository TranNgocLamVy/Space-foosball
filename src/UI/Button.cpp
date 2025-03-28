#include "Button.h"

#include <SDL_ttf.h>

#include "Function.h"

Button::Button(SDL_Renderer* renderer, std::string text, int fontSize, Vector2 position, int width, int height, std::function<void()> onClick) {
	int positionX = static_cast<int>(position.x);
	int positionY = static_cast<int>(position.y);
	_rect.w = width;
	_rect.h = height;
	_rect.x = positionX - width / 2;
	_rect.y = positionY - height / 2;
	_onClick = onClick;
	_hasOnClick = true;
	_renderBorder = true;

	_clickSound = Mix_LoadWAV("assets/sfxs/Button_click.mp3");
	Mix_VolumeChunk(_clickSound, MIX_MAX_VOLUME);
	_hoverSound = Mix_LoadWAV("assets/sfxs/Button_click.mp3");
	Mix_VolumeChunk(_hoverSound, MIX_MAX_VOLUME);

	TTF_Font *font = TTF_OpenFont("assets/fonts/Stepalange.ttf", fontSize);
	_textSurface = TTF_RenderText_Solid(font, text.c_str(), WHITE);
	_textTexture = SDL_CreateTextureFromSurface(renderer, _textSurface);
	_textRect = { positionX - _textSurface->w / 2, positionY - _textSurface->h / 2, _textSurface->w, _textSurface->h };

	TTF_CloseFont(font);
}

Button::Button(SDL_Renderer* renderer, std::string path, Vector2 position, int width, int height, std::function<void()> onClick) {
	int positionX = static_cast<int>(position.x);
	int positionY = static_cast<int>(position.y);
	_rect.w = width;
	_rect.h = height;
	_rect.x = positionX - width / 2;
	_rect.y = positionY - height / 2;
	_onClick = onClick;
	_hasOnClick = false;
	_renderBorder = false;
	_clickSound = Mix_LoadWAV("assets/sfxs/Button_click.mp3");
	Mix_VolumeChunk(_clickSound, MIX_MAX_VOLUME);
	_hoverSound = Mix_LoadWAV("assets/sfxs/Button_click.mp3");
	Mix_VolumeChunk(_hoverSound, MIX_MAX_VOLUME);
	SDL_Surface* surface = loadSurface(path);
	_textSurface = surface;
	_textTexture = SDL_CreateTextureFromSurface(renderer, surface);
	_textRect = { positionX - surface->w / 2, positionY - surface->h / 2, surface->w, surface->h };
	SDL_FreeSurface(surface);
}

void Button::handleEvent(SDL_Event event) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	
	bool inRect = x >= _rect.x && x <= _rect.x + _rect.w && y >= _rect.y && y <= _rect.y + _rect.h;
	if (event.type == SDL_MOUSEBUTTONDOWN && inRect) {
		Mix_PlayChannel(-1, _clickSound, 0);
		_isHovered = false;
		SDL_SetTextureColorMod(_textTexture, 255, 255, 255);
		static SDL_Cursor* defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(defaultCursor);
		_onClick();
	}
}

void Button::draw(SDL_Renderer* renderer) {
	SDL_Rect textRect = { _textRect.x, _textRect.y + 2, _textRect.w, _textRect.h };
	if (_isHovered) {
		setColor(renderer, WHITE);
		SDL_RenderFillRect(renderer, &_rect);
		SDL_RenderCopy(renderer, _textTexture, NULL, &textRect);
	} else {
		setColor(renderer, WHITE);
		if (_renderBorder) {
			int borderThickness = 5;
			for (int i = 0; i < borderThickness; ++i) {
				SDL_Rect borderRect = { _rect.x + i, _rect.y + i, _rect.w - 2 * i, _rect.h - 2 * i };
				SDL_RenderDrawRect(renderer, &borderRect);
			}
		}
		SDL_RenderCopy(renderer, _textTexture, NULL, &textRect);
	}
}

void Button::update() {
	if (!_hasOnClick) return;
	int x, y;
	SDL_GetMouseState(&x, &y);
	bool inRect = x >= _rect.x && x <= _rect.x + _rect.w && y >= _rect.y && y <= _rect.y + _rect.h;

	if (inRect && !_isHovered) {
		/*Mix_PlayChannel(-1, _hoverSound, 0);*/
		_isHovered = true;
		SDL_SetTextureColorMod(_textTexture, 0, 0, 0);
		static SDL_Cursor* handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(handCursor);
	}
	else if (!inRect && _isHovered) {
		_isHovered = false;
		SDL_SetTextureColorMod(_textTexture, 255, 255, 255);
		static SDL_Cursor* defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(defaultCursor);
	}
}
