#include "Map.h"
#include <iostream>
#include <fstream>

#include "../MapObject/MapObject.h"

void Map::loadMap(std::vector<std::unique_ptr<IGameObject>> &map, int levelNum) {
    std::string levelFile = "level" + std::to_string(levelNum);

    std::ifstream file(levelFile);

    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл!" << std::endl;
    }

    int x = 0;
    int y = 0;
    char c;
    while (file.get(c)) {
        if (!std::isspace(c)) {
            //IGameObject *obj;

            //allBullets.emplace_back(std::make_unique<PlayerBullet>(dx, dy, tankDestination));

            // создаем соответсвующий объект и добавляем в вектор
            switch (c) {
                case 'B':
                    //obj = new Brick(x * FACTOR * 8, y * FACTOR * 8);
                    map.emplace_back(std::make_unique<Brick>(x * FACTOR * 8, y * FACTOR * 8));
                    break;
                case 'M':
                    //obj = new Metal(x * FACTOR * 8, y * FACTOR * 8);
                    map.emplace_back(std::make_unique<Metal>(x * FACTOR * 8, y * FACTOR * 8));
                    break;
                case 'W':
                    //obj = new Water(x * FACTOR * 8, y * FACTOR * 8);
                    map.emplace_back(std::make_unique<Water>(x * FACTOR * 8, y * FACTOR * 8));
                    break;
                case 'G':
                    //obj = new Grass(x * FACTOR * 8, y * FACTOR * 8);
                    map.emplace_back(std::make_unique<Grass>(x * FACTOR * 8, y * FACTOR * 8));
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

void Map::update(std::vector<std::unique_ptr<IGameObject>> &map) {
}

void Map::render(std::vector<std::unique_ptr<IGameObject>> &map, sf::RenderWindow &window) {
    for (const auto &gameObject : map) {
        if (gameObject != nullptr)
            gameObject->render(window);
    }
}