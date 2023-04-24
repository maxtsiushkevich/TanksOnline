#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class GameEngine {
private:
    sf::RenderWindow &window;
    sf::Texture mainTexture;

    std::unique_ptr<IGameObject> playerTank;

    std::vector<std::unique_ptr<IGameObject>> map;

    std::vector<std::unique_ptr<IGameObject>> allBullets;

    std::vector<std::unique_ptr<IGameObject>> enemyTanks;

    bool isPaused;
    sf::Clock clock;

    int levelNum;
public:
    GameEngine(sf::RenderWindow &window) : window(window) {}
    ~GameEngine() = default;
    void init();
    void update();
    void handleCollisions();
    void togglePause();
    void render();
    void end();
};

#endif
