#include <iomanip>
#include <sstream>

#include "GameManager.h"
#include "Function.h"
#include "Text.h"
#include "Button.h"
#include "Sprite.h"

int gameTime = 180000;

GameManager::GameManager() {
	_deltaTime = 0;
	_time = gameTime;

	_player1Ready = false;
	_player1Score = 0;

	_player2Ready = false;
	_player2Score = 0;

	_isStarted = false;
	_isPaused = false;
	_isEnded = false;

	_player1ScoreTextDuration = 0;
	_player2ScoreTextDuration = 0;
}

GameManager::~GameManager() {}

void GameManager::update() {
	if (_isPaused || !_isStarted) {
		_deltaTime = 0;
		return;
	}

	int deltaTime = timer.getAverageDeltaTime();

	_deltaTime = deltaTime;
	_time -= deltaTime;
	if (_time <= 0) {
		_time = 0;
		_isEnded = true;
	}
	_player1ScoreTextDuration -= deltaTime;
	_player2ScoreTextDuration -= deltaTime;
	if (_player1ScoreTextDuration < 0) {
		_player1ScoreTextDuration = 0;
	}
	if (_player2ScoreTextDuration < 0) {
		_player2ScoreTextDuration = 0;
	}
}

void GameManager::draw(SDL_Renderer* renderer) {
	drawIntruction(renderer);
	TTF_Font* font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 80);
	int gap = 100;

	// Player 1 score
	SDL_Color blue = { 58, 167, 222, 255 };
	Text* player1Score = new Text(renderer, font, std::to_string(_player1Score), blue, Vector2(WINDOW_WIDTH * 1 / 4 - gap, 46));
	player1Score->draw(renderer);
	delete player1Score;
	if (_player1ScoreTextDuration > 0) {
		Text* player1ScoreText = new Text(renderer, font, "PLAYER 1 SCORE!", blue, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 150));
		SDL_Rect rect = player1ScoreText->getRect();
		rect = { rect.x - 40, rect.y - 20, rect.w + 80, rect.h + 40 };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, &rect);
		player1ScoreText->draw(renderer);
		delete player1ScoreText;
	}

	SDL_Color red = { 221, 58, 58, 255 };
	Text* player2Score = new Text(renderer, font, std::to_string(_player2Score), red, Vector2(WINDOW_WIDTH * 3 / 4 + gap, 46));
	player2Score->draw(renderer);
	delete player2Score;
	if (_player2ScoreTextDuration > 0) {
		Text* player2ScoreText = new Text(renderer, font, "PLAYER 2 SCORE!", red, Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 150));
		SDL_Rect rect = player2ScoreText->getRect();
		rect = { rect.x - 40, rect.y - 20, rect.w + 80, rect.h + 40 };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, &rect);
		player2ScoreText->draw(renderer);
		delete player2ScoreText;
	}

	TTF_CloseFont(font);
	font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 60);

	//Time in seconds limit to 3 decimal places
	int minutes = _time / 60000;
	int seconds = (_time % 60000) / 1000;
	SDL_Color color = _time < 30000 ? RED : WHITE;
	std::string time = (minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes)) + ":" + (seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds));
	Text* timeText = new Text(renderer, font, time, color, Vector2(WINDOW_WIDTH / 2, 46));
	timeText->draw(renderer);
	delete timeText;
	TTF_CloseFont(font);

	int paddingX = 40;
	int paddingY = 20;

	if (!_player1Ready) {
		font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 40);
		Text* player1ReadyText = new Text(renderer, font, "Player 1 press SPACE to ready!", blue, Vector2(WINDOW_WIDTH * 1 / 4, WINDOW_HEIGHT / 2));
		SDL_Rect rect = player1ReadyText->getRect();
		rect = { rect.x - paddingX, rect.y - paddingY, rect.w + paddingX * 2, rect.h + paddingY * 2 };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, &rect);
		player1ReadyText->draw(renderer);
		delete player1ReadyText;
		TTF_CloseFont(font);
	}

	if (!_player2Ready) {
		font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 40);
		Text* player2ReadyText = new Text(renderer, font, "Player 2 press RIGHT SHIFT to ready!", red, Vector2(WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT / 2));
		SDL_Rect rect = player2ReadyText->getRect();
		rect = { rect.x - paddingX, rect.y - paddingY, rect.w + paddingX * 2, rect.h + paddingY * 2 };
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
		SDL_RenderFillRect(renderer, &rect);
		player2ReadyText->draw(renderer);
		delete player2ReadyText;
		TTF_CloseFont(font);
	}
}

void GameManager::drawIntruction(SDL_Renderer* renderer) {
	TTF_Font* font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 40);

	Text* player1Text = new Text(renderer, font, "Player 1", WHITE, Vector2(75, WINDOW_HEIGHT - 40));
	player1Text->draw(renderer);
	delete player1Text;

	Text* player2Text = new Text(renderer, font, "Player 2", WHITE, Vector2(WINDOW_WIDTH - 75, WINDOW_HEIGHT - 40));
	player2Text->draw(renderer);
	delete player2Text;

	TTF_CloseFont(font);

	font = TTF_OpenFont("assets/fonts/Stepalange.ttf", 24);

	int marginX = 320;
	int gap = 35;
	int buttonSize = 44;

	Button* player1Button = new Button(renderer, "SPACE", 24, Vector2(220, WINDOW_HEIGHT - 54), buttonSize * 2, buttonSize, []() {});
	player1Text = new Text(renderer, font, "SKILL", WHITE, Vector2(220, WINDOW_HEIGHT - 14));
	player1Button->draw(renderer);
	player1Text->draw(renderer);
	delete player1Button;
	delete player1Text;

	player1Button = new Button(renderer, "W", 30, Vector2(marginX, WINDOW_HEIGHT - 54), buttonSize, buttonSize, []() {});
	player1Text = new Text(renderer, font, "UP", WHITE, Vector2(marginX, WINDOW_HEIGHT - 14));
	player1Button->draw(renderer);
	player1Text->draw(renderer);
	delete player1Button;
	delete player1Text;

	player1Button = new Button(renderer, "S", 30, Vector2(marginX + buttonSize + gap, WINDOW_HEIGHT - 54), buttonSize, buttonSize, []() {});
	player1Text = new Text(renderer, font, "DOWN", WHITE, Vector2(marginX + buttonSize + gap, WINDOW_HEIGHT - 14));
	player1Button->draw(renderer);
	player1Text->draw(renderer);
	delete player1Button;
	delete player1Text;

	player1Button = new Button(renderer, "A", 30, Vector2(marginX + (buttonSize + gap) * 2, WINDOW_HEIGHT - 54), buttonSize, buttonSize, []() {});
	player1Text = new Text(renderer, font, "LEFT", WHITE, Vector2(marginX + (buttonSize + gap) * 2, WINDOW_HEIGHT - 14));
	player1Button->draw(renderer);
	player1Text->draw(renderer);
	delete player1Button;
	delete player1Text;

	player1Button = new Button(renderer, "D", 30, Vector2(marginX + (buttonSize + gap) * 3, WINDOW_HEIGHT - 54), buttonSize, buttonSize, []() {});
	player1Text = new Text(renderer, font, "RIGHT", WHITE, Vector2(marginX + (buttonSize + gap) * 3, WINDOW_HEIGHT - 14));
	player1Button->draw(renderer);
	player1Text->draw(renderer);
	delete player1Button;
	delete player1Text;

	marginX = WINDOW_WIDTH - 310;

	Button* player2Button = new Button(renderer, "RSHIFT", 24, Vector2(WINDOW_WIDTH - 210, WINDOW_HEIGHT - 54), buttonSize * 2, buttonSize, []() {});
	player2Text = new Text(renderer, font, "SKILL", WHITE, Vector2(WINDOW_WIDTH - 210, WINDOW_HEIGHT - 14));
	player2Button->draw(renderer);
	player2Text->draw(renderer);
	delete player2Button;
	delete player2Text;

	Sprite* upArrow = new Sprite(renderer, "assets/sprites/UI/Arrow.png", 1);
	upArrow->setAngle(270);
	player2Text = new Text(renderer, font, "RIGHT", WHITE, Vector2(marginX, WINDOW_HEIGHT - 14));
	upArrow->draw(renderer, Vector2(marginX, WINDOW_HEIGHT - 54));
	player2Text->draw(renderer);
	delete upArrow;
	delete player2Text;

	Sprite* downArrow = new Sprite(renderer, "assets/sprites/UI/Arrow.png", 1);
	downArrow->setAngle(90);
	player2Text = new Text(renderer, font, "LEFT", WHITE, Vector2(marginX - buttonSize - gap, WINDOW_HEIGHT - 14));
	downArrow->draw(renderer, Vector2(marginX - buttonSize - gap, WINDOW_HEIGHT - 54));
	player2Text->draw(renderer);
	delete downArrow;
	delete player2Text;

	Sprite* leftArrow = new Sprite(renderer, "assets/sprites/UI/Arrow.png", 1);
	leftArrow->setAngle(0);
	player2Text = new Text(renderer, font, "DOWN", WHITE, Vector2(marginX - (buttonSize + gap) * 2, WINDOW_HEIGHT - 14));
	leftArrow->draw(renderer, Vector2(marginX - (buttonSize + gap) * 2, WINDOW_HEIGHT - 54));
	player2Text->draw(renderer);
	delete leftArrow;
	delete player2Text;

	Sprite* rightArrow = new Sprite(renderer, "assets/sprites/UI/Arrow.png", 1);
	rightArrow->setAngle(180);
	player2Text = new Text(renderer, font, "UP", WHITE, Vector2(marginX - (buttonSize + gap) * 3, WINDOW_HEIGHT - 14));
	rightArrow->draw(renderer, Vector2(marginX - (buttonSize + gap) * 3, WINDOW_HEIGHT - 54));
	player2Text->draw(renderer);
	delete rightArrow;
	delete player2Text;

	TTF_CloseFont(font);
}

void GameManager::start() {
	_isStarted = true;
}

void GameManager::pause() {
	_isPaused = true;
}

void GameManager::resume() {
	_isPaused = false;
}

void GameManager::restart() {
	_time = gameTime;
	_player1Score = 0;
	_player2Score = 0;
	_isStarted = false;
	_isPaused = false;
	_isEnded = false;
	_player1Ready = false;
	_player2Ready = false;
}

void GameManager::player1Scored() {
	_player1Score++;
	_player1ScoreTextDuration = 1000;
}

bool GameManager::isPlayer1Ready() {
	return _player1Ready;
}

void GameManager::setPlayer1Ready(bool ready) {
	_player1Ready = ready;
}

void GameManager::player2Scored() {
	_player2Score++;
	_player2ScoreTextDuration = 1000;
}

bool GameManager::isPlayer2Ready() {
	return _player2Ready;
}

void GameManager::setPlayer2Ready(bool ready) {
	_player2Ready = ready;
}

int GameManager::getPlayer1Score() {
	return _player1Score;
}

int GameManager::getPlayer2Score() {
	return _player2Score;
}

int GameManager::getDeltaTime() {
	return _deltaTime;
}

int GameManager::getTime() {
	return _time;
}

bool GameManager::isStarted() {
	return _isStarted;
}

bool GameManager::isPaused() {
	return _isPaused;
}

bool GameManager::isEnded() {
	return _time <= 0;
}

int GameManager::getWinner() {
	if (_player1Score > _player2Score) {
		return 1;
	}
	else if (_player1Score < _player2Score) {
		return -1;
	}
	else {
		return 0;
	}
}