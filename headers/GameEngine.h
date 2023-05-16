#ifndef GAME_GAMEENGINE_H
#define GAME_GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "../headers/IGameObject.h"
#include <memory>

#include "../headers/Tank.h"
#include "../headers/Bullet.h"
#include "../headers/Map.h"
#include "../headers/MapObject.h"
#include "../headers/Bonus.h"
#include "../headers/Multiplayer.h"

#define FONT_NAME "Font.ttf"
#define SPRITE_LIST_NAME "SpriteList.png"

extern float FACTOR;

class GameState
{
public:
    std::shared_ptr<IGameObject> playerTank;
    std::shared_ptr<IGameObject> allyTank;
    std::shared_ptr<IGameObject> bonus;
    std::vector<std::shared_ptr<IGameObject>> allBullets; // all bullets are stored here
    std::vector<std::shared_ptr<IGameObject>> enemyTanks; // all enemies are stored here
    std::vector<std::shared_ptr<IGameObject>> map;
    int points;
    bool isPaused; // flag is game on pause
    int remainingEnemies; // remaining enemies on level

//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        ar & *playerTank;
//        ar & *allyTank;
//        if (bonus != nullptr) {
//            ar & *bonus;
//        }
//        ar & points;
//        ar & isPaused;
//        ar & remainingEnemies;
//
//        for (auto bullet : allBullets)
//            ar & *bullet;
//
//        //ar & enemyTanks;
//        //ar & map;
//    }

//    GameState()
//    {
//        playerTank = std::make_shared<PlayerTank>(-100,-100, allBullets, false);
//        allyTank = std::make_shared<PlayerTank>(-100, -100, allBullets, false);
//        bonus = std::make_shared<Bonus>(-100, -100, -1);
//    };

    ~GameState() { std::cout <<"destructor" << std::endl;}
};

class GameEngine
{
private:

    sf::Font font; // main font in game

    sf::RenderWindow &window; // main window
    sf::Texture mainTexture;
    sf::Sprite hud; // sprite with HUD
    sf::Clock timeBetweenFrames;
    sf::Clock clockBonusTimer; // еimer that counts how long the bonus is valid

    GameState gameState;

    Multiplayer multiplayer;

    bool isFlagFallen; // flag is base destroyed

    bool isTwoPlayers; // is 2 players on one computer
    bool isOnline; // is online game

    int enemiesOnScreen;
    int timeBetweenRenderEnemyTank;

    bool isClockBonusActive; // flag is clock bonus active, because this bonus stopped all enemies
    int enemyWithBonusCounter; // every 4th enemy drops bonus
    int levelNum;

public:
    GameEngine(sf::RenderWindow &window) : window(window) {}
    ~GameEngine() = default;
    void init(bool isTwoPlayers, bool isOnline);
    void update();
    void updateEnemies(float seconds);
    void updateBullets(float seconds);
    void endInfo();
    void handleCollisions();
    void togglePause();
    void render();
    void dropBonus();
    void bonusEffect();
    void dropEnemies();
    void renderHUD();
    void end();
    void restart();

    void connect();
};

#endif
