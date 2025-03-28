#pragma once
#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Constants.h"

inline SDL_Texture* loadBackground(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    if (!optimizedSurface) {
        std::cerr << "Failed to convert surface format: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
    SDL_FreeSurface(optimizedSurface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}

inline SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path, int& width, int& height) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	width = surface->w;
	height = surface->h;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
		return nullptr;
	}
	return texture;
}

inline SDL_Surface* loadSurface(const std::string& path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	return surface;
}

inline IMG_Animation* loadGIFAnimation(const std::string& path) {
	IMG_Animation* animation = IMG_LoadGIFAnimation_RW(SDL_RWFromFile(path.c_str(), "rb"));
	if (!animation) {
		std::cerr << "Failed to load animation: " << IMG_GetError() << std::endl;
		return nullptr;
	}
	return animation;
}

inline bool IsChunkPlaying(Mix_Chunk* chunk) {
	int num_channels = Mix_AllocateChannels(-1); // Get the total number of allocated channels
	for (int i = 0; i < num_channels; i++) {
		if (Mix_Playing(i) && Mix_GetChunk(i) == chunk) {
			return 1; // The chunk is being played on this channel
		}
	}
	return 0; // The chunk is not being played on any channel
}

inline void drawPixel(SDL_Renderer* renderer, int x, int y) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

inline void drawCircleBorder(SDL_Renderer* renderer, Vector2 position, int radius) {
	int32_t offsetX, offsetY, d;
	offsetX = 0;
	offsetY = radius;
	d = 3 - 2 * radius;

	while (offsetY >= offsetX) {
		drawPixel(renderer, static_cast<int>(position.x + offsetX), static_cast<int>(position.y + offsetY));
		drawPixel(renderer, static_cast<int>(position.x - offsetX), static_cast<int>(position.y + offsetY));
		drawPixel(renderer, static_cast<int>(position.x + offsetX), static_cast<int>(position.y - offsetY));
		drawPixel(renderer, static_cast<int>(position.x - offsetX), static_cast<int>(position.y - offsetY));
		drawPixel(renderer, static_cast<int>(position.x + offsetY), static_cast<int>(position.y + offsetX));
		drawPixel(renderer, static_cast<int>(position.x - offsetY), static_cast<int>(position.y + offsetX));
		drawPixel(renderer, static_cast<int>(position.x + offsetY), static_cast<int>(position.y - offsetX));
		drawPixel(renderer, static_cast<int>(position.x - offsetY), static_cast<int>(position.y - offsetX));

		if (d < 0) {
			d += 4 * offsetX + 6;
		}
		else {
			d += 4 * (offsetX - offsetY) + 10;
			offsetY--;
		}
		offsetX++;
	}
}

inline bool aabbCollisionCircle(SDL_Rect rect, Vector2 circle, int radius) {
	// Convert rectangle coordinates to float.
	float rectX = static_cast<float>(rect.x);
	float rectY = static_cast<float>(rect.y);
	float rectXMax = static_cast<float>(rect.x + rect.w);
	float rectYMax = static_cast<float>(rect.y + rect.h);

	// Find the closest point on the rectangle to the circle's center.
	float closestX = std::max(rectX, std::min(circle.x, rectXMax));
	float closestY = std::max(rectY, std::min(circle.y, rectYMax));

	// Compute the distance between the circle's center and this closest point.
	float deltaX = circle.x - closestX;
	float deltaY = circle.y - closestY;

	// Compare squared distance to squared radius (converted to float).
	return (deltaX * deltaX + deltaY * deltaY) <= (static_cast<float>(radius * radius));
}

inline bool aabbCollisionAABB(const SDL_Rect& a, const SDL_Rect& b) {
	// If one rectangle is entirely to the left of the other
	if (a.x + a.w <= b.x) return false;
	if (b.x + b.w <= a.x) return false;

	// If one rectangle is entirely above the other
	if (a.y + a.h <= b.y) return false;
	if (b.y + b.h <= a.y) return false;

	// Otherwise, they must be overlapping
	return true;
}

inline float angleWithXAxis(Vector2 vector) {
	double magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (magnitude == 0) return 0;
	float cosine = vector.x / magnitude;
	float angleRad = std::acos(cosine);
	float angleDeg = angleRad * (180.0f / 3.14159265);
	if (vector.y < 0) {
		angleDeg = 360.0f - angleDeg;
	}
	return angleDeg;
}

inline Vector2 getVectorTip(Vector2 center, double radius, double angleDeg) {
	double angleRad = angleDeg * (3.14159265 / 180.0);
	Vector2 tip;
	tip.x = center.x + radius * std::cos(angleRad);
	tip.y = center.y + radius * std::sin(angleRad);
	return tip;
}

struct SDL_Deleter
{
    void operator()(SDL_Window* p) const { 
		if (!p) return;
		SDL_DestroyWindow(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying window" << std::endl;
#endif
	}
    void operator()(SDL_Renderer* p) const { 
		if (!p) return;
		SDL_DestroyRenderer(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying renderer" << std::endl;
#endif
	}
	void operator()(SDL_Texture* p) const { 
		if (!p) return;
		SDL_DestroyTexture(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying texture" << std::endl;
#endif
	}
	void operator()(SDL_Surface* p) const { 
		if (!p) return;
		SDL_FreeSurface(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying surface" << std::endl;
#endif
	}
	void operator()(IMG_Animation* p) const { 
		if (!p) return;
		IMG_FreeAnimation(p);
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying animation" << std::endl;
#endif 

	}
	void operator()(Mix_Music* p) const { 
		if (!p) return;
		Mix_FreeMusic(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying music" << std::endl;
#endif
	}
	void operator()(Mix_Chunk* p) const { 
		if (!p) return;
		Mix_FreeChunk(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying chunk" << std::endl;
#endif
	}
	void operator()(TTF_Font* p) const { 
		if (!p) return;
		TTF_CloseFont(p); 
#ifdef DEBUG_MEMORY_LEAKS
		std::cout << "Destroying Font" << std::endl;
#endif
	}
};