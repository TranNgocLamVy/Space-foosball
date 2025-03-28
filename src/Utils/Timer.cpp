#include "Timer.h"

Timer::Timer() {}

Timer& Timer::getInstance() {
    static Timer instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

void Timer::init() {
    _startTime = SDL_GetTicks();
    _lastTick = _startTime;
}

void Timer::setFramerate(int framerate) {
    _framerate = framerate;
	_averageDeltaTime = 1000 / _framerate;
}

void Timer::update() {
    Uint32 tickPerFrame = 1000 / _framerate;
    Uint32 tickPassed = SDL_GetTicks() - _lastTick;
    if (tickPassed < tickPerFrame) {
        SDL_Delay(tickPerFrame - tickPassed);
    }
    _deltaTime = SDL_GetTicks() - _lastTick;
    _lastTick = SDL_GetTicks();
	_averageDeltaTime = (_averageDeltaTime * _frameCount + _deltaTime) / (_frameCount + 1);
	_frameCount++;
}

Uint32 Timer::getDeltaTime() {
    return _deltaTime;
}

Uint32 Timer::getAverageDeltaTime() {
	return 1000 / _framerate;
}

Uint32 Timer::getTime() {
    return SDL_GetTicks() - _startTime;
}
