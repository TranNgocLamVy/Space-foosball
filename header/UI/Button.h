#pragma once
#include <functional>
#include <SDL_mixer.h>


#include "Global.h"
#include "UI.h"

class Button : public UI {
private:
	SDL_Surface*	_textSurface;
	SDL_Texture*	_textTexture;
	SDL_Rect		_textRect;
	SDL_Rect		_rect;
	Mix_Chunk*		_clickSound;
	Mix_Chunk*		_hoverSound;
	bool			_hasOnClick;

	std::function<void()> _onClick;
	bool			_isHovered = false;
	bool            _renderBorder;

public:
	Button(SDL_Renderer* renderer, std::string text, int fontSize, Vector2 position, int width, int height, std::function<void()> onClick);
	Button(SDL_Renderer* renderer, std::string path, Vector2 position, int width, int height, std::function<void()> onClick);
	~Button() {
		if (_textSurface) SDL_FreeSurface(_textSurface);
		if (_textTexture) SDL_DestroyTexture(_textTexture);
		if (_clickSound) Mix_FreeChunk(_clickSound);
		if (_hoverSound) Mix_FreeChunk(_hoverSound);
	};
	void handleEvent(SDL_Event event) override;
	void draw(SDL_Renderer* renderer) override;
	void update() override;
	void setHasOnClick(bool hasOnClick) { _hasOnClick = hasOnClick; }
};

