#ifndef GAME_IGAMEOBJECT_H
#define GAME_IGAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "IVisitor.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#include <memory>

enum Destination {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class IGameObject : public std::enable_shared_from_this<IGameObject>
{
    friend class GameState;

protected:
    bool isDestroyed;
    sf::Sprite sprite;
    float dx, dy;
public:
    static sf::Texture texture;
    IGameObject(float x, float y);
    virtual ~IGameObject() = default;
    virtual void update(float time) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void handleCollision(IVisitor *visitor) = 0;
    virtual void setIsDestroyed();
    bool getIsDestroyed();
    sf::Sprite &getSprite();

//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        ar & isDestroyed;
//        ar & dx;
//        ar & dy;
//    }

};


#endif
