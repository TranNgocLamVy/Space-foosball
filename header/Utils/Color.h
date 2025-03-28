#pragma once
#include <SDL.h>

const SDL_Color RED = { 255, 0, 0, 255 };
const SDL_Color GREEN = { 0, 255, 0, 255 };
const SDL_Color BLUE = { 0, 0, 255, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color WHITE = { 255, 255, 255, 255 };

inline void setColor(SDL_Renderer* renderer, const SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}