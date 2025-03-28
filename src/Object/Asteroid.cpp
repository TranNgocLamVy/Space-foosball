#include "Asteroid.h"

#include "GameScene.h"


Asteroid::Asteroid(SDL_Renderer* renderer) {
	float size = 0.5;
	_asteroidSprite = std::make_unique<Sprite>(renderer, "assets/sprites/Asteroid/Asteroid.png", 1 * size);
	_radius = 50;
	_spriteAngle = 0;

	_position = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	_velocity = Vector2(200, 200);
	_maxVelocity = Vector2(900, 900);
	_minVelocity = Vector2(200, 200);
    
	_isSpinning = true;
	
	_spinTime = 2000 + rand() % 500;
    _velocityAngle = -90;
	_tickSound = std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Tick1.mp3"), SDL_Deleter());

    _indicator = std::make_unique<Sprite>(renderer, "assets/sprites/UI/Indicator.png", 1);
    _indicator->setAngle(_velocityAngle - 90);

    _asteroidSounds.push_back(std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Asteroid1.mp3"), SDL_Deleter()));
    _asteroidSounds.push_back(std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Asteroid2.mp3"), SDL_Deleter()));
    _asteroidSounds.push_back(std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Asteroid3.mp3"), SDL_Deleter()));
    _asteroidSounds.push_back(std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Asteroid4.mp3"), SDL_Deleter()));
	for (int i = 0; i < _asteroidSounds.size(); i++) {
		Mix_VolumeChunk(_asteroidSounds[i].get(), 20);
	}
}

Asteroid::~Asteroid() { 
	_asteroidSprite = nullptr;
}

void Asteroid::draw(SDL_Renderer * renderer) {
	_asteroidSprite->draw(renderer, _position);

	Vector2 position = getVectorTip(_position, _radius, _velocityAngle);
    _indicator->setAngle(_velocityAngle - 90);
	if (_isSpinning) {
		_indicator->draw(renderer, position);
	}
}

void Asteroid::update(std::vector<SDL_Rect> rects, std::vector<std::shared_ptr<SpaceShip>> spaceShips) {
	GameManager* gameManager = GameScene::getGameManager();

	if (gameManager->isPaused() || !gameManager->isStarted()) {
		return;
	}

    if (!gameManager->isPlayer1Ready() || !gameManager->isPlayer2Ready()) {
        return;
    }

	if (_isSpinning) {
		// Calculate spin speed base on the spin time
		float spinSpeed = _spinTime / 100;
		float newAngle = _velocityAngle + spinSpeed;
		int tick = static_cast<int>(std::floor(newAngle / 90)) - static_cast<int>(std::floor((_velocityAngle - 1) / 90));
		if (tick > 0) {
			Mix_PlayChannel(-1, _tickSound.get(), 0);
		}
		_velocityAngle += spinSpeed;
		_velocityAngle = fmod(_velocityAngle, 360);
		_spinTime -= gameManager->getDeltaTime();
		if (_spinTime < 0) {
			_spinTime = 0;
			_isSpinning = false;
			_velocity = Vector2(cos(_velocityAngle * M_PI / 180), sin(_velocityAngle * M_PI / 180)) * _minVelocity.magnitude();
        } else {
            return;
        }
	}


	float dt = gameManager->getDeltaTime() * 0.001f;

    _spriteAngle += 150 * dt;
    _asteroidSprite->setAngle(_spriteAngle);

	_velocityAngle = angleWithXAxis(_velocity);

	_velocity *= 0.99f;
	_velocity.clamp(_minVelocity.magnitude(), _maxVelocity.magnitude());

	_position += _velocity * dt;

    SDL_Rect spriteRect = _asteroidSprite->getRect();
    int width = spriteRect.w;
    int height = spriteRect.h;

    
    SDL_Rect asteroidRect;
    asteroidRect.w = width;
    asteroidRect.h = height;
    asteroidRect.x = static_cast<int>(_position.x - width / 2.0f);
    asteroidRect.y = static_cast<int>(_position.y - height / 2.0f);

    
    if (asteroidRect.x < 50) {
		gameManager->player2Scored();
		gameManager->setPlayer1Ready(false);
		gameManager->setPlayer2Ready(false);
		_position = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		_velocity = Vector2(200, 200);
        _spinTime = 2000 + rand() % 500;
		_isSpinning = true;
		_velocityAngle = -90;
    } else if (asteroidRect.x > WINDOW_WIDTH - 50) {
		gameManager->player1Scored();
		gameManager->setPlayer1Ready(false);
		gameManager->setPlayer2Ready(false);
		_position = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		_velocity = Vector2(200, 200);
		_spinTime = 2000 + rand() % 500;
		_isSpinning = true;
		_velocityAngle = -90;
    }

    asteroidRect.x = static_cast<int>(_position.x - width / 2.0f);
    asteroidRect.y = static_cast<int>(_position.y - height / 2.0f);

	SpaceShip* collidedShip = nullptr;

	for (int i = 0; i < spaceShips.size(); i++) {
		SpaceShip* spaceShip = spaceShips[i].get();
		Vector2 shipPosition = spaceShip->getPosition();
		int shieldRadius = spaceShips[i]->getShieldRadius();
		SDL_Rect shipRect = spaceShip->getRect();
		Vector2 shipVelocity = spaceShip->getVelocity();
		float distance = sqrt(pow(shipPosition.x - _position.x, 2) + pow(shipPosition.y - _position.y, 2));
		bool inShield = distance < shieldRadius;
		if (spaceShip->isShielded() && !spaceShip->isHitAsteroid() && inShield) {
			Vector2 direction = _position - shipPosition;
			direction.normalized();
			Vector2 magnitude = _maxVelocity - (_maxVelocity - _minVelocity) * (distance / shieldRadius);
			_velocity = direction * magnitude.magnitude();
			spaceShip->setHitAsteroid(true);
		}
		else {
			float distanceX = abs(_position.x - shipPosition.x);
			if (SDL_HasIntersection(&asteroidRect, &shipRect)) {
				if (!spaceShip->isCollide()) {
					std::cout << "Collide with ship" << std::endl;
					spaceShips[i]->setCollide(true);
					SDL_Rect rect = spaceShips[i]->getRect();
					rects.push_back(rect);
				}
				else if (_position.y < shipPosition.y) {
					if (_velocity.y < 0 && _velocity.y > shipVelocity.y) {
						_velocity.y = shipVelocity.y;
					}
					else {
						_velocity.y *= -1;
					}
					_position.y = shipPosition.y - shipRect.h / 2 - height / 2 - 10;
				}
				else if (_position.y > shipPosition.y) {
					if (_velocity.y > 0 && _velocity.y < shipVelocity.y) {
						_velocity.y = shipVelocity.y;
					}
					else {
						_velocity.y *= -1;
					}
					_position.y = shipPosition.y + shipRect.h / 2 + height / 2 + 10;
				}
				collidedShip = spaceShip;
			} else if (distanceX > shipRect.w / 2 + asteroidRect.w) {
				spaceShip->setCollide(false);
			}
		}
	}

    for (SDL_Rect rect : rects) {
        if (SDL_HasIntersection(&asteroidRect, &rect)) {
            int overlapX = std::min(asteroidRect.x + asteroidRect.w, rect.x + rect.w) - std::max(asteroidRect.x, rect.x);
            int overlapY = std::min(asteroidRect.y + asteroidRect.h, rect.y + rect.h) - std::max(asteroidRect.y, rect.y);

            int soundIndex = rand() % _asteroidSounds.size();
            Mix_PlayChannel(-1, _asteroidSounds[soundIndex].get(), 0);

            if (overlapX < overlapY) {
                if (asteroidRect.x < rect.x) {
                    _position.x -= overlapX;
                } else {
                    _position.x += overlapX;
                }
                _velocity.x *= -1;
            } else {
                if (asteroidRect.y < rect.y) {
                    _position.y -= overlapY;
                } else {
                    _position.y += overlapY;
                }
                _velocity.y *= -1;
            }
            break;
        }
    }

	if (asteroidRect.y < 92) {
		_position.y = 100 + height;
		if (collidedShip) {
			Vector2 shipPosition = collidedShip->getPosition();
			if (shipPosition.x < _position.x) {
				_position.x = shipPosition.x + collidedShip->getRect().w / 2 + width / 2 + 10;
			}
			else {
				_position.x = shipPosition.x - collidedShip->getRect().w / 2 - width / 2 - 10;
			}
		}
	}
	else if (asteroidRect.y > WINDOW_HEIGHT - 92) {
		_position.y = WINDOW_HEIGHT - 100 - height;
		if (collidedShip) {
			Vector2 shipPosition = collidedShip->getPosition();
			if (shipPosition.x < _position.x) {
				_position.x = shipPosition.x + collidedShip->getRect().w / 2 + width / 2 + 10;
			}
			else {
				_position.x = shipPosition.x - collidedShip->getRect().w / 2 - width / 2 - 10;
			}
		}
	}
}

void Asteroid::restart() {
	_position = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	_velocity = Vector2(200, 200);
	_maxVelocity = Vector2(900, 900);
	_minVelocity = Vector2(200, 200);
	_isSpinning = true;
	_spinTime = 2000 + rand() % 500;
	_velocityAngle = -90;
}