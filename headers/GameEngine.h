#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../headers/IGameObject.h"
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
    sf::Sprite hud;
    sf::Clock timeBetweenFrames;
    sf::Clock clockBonusTimer;

    std::shared_ptr<IGameObject> playerTank;
    std::shared_ptr<IGameObject> allyTank;
    std::vector<std::shared_ptr<IGameObject>> map;
    std::shared_ptr<IGameObject> bonus;
    std::vector<std::shared_ptr<IGameObject>> allBullets;
    std::vector<std::shared_ptr<IGameObject>> enemyTanks;
    int points;
    bool isPaused;
    bool isFlagFallen;
    bool isTwoPlayers;

    int enemiesOnScreen;
    int remainingEnemies;
    int timeBetweenRenderEnemyTank;

    bool isClockBonusActive;
    int enemyWithBonusCounter;
    int levelNum;

public:
    GameEngine(sf::RenderWindow &window) : window(window) {}
    ~GameEngine() = default;
    void init(bool isTwoPlayers);
    void update();
    void updateEnemies(float seconds);
    void updateBullets(float seconds);
    void endInfo();
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
