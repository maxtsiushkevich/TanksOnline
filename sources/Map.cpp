#include "../headers/Map.h"
#include <iostream>
#include <fstream>

#include "../headers/MapObject.h"

void Map::loadMap(std::vector<std::shared_ptr<IGameObject>> &map, int levelNum)
{
    map.clear();
    std::string levelFile = "Levels/level" + std::to_string(levelNum) + ".txt";

    std::cout << levelFile << std::endl;

    std::ifstream file(levelFile);

    if (!file.is_open())
        std::cout << "Не удалось открыть файл!" << std::endl;

    float x = 0;
    float y = 0;
    char c;
    while (file.get(c)) {
        if (!std::isspace(c)) {
            // создаем соответсвующий объект и добавляем в вектор
            switch (c) {
                case 'B':
                    map.emplace_back(std::make_shared<Brick>(x * FACTOR * 8., y * FACTOR * 8.));
                    break;
                case 'M':
                    map.emplace_back(std::make_shared<Metal>(x * FACTOR * 8., y * FACTOR * 8.));
                    break;
                case 'W':
                    map.emplace_back(std::make_shared<Water>(x * FACTOR * 8., y * FACTOR * 8.));
                    break;
                case 'G':
                    map.emplace_back(std::make_shared<Grass>(x * FACTOR * 8., y * FACTOR * 8.));
                    break;
                case 'E':
                    map.emplace_back(std::make_shared<Eagle>(x * FACTOR * 8., y * FACTOR * 8.));
                    break;
                default:
                    break;
            }
            x++;
        }
        if (c == ' ')
            x++;

        if (c == '\n') {
            x = 0;
            y++;
        }
    }
    file.close();
}

void Map::update(std::vector<std::shared_ptr<IGameObject>> &map)
{
    for (auto it = map.begin(); it != map.end(); )
    {
        auto* mapObj = dynamic_cast<MapObject*>(it->get());
        if (mapObj && mapObj->getIsDestroyed())
            it = map.erase(it);
        else
            ++it;
    }
}

void Map::render(std::vector<std::shared_ptr<IGameObject>> &map, sf::RenderWindow &window) {
    for (const auto &gameObject : map)
            gameObject->render(window);
}