#ifndef GAME_MULTIPLAYER_H
#define GAME_MULTIPLAYER_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

class GameState;
class PlayerTank;

class SendReceiveMessage
{
    friend class Multiplayer;
private:
    struct playerTank // еще количество жизней
    {
        float dx, dy;
        float time;
        int stars, animation;
        int health;
        int destination;
        bool isInvulnerable, isDestroyed = true;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & dx;
            ar & dy;
            ar & time;
            ar & stars;
            ar & health;
            ar & animation;
            ar & destination;
            ar & isInvulnerable;
            ar & isDestroyed;
        }
    } pt[2];

    struct enemyTank
    {
        float dx, dy;
        int destination, type;
        bool isBonus, isDestroyed = true;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & dx;
            ar &dy;
            ar & type;
            ar & destination;
            ar & isBonus;
            ar & isDestroyed;
        }
    } et[4];

//    struct bullet
//    {
//        float dx, dy;
//        int destination;
//        bool isDestroyed = true, isPowerful = false, isFast = false;
//        friend class boost::serialization::access;
//        template<class Archive>
//        void serialize(Archive& ar, const unsigned int version)
//        {
//            ar & dx;
//            ar & dy;
//            ar & destination;
//            ar & isDestroyed;
//            ar & isPowerful;
//            ar & isFast;
//        }
//    } bul[7];

    struct map
    {
        float dx, dy;
        bool isDestroyed = true;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & dx;
            ar & dy;
            ar & isDestroyed;
        }
    } map[676];

    struct bonus
    {
        int type;
        float dx, dy;
        bool isPicked, isDestroyed = true;
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & dx;
            ar & dy;
            ar & type;
            ar & isPicked;
            ar & isDestroyed;
        }
    } bonus;

    int levelNum, remainingEnemies;
    bool isPaused;

public:
    SendReceiveMessage() = default;

    void generateMessage(const GameState &object);
    void parsingMessageForClient(GameState &gameState);
    void parsingMessageForServer(GameState &gameState);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & pt;
        ar & et;
        //ar & bul;
        ar & map;
        ar & bonus;
        ar & levelNum;
        ar & remainingEnemies;
        ar & isPaused;
    }


};

class Multiplayer
{
public:
    int server;
    int client;
    int port;
    std::string ip;
    struct sockaddr_in settings;

    Multiplayer() = default;
    ~Multiplayer()
    {
        close(server);
        close(client);
    }

    bool serverInit(int port);
    bool clientInit(std::string ip, int port);
    std::string serializeGameState(const GameState &gameState); // сериализует все состояние
    void deserializeGameState(GameState &gameState, std::string data, bool isServer);
    std::string getIpFromStruct();
    int getClientSocket() { return client; }
    int getServerSocket() { return server; }
};
#endif
