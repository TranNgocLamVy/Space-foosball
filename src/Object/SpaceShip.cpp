#include "SpaceShip.h"

#include "Timer.h"

SpaceShip::SpaceShip(SDL_Renderer* renderer, const std::string& path, float scale, Vector2 position, int angle, int hp, std::function<void(SDL_Event event, SpaceShip* self)> customHandleEvent) {
    _shipSprite = std::make_unique<Sprite>(renderer, path, scale);
	_shipSprite->setAngle(angle);

	_shieldSprite = std::make_unique<Sprite>(renderer, "assets/sprites/Ship/Shield.png", 1);
	_shieldSound = std::unique_ptr<Mix_Chunk, SDL_Deleter>(Mix_LoadWAV("assets/sfxs/Shield.mp3"), SDL_Deleter());
    _position = position;
    _velocity = Vector2(0, 0);
    _maxVelocity = Vector2(0, 600);
    _acceleration = Vector2(0, 1000);
    _angle = angle;
    _isMovingUp = false;
    _isMovingDown = false;
	_slowDownAcceleration = 0.98f;
	_customHandleEvent = customHandleEvent;

	_isCrashed = false;
	_isCollide = false;

	_shieldRadius = 100;
    _shieldCooldown = 0;
    _shieldDuration = 0;
    _isShielded = false;
	_hitAsteroid = false;
	_isUsingSkill = false;
}

SpaceShip::~SpaceShip() {
    _shipSprite = nullptr;
}

void SpaceShip::draw(SDL_Renderer* renderer) {
    _shipSprite->draw(renderer, _position);
}

void SpaceShip::drawShield(SDL_Renderer* renderer) {
	if (_shieldDuration > 0) {
		_shieldSprite->draw(renderer, _position);
	}
}

void SpaceShip::handleEvent(SDL_Event event) {
	_customHandleEvent(event, this);
}

void SpaceShip::update() {
	int deltaTime = timer.getDeltaTime();
    float deltaTimeSeconds = deltaTime / 1000.0f;
    if ((!_isMovingUp && !_isMovingDown) || (_isMovingUp && _isMovingDown)) {
        _velocity.y *= _slowDownAcceleration;
		if (abs(_velocity.y) < 10) {
			_velocity.y = 0;
		}
    } else if (_isMovingUp) {
        _velocity.y -= _acceleration.y * deltaTimeSeconds;
    } else if (_isMovingDown) {
        _velocity.y += _acceleration.y * deltaTimeSeconds;
    }

    if (_velocity.y > _maxVelocity.y) {
        _velocity.y = _maxVelocity.y;
    }
    if (_velocity.y < -_maxVelocity.y) {
        _velocity.y = -_maxVelocity.y;
    }

	float bounce = 0.5;
    int borderTop = BORDER_WIDTH + _shipSprite->getHeight() * 0.5;
	int borderBottom = WINDOW_HEIGHT - BORDER_WIDTH - _shipSprite->getHeight() * 0.5;
	if ((_position.y <= borderTop && _velocity.y < 0) || (_position.y >= borderBottom && _velocity.y > 0)) {
		if (!_isCrashed) {
            Mix_Chunk* crashSound = Mix_LoadWAV("assets/sfxs/Space_ship_crash.mp3");
			_crashSounds.push_back(std::unique_ptr<Mix_Chunk, SDL_Deleter>(crashSound, SDL_Deleter()));
            float volume = MIX_MAX_VOLUME * (abs(_velocity.y) / _maxVelocity.y) * SFX_VOLUME;
            Mix_VolumeChunk(crashSound, volume);
			Mix_PlayChannel(-1, crashSound, 0);
			_isCrashed = true;
		}
	    _velocity.y = -bounce * _velocity.y;
	} else {
        _position.y += _velocity.y * deltaTimeSeconds;
		if (_position.y < borderTop) {
			_position.y = borderTop;
		}
		if (_position.y > borderBottom) {
			_position.y = borderBottom;
		}
		_isCrashed = false;
    }

	for (int i = 0; i < _crashSounds.size(); i++) {
		if (!IsChunkPlaying(_crashSounds[i].get())) {
			_crashSounds.erase(_crashSounds.begin() + i);
		}
	}

	if (_shieldCooldown > 0) {
		_shieldCooldown -= deltaTime;
		if (_shieldCooldown <= 0) {
			_shieldCooldown = 0;
		}
	}

	_shieldDuration -= deltaTime;
	if (_shieldDuration <= 0) {
		_shieldDuration = 0;
		_isShielded = false;
		_hitAsteroid = false;
	}
}

Vector2 SpaceShip::getVelocity() {
	return _velocity;
}

void SpaceShip::setMoveUp(bool isMovingUp) {
	_isMovingUp = isMovingUp;
}

bool SpaceShip::isMovingUp() {
	return _isMovingUp;
}

void SpaceShip::setMoveDown(bool isMovingDown) {
	_isMovingDown = isMovingDown;
}

bool SpaceShip::isMovingDown() {
	return _isMovingDown;
}

bool SpaceShip::isCollide() {
	return _isCollide;
}

void SpaceShip::setCollide(bool isCollide) {
	_isCollide = isCollide;
}

void SpaceShip::stop() {
	_isMovingDown = false;
	_isMovingUp = false;
	_isUsingSkill = false;
}


// Position and Size
Vector2 SpaceShip::getPosition() {
    return _position;
}

int SpaceShip::getWidth() {
    return _shipSprite->getWidth();
}

int SpaceShip::getHeight() {
    return _shipSprite->getHeight();
}

SDL_Rect SpaceShip::getRect() {
	SDL_Rect rect = _shipSprite->getRect();
	rect.x = _position.x - rect.w * 0.5;
	rect.y = _position.y - rect.h * 0.5;
	return rect;
}

// Shield
void SpaceShip::ActivateShield() {
	if (_shieldCooldown <= 0 && !_isShielded) {
		Mix_PlayChannel(-1, _shieldSound.get(), 0);
		_isShielded = true;
		_hitAsteroid = false;
		_shieldDuration = 500;
		_shieldCooldown = 500;
	}
}

bool SpaceShip::isShielded() {
	return _isShielded;
}

void SpaceShip::setIsShielded(bool isShielded) {
	_isShielded = isShielded;
}

bool SpaceShip::isHitAsteroid() {
	return _hitAsteroid;
}

void SpaceShip::setHitAsteroid(bool hitAsteroid) {
	_hitAsteroid = hitAsteroid;
}

int SpaceShip::getShieldRadius() {
	return _shieldRadius;
}

bool SpaceShip::isUsingSkill() {
	return _isUsingSkill;
}

void SpaceShip::setUsingSkill(bool isUsingSkill) {
	_isUsingSkill = isUsingSkill;
}

void SpaceShip::restart() {
	_position = Vector2(_position.x, WINDOW_HEIGHT / 2);
	_velocity = Vector2(0, 0);
	_isMovingUp = false;
	_isMovingDown = false;
	_isUsingSkill = false;
	_isShielded = false;
	_hitAsteroid = false;
	_shieldCooldown = 0;
	_shieldDuration = 0;
	_isCrashed = false;
}