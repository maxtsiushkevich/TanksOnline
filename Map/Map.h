#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../IGameObject/IGameObject.h"

class Map
{
public:
    Map() = default;
    ~Map() = default;
    static void loadMap(std::vector<IGameObject*>& map, int levelNum);
    static void update(std::vector<IGameObject*>& map);
};


#endif
