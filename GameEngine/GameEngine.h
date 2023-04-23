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

    IGameObject *playerTank;

    std::vector<IGameObject*> map; // объекты карты

    //std::vector<IGameObject*> tanks; // все танки

    std::vector<IGameObject*> allBullets;

    std::vector<IGameObject*> enemyTanks; // заполняется рандомно

    bool isPaused;
    sf::Clock clock;

    int levelNum;
public:
    GameEngine(sf::RenderWindow &window) : window(window) { }
    ~GameEngine() = default;
    void init();
    void update();
    void handleCollisions();
    void togglePause();
    void render();
    void end();
};

#endif
