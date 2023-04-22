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

    std::vector<IGameObject*> map;

    bool isPaused;
    sf::Clock clock;

    int levelNum;
public:
    GameEngine(sf::RenderWindow &window) : window(window) { }
    ~GameEngine() = default;
    void init();
    void update();
    void togglePause();
    void render();
    void end();
};

#endif
