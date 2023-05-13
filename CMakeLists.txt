cmake_minimum_required(VERSION 3.24)
project(Game)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Game
        sources/main.cpp
        headers/Tank.h
        headers/IGameObject.h
        headers/Bullet.h
        headers/IVisitor.h
        headers/GameEngine.h
        sources/GameEngine.cpp
        sources/Tank.cpp
        sources/Bullet.cpp
        sources/IVisitor.cpp sources/IGameObject.cpp sources/Map.cpp headers/Map.h sources/MapObject.cpp headers/MapObject.h sources/Bonus.cpp headers/Bonus.h sources/PlayerTank.cpp sources/EnemyTank.cpp sources/main.cpp sources/main.cpp)

target_link_libraries(Game sfml-graphics sfml-audio)
