#ifndef GAME_MULTIPLAYER_H
#define GAME_MULTIPLAYER_H

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
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

    bool serverInit(int port)
    {
        this->port = port;
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == -1)
        {
            std::cout << "socket creation failed";
            close(server);
            exit(0);
        }
        else
            std::cout << "socket created" << std::endl;

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        //servaddr.sin_port = htons(port);
        servaddr.sin_port = htons(port);

        if (bind(server, (struct sockaddr*)&servaddr, sizeof(servaddr)))
        {
            std::cout << "socket bind failed";
            close(server);
            exit(0);
        }
        else
            std::cout << "socket binded" << std::endl;

        if (listen(server, 1) != 0)
        {
            std::cout << "listen error";
            close(server);
            exit(0);
        }
        else
            std::cout << "server listening..." << std::endl;

        struct sockaddr_in cli;
        int len = sizeof(cli);

        client = accept(server, (struct sockaddr*)&cli, reinterpret_cast<socklen_t *>(&len));
        if (client < 0)
        {
            std::cout << "server accepted failed";
            close(server);
            exit(0);
        }
        else
            std::cout << "server accepted successfully" << std::endl;

        return true;
    }

    bool clientInit(std::string ip, int port)
    {
        this->port = port;
        this->ip = ip;
        client = socket(AF_INET, SOCK_STREAM, 0);
        if (client == -1)
        {
            std::cout << "socket creation failed";
            close(client);
            exit(0);
        }
        else
            std::cout << "socket created" << std::endl;

        servaddr.sin_family = AF_INET;
        //servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
        //servaddr.sin_port = port;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = port;

        if (connect(client, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        {
            std::cout << "connection with server failed";
            close(client);
            return false;
        }
        else
            std::cout << "connected to server" << std::endl;
        return true;
    }

    std::string getIpFromStruct()
    {
        struct ifaddrs *addrs, *tmp;
        getifaddrs(&addrs);
        tmp = addrs;

        while (tmp)
        {
            if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
                if (strcmp(tmp->ifa_name, "lo0") != 0 && (pAddr->sin_addr.s_addr & htonl(0xff000000)) != htonl(0x7f000000)) {
                    return inet_ntoa(pAddr->sin_addr);
                }
            }
            tmp = tmp->ifa_next;
        }
        freeifaddrs(addrs);
    }
};
#endif
