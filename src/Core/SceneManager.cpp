#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager::SceneManager() {
    _transitionDuration = 500;
    _transitionDirection = 0;
    _transitionAlpha = 0;
}

SceneManager::~SceneManager() {
	_scenes.clear();
	_currentScene = nullptr;
	_nextScene = nullptr;
}

void SceneManager::addScene(std::shared_ptr<Scene> scene) {
    for (const auto& s : _scenes) {
        if (s->getName() == scene->getName()) {
            std::cerr << "Scene with name \"" << scene->getName() << "\" already exists!" << std::endl;
            return;
        }
    }
    _scenes.push_back(scene);
}

void SceneManager::setCurrentScene(const std::string& name) {
	for (const auto& s : _scenes) {
		if (s->getName() == name) {
			_currentScene = s;
			_currentScene->startScene();
			return;
		}
	}
	std::cerr << "Scene with name \"" << name << "\" does not exist!" << std::endl;
}

void SceneManager::changeScene(const std::string& name) {
	if (_isTransitioning) return;

    for (const auto& s : _scenes) {
        if (s->getName() == name) {
            _nextScene = s;
			_isTransitioning = true;
            _transitionDirection = 1;
            return;
        }
    }
    std::cerr << "Scene with name \"" << name << "\" does not exist!" << std::endl;
}

void SceneManager::handleEvent(SDL_Event event) {
    if (_isTransitioning) return;

    if (_currentScene) {
        _currentScene->handleEvent(event);
    } else {
        std::cerr << "No current scene to handle events!" << std::endl;
		return;
    }
}

void SceneManager::draw(SDL_Renderer* renderer) {
    if (_currentScene) {
        _currentScene->draw(renderer);
    } else {
        std::cerr << "No current scene to draw!" << std::endl;
		return;
    }

    if (_isTransitioning) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, _transitionAlpha);
        SDL_RenderFillRect(renderer, nullptr);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

		int _transitionSpeed = 255 / (_transitionDuration / timer.getAverageDeltaTime());
        if (_transitionDirection == 1) {
            _transitionAlpha += _transitionSpeed;
            if (_transitionAlpha >= 255) {
                _transitionAlpha = 255;
                _transitionDirection = 0;
            }
        } else if (_transitionDirection == 0) {
            _currentScene->endScene();
            _currentScene = _nextScene;
            _currentScene->startScene();
            _transitionDirection = -1;
        } else if (_transitionDirection == -1) {
            _transitionAlpha -= _transitionSpeed;
            if (_transitionAlpha <= 0) {
                _transitionAlpha = 0;
                _isTransitioning = false;
            }
        }
    }
}

void SceneManager::update() {
	if (_isTransitioning) return;
	if (_currentScene) {
		_currentScene->update();
	}
	else {
		std::cerr << "No current scene to update!" << std::endl;
		return;
	}
}

void SceneManager::clear() {
	_scenes.clear();
	_currentScene = nullptr;
	_nextScene = nullptr;
}
