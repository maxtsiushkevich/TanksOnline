#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../IGameObject/IGameObject.h"
#include <memory>

#define FONT_NAME "Font.ttf"
#define SPRITE_LIST_NAME "SpriteList.png"

extern float FACTOR;

class GameEngine
{
private:

    sf::Font font;

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

    sf::Clock clockBonusTimer;
    bool isClockBonusActive;

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
    void renderHUD();
    void end();
    void restart();
};

#endif
