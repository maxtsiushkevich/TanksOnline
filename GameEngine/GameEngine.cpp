#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Map/Map.h"

void GameEngine :: init()
{
    mainTexture.loadFromFile("SpriteList.png");

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);

    playerTank = new PlayerTank(64 * FACTOR, 192 * FACTOR);
}

void GameEngine :: togglePause()
{
    isPaused = !isPaused;
}

void GameEngine :: update()
{
    if(!isPaused)
    {
        sf::Time time = clock.restart();
        float seconds = time.asSeconds();

        playerTank->update(seconds);

        // проверяем вектор врагов, если пуст какая-нибудь заставка и переход на новый уровень

    }
    else
        clock.restart();
}

void GameEngine :: render()
{
    playerTank->render(window);

    for (auto object : map) {
        object->render(window);
    }
}


void GameEngine :: end()
{
    delete playerTank;
}
