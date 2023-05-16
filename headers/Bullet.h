#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#define PLAYER_BULLET_SPEED 110.f
#define PLAYER_FAST_BULLET_SPEED 220.f
#define ENEMY_BULLET_SPEED 90.f
#define ENEMY_FAST_BULLET_SPEED 180.f

#include "../headers/IGameObject.h"
#include "../headers/Tank.h"

class Bullet : public IGameObject {
    friend class GameState;
protected:
    float speed;
    std::shared_ptr<IGameObject> owner;
    Destination bulletDestination;
public:
    Bullet(float x, float y, float speed, Destination dest, std::shared_ptr<IGameObject> owner);
    ~Bullet() override = default;
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    bool checkBounds();
    void setIsDestroyed() override;
    std::shared_ptr<IGameObject> getOwner() const;

//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        IGameObject::serialize(ar, version);
//        ar & *owner;
//        ar & bulletDestination;
//    }
};

class PlayerBullet : public Bullet // пуля игрока
{
    friend class GameState;
public:
    PlayerBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;

//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        Bullet::serialize(ar, version);
//    }
};

class PlayerFastBullet : public Bullet {
    friend class GameState;
public:
    PlayerFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        Bullet::serialize(ar, version);
//    }
};

class PlayerPowerfulBullet : public Bullet {
public:
    PlayerPowerfulBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
    friend class boost::serialization::access;

//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        Bullet::serialize(ar, version);
//    }
};

class EnemyBullet : public Bullet // пуля врага
{
    friend class GameState;
public:
    EnemyBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        Bullet::serialize(ar, version);
//    }
};

class EnemyFastBullet : public Bullet // пуля врага
{
    friend class GameState;
public:
    EnemyFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
//    friend class boost::serialization::access;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        Bullet::serialize(ar, version);
//    }
};

#endif
