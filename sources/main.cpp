#include <iostream>
#include <SFML/Graphics.hpp>
#include "../headers/GameEngine.h"

#include "../headers/MapObject.h"

float FACTOR = 5.f;

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(208. * FACTOR + 32. * FACTOR, 208. * FACTOR), "Tanks Online",
                  sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(45);

    GameEngine engine(window);
    //engine.init(true);
    engine.init(false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                engine.togglePause();
        }

        //window.clear(sf::Color::White);
        window.clear();

        engine.update();
        engine.handleCollisions();
        engine.render();

        window.display();
    }
    engine.end();
    return 0;
}


//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu");
//    window.setFramerateLimit(60);
//
//    std::vector<std::string> menuItems = {"Start Game", "Options", "Exit"};
//    int selectedItemIndex = 0;
//
//    sf::Font font;
//    if (!font.loadFromFile("Font.ttf")) {
//        std::cout << "Could not load font file." << std::endl;
//        return 1;
//    }
//
//    std::vector<sf::Text> menuTexts;
//    for (size_t i = 0; i < menuItems.size(); i++) {
//        sf::Text menuItem(menuItems[i], font, 24);
//        menuItem.setPosition(400 - menuItem.getGlobalBounds().width / 2, 200 + i * 50);
//        menuTexts.push_back(menuItem);
//    }
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            } else if (event.type == sf::Event::KeyPressed) {
//                if (event.key.code == sf::Keyboard::Up) {
//                    if (selectedItemIndex > 0) {
//                        menuTexts[selectedItemIndex].setColor(sf::Color::White);
//                        selectedItemIndex--;
//                        menuTexts[selectedItemIndex].setColor(sf::Color::Red);
//                    }
//                } else if (event.key.code == sf::Keyboard::Down) {
//                    if (selectedItemIndex < menuItems.size() - 1) {
//                        menuTexts[selectedItemIndex].setColor(sf::Color::White);
//                        selectedItemIndex++;
//                        menuTexts[selectedItemIndex].setColor(sf::Color::Red);
//                    }
//                } else if (event.key.code == sf::Keyboard::Return) {
//                    switch (selectedItemIndex) {
//                        case 0:
//                            std::cout << "Starting game..." << std::endl;
//                            break;
//                        case 1:
//                            std::cout << "Opening options menu..." << std::endl;
//                            break;
//                        case 2:
//                            window.close();
//                            break;
//                    }
//                }
//            }
//        }
//        window.clear(sf::Color::Black);
//
//        for (const auto &menuItem: menuTexts) {
//            window.draw(menuItem);
//        }
//
//        window.display();
//    }
//
//    return 0;
//}



