#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../IGameObject/IGameObject.h"

extern double FACTOR;

class GameEngine {
private:
    sf::RenderWindow &window;
    sf::Texture mainTexture;

    std::shared_ptr<IGameObject> playerTank;

    std::vector<std::shared_ptr<IGameObject>> map;

    std::vector<std::shared_ptr<IGameObject>> allBullets;

    std::vector<std::shared_ptr<IGameObject>> enemyTanks;

    sf::Sprite hud;

    int points;

    bool isPaused;
    sf::Clock clock;

    int levelNum;
public:
    GameEngine(sf::RenderWindow &window) : window(window) {}
    ~GameEngine() = default;
    void init();
    void initEnemies();
    void update();
    void handleCollisions();
    void togglePause();
    void render();
    void drawHUD();
    void end();
};

#endif
