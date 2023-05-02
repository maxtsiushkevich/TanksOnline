cmake_minimum_required(VERSION 3.24)
project(Game)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(SFML_INCLUDE_DIR /usr/local/include/SFML)

add_executable(Game
        main.cpp
        Tank/Tank.h
        IGameObject/IGameObject.h
        Bullet/Bullet.h
        IVisitor/IVisitor.h
        GameEngine/GameEngine.h
        GameEngine/GameEngine.cpp
        Tank/Tank.cpp
        Bullet/Bullet.cpp
        IVisitor/IVisitor.cpp IGameObject/IGameObject.cpp Map/Map.cpp Map/Map.h MapObject/MapObject.cpp MapObject/MapObject.h Bonus/Bonus.cpp Bonus/Bonus.h Tank/PlayerTank.cpp Tank/EnemyTank.cpp)

target_link_libraries(Game sfml-graphics sfml-audio)
