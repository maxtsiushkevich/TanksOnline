#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameEngine/GameEngine.h"

#include "MapObject/MapObject.h"

float FACTOR = 5.f;

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(208. * FACTOR + 32. * FACTOR , 208. * FACTOR), "Tanks Online", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    GameEngine engine(window);
    engine.init();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                engine.togglePause();
        }

        window.clear();

        engine.update();
        engine.handleCollisions();
        // проверяем коллизии
        engine.render();

        window.display();
    }
    engine.end();
    return 0;
}
