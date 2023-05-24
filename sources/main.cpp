#include <iostream>
#include <SFML/Graphics.hpp>
#include "../headers/GameEngine.h"


float FACTOR = 3.f;

pthread_mutex_t mutex;

int firstMenu(sf::RenderWindow &window)
{
    std::vector<std::string> menuItems = {"1 PLAYER", "2 PLAYERS", "ONLINE"};
    int selectedItemIndex = 0;

    sf::Font font;
    if (!font.loadFromFile("Font.ttf")) {
        std::cout << "Could not load font file." << std::endl;
        exit(0);
    }

    float centerX = window.getSize().x / 2.f;
    float centerY = window.getSize().y / 2.f;

    std::vector<sf::Text> menuTexts;
    for (size_t i = 0; i < menuItems.size(); i++) {
        sf::Text menuItem(menuItems[i], font, 6*FACTOR);
        menuItem.setOrigin(menuItem.getGlobalBounds().width / 2.f, menuItem.getGlobalBounds().height / 2.f);
        menuItem.setPosition(centerX, centerY + i * 10 *FACTOR);
        menuTexts.push_back(menuItem);
    }
    menuTexts[selectedItemIndex].setFillColor(sf::Color::Red);

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (selectedItemIndex > 0)
                    {
                        menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuTexts[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1) {
                        menuTexts[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuTexts[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Return)
                {
                    switch (selectedItemIndex)
                    {
                        case 0:
                            std::cout << "1 player" << std::endl;
                            return 0;
                        case 1:
                            std::cout << "2 players" << std::endl;
                            return 1;
                        case 2:
                            std::cout << "online" << std::endl;
                            return 2;
                        default:
                            exit(0);
                    }
                }
            }
        }
        window.clear(sf::Color::Black);

        for (const auto &menuItem: menuTexts) {
            window.draw(menuItem);
        }

        window.display();
    }
    exit(0);
}

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(208. * FACTOR + 32. * FACTOR, 208. * FACTOR), "Tanks Online",
                  sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(45);

    int mode = firstMenu(window);

    GameEngine engine(window);

    switch (mode)
    {
        case 0:
            engine.init(false, false);
            break;
        case 1:
            engine.init(true, false);
            break;
        case 2:
            engine.init(true, true);
            break;
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                engine.end();
                exit(0);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                engine.togglePause();
        }
        window.clear();

        pthread_mutex_lock(&mutex);

        engine.update();

        if (engine.getIsServer())
            engine.handleCollisions();

        engine.render();
        pthread_mutex_unlock(&mutex);
        usleep(1);
        window.display();
    }
    engine.end();
    return 0;
}




