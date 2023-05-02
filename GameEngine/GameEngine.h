#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class GameEngine
        {
private:
    sf::RenderWindow &window;
    sf::Texture mainTexture;

    std::shared_ptr<IGameObject> playerTank;

    std::vector<std::shared_ptr<IGameObject>> map;

    std::shared_ptr<IGameObject> bonus;

    std::vector<std::shared_ptr<IGameObject>> allBullets;

    std::vector<std::shared_ptr<IGameObject>> enemyTanks;

    sf::Sprite hud;

    int points;

    int enemiesOnMap;
    int remainingEnemies;

    int timeBetweenRenderEnemyTank;

    bool isPaused;
    sf::Clock clock;

    int enemyWithBonusCounter;
    int levelNum;
    bool isFlagFallen;
public:
    GameEngine(sf::RenderWindow &window) : window(window) {}
    ~GameEngine() = default;
    void init();
    void update();
    void handleCollisions();
    void togglePause();
    void render();
    void dropBonus();
    void bonusEffect();
    void dropEnemies();
    void updateHUD();
    void end();
    void restart();
};

#endif
