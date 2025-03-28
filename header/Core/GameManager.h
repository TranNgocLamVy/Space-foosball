#pragma once
#include <SDL_ttf.h>

#include "Global.h"
#include "Timer.h"

class GameManager {
private:
	int _deltaTime;
	int _time;
	bool _isStarted;
	bool _isPaused;
	bool _isEnded;

	bool _player1Ready;
	int _player1Score;
	int _player1ScoreTextDuration;

	bool _player2Ready;
	int _player2Score;
	int _player2ScoreTextDuration;

public:
	GameManager();
	~GameManager();
	void update();
	void draw(SDL_Renderer* renderer);
	void drawIntruction(SDL_Renderer* renderer);
	void start();
	void pause();
	void resume();
	void restart();

	bool isPlayer1Ready();
	void setPlayer1Ready(bool ready);
	void player1Scored();
	int getPlayer1Score();

	bool isPlayer2Ready();
	void setPlayer2Ready(bool ready);
	void player2Scored();
	int getPlayer2Score();

	int getDeltaTime();
	int getTime();
	bool isStarted();
	bool isPaused();
	bool isEnded();
	int getWinner();
};

