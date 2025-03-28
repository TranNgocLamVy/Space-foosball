#pragma once
#include <SDL.h>

class Timer {
private:
    Uint32 _startTime = 0;
    Uint32 _lastTick = 0;
    Uint16 _framerate = 0;
    Uint16 _deltaTime = 0;
	Uint32 _averageDeltaTime = 0;
	Uint64 _frameCount = 0;

    Timer();
    ~Timer() {}

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

public:
    static Timer& getInstance();

    void init();
    void setFramerate(int framerate);
    void update();
    Uint32 getDeltaTime();
	Uint32 getAverageDeltaTime();
    Uint32 getTime();
};

#define timer Timer::getInstance()