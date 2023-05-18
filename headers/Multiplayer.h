#ifndef GAME_MULTIPLAYER_H
#define GAME_MULTIPLAYER_H

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

class Multiplayer
{
public:
    int server, client;
    std::string ip;
    int port;
    struct sockaddr_in servaddr;

    ~Multiplayer()
    {
        close(server);
        close(client);
    }

    bool serverInit(int port);

    bool clientInit(std::string ip, int port);

    std::string getIpFromStruct();
};
#endif
