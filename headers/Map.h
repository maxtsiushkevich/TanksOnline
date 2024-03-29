#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../headers/IGameObject.h"

class Map {
public:
    Map() = default;
    virtual ~Map() = 0;
    static void loadMap(std::vector<std::shared_ptr<IGameObject>> &map, int levelNum);
    static void update(std::vector<std::shared_ptr<IGameObject>> &map) { }
    static void render(std::vector<std::shared_ptr<IGameObject>> &map, sf::RenderWindow &window);
};

#endif
