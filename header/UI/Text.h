#pragma once
#include <SDL_ttf.h>

#include "Global.h"
#include "UI.h"

class Text : public UI {
private:
	SDL_Surface*	_textSurface;
	SDL_Texture*	_textTexture;
	SDL_Rect		_textRect;

public:
	Text(SDL_Renderer* renderer, TTF_Font *font,const std::string& text, SDL_Color color, Vector2 position);
	~Text() {
		SDL_FreeSurface(_textSurface);
		SDL_DestroyTexture(_textTexture);
	};
	void handleEvent(SDL_Event event) override;
	void draw(SDL_Renderer* renderer) override;
	void drawAt(SDL_Renderer* renderer, Vector2 position);
	void update() override;
	SDL_Rect getRect() { return _textRect; }
};

