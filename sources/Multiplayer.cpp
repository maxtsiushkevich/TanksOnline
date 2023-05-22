#include "../headers/Multiplayer.h"
#include "../headers/GameEngine.h"

bool Multiplayer :: serverInit(int port)
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

bool Multiplayer :: clientInit(std::string ip, int port)
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

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    servaddr.sin_port = htons(port);
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

std::string Multiplayer :: getIpFromStruct() /// если нет подключения к сети, то  вылетает
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

std::string Multiplayer :: serializeGameState(const GameState &gameState)
{
    SendReceiveMessage message;

    message.generateMessage(gameState);

    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    archive << message;
    return oss.str();
}

void Multiplayer :: deserializeGameState(GameState &gameState, std::string data, bool isServer)
{

    std::istringstream iss(data);
    boost::archive::binary_iarchive ar(iss);
    SendReceiveMessage message;
    ar >> message;
    if (isServer)
        message.parsingMessageForServer(gameState);
    else
        message.parsingMessageForClient(gameState);
}

void SendReceiveMessage :: generateMessage(const GameState &gameState)
{
    if (gameState.playerTank1.use_count() != 0) {
        this->pt[0].dx = gameState.playerTank1->getX();
        this->pt[0].dy = gameState.playerTank1->getY();
        this->pt[0].time = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getTime();
        this->pt[0].isDestroyed = gameState.playerTank1->getIsDestroyed();
        this->pt[0].stars = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getStars();
        this->pt[0].health = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getHealth();
        this->pt[0].destination = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getDestination();
        this->pt[0].isInvulnerable = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getIsInvulnerable();
        this->pt[0].animation = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getAnimation();
    }

    if (gameState.playerTank2.use_count() != 0) {
        this->pt[1].dx = gameState.playerTank2->getX();
        this->pt[1].dy = gameState.playerTank2->getY();
        this->pt[1].time = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getTime();
        this->pt[1].isDestroyed = gameState.playerTank2->getIsDestroyed();
        this->pt[1].stars = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getStars();
        this->pt[1].health = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getHealth();
        this->pt[1].destination = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getDestination();
        this->pt[1].isInvulnerable = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getIsInvulnerable();
        this->pt[1].animation = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getAnimation();
    }

    //for (int i = 0; i < gameState.enemyTanks.size(); i++)
    //{
    //    this->et[i].dx = gameState.enemyTanks[i]->getX();
    //    this->et[i].dy = gameState.enemyTanks[i]->getY();
    //    this->et[i].isDestroyed = gameState.enemyTanks[i]->getIsDestroyed();
    //    this->et[i].destination = dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->getDestination();
    //    this->et[i].type = dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->getType();
    //    this->et[i].isBonus = dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->getIsBonusTank();
    //}

//    for (int i = 0; i < gameState.allBullets.size(); i++)
//    {
//        this->bul[i].dx = gameState.allBullets[i]->getX();
//        this->bul[i].dy = gameState.allBullets[i]->getY();
//        this->bul[i].isDestroyed = gameState.allBullets[i]->getIsDestroyed();
//        this->bul[i].destination = dynamic_cast<Bullet*>(gameState.allBullets[i].get())->getDestination();
//
//        if (dynamic_cast<PlayerPowerfulBullet*>(gameState.allBullets[i].get()))
//            this->bul[i].isPowerful = true;
//        if (dynamic_cast<PlayerFastBullet*>(gameState.allBullets[i].get()))
//            this->bul[i].isFast = true;
//    }

    for (int i = 0; i < gameState.map.size(); i++)
    {
        this->map[i].dx = gameState.map[i]->getX();
        this->map[i].dy = gameState.map[i]->getY();
        this->map[i].isDestroyed = gameState.map[i]->getIsDestroyed();
    }

    //if (gameState.bonus.use_count() != 0) // добавить тип бонуса
    //{
    //    this->bonus.dx = gameState.bonus->getX();
    //    this->bonus.dy = gameState.bonus->getY();
    //    this->bonus.isDestroyed = gameState.bonus->getIsDestroyed();
    //    this->bonus.isPicked = dynamic_cast<Bonus*>(gameState.bonus.get())->getIsPicked();
    //}

    this->levelNum = gameState.levelNum;
    this->remainingEnemies = gameState.remainingEnemies;
    this->isPaused = gameState.isPaused;
}

void SendReceiveMessage :: parsingMessageForClient(GameState &gameState)
{
    gameState.playerTank1->setX(this->pt[0].dx);
    gameState.playerTank1->setY(this->pt[0].dy);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setTime(this->pt[0].time);
    gameState.playerTank1->setIsDestroyed(this->pt[0].isDestroyed);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setStars(this->pt[0].stars);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setHealth(this->pt[0].health);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setDestination(this->pt[0].destination);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setIsInvulnerable(this->pt[0].isInvulnerable);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setAnimation(this->pt[0].animation);
    dynamic_cast<PlayerTank*>(gameState.playerTank1.get())->setSprite();


    gameState.playerTank2->setX(this->pt[1].dx);
    gameState.playerTank2->setY(this->pt[1].dy);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setTime(this->pt[1].time);
    gameState.playerTank2->setIsDestroyed(this->pt[1].isDestroyed);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setStars(this->pt[1].stars);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setHealth(this->pt[1].health);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setDestination(this->pt[1].destination);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setIsInvulnerable(this->pt[1].isInvulnerable);
    dynamic_cast<PlayerTank*>(gameState.playerTank2.get())->setAnimation(this->pt[1].animation);
    // танку игрока 2 нет смысла устанавливать спрайт, так как его спрайт уже стоит как надо


//    for (int i = 0; !this->et[i].isDestroyed; i++)
//    {
//        gameState.enemyTanks[i]->setX(this->et[i].dx);
//        gameState.enemyTanks[i]->setY(this->et[i].dy);
//        gameState.enemyTanks[i]->setIsDestroyed( this->et[i].isDestroyed );
//        dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->setDestination(this->et[i].destination);
//        dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->setType(this->et[i].type);
//        dynamic_cast<EnemyTank*>(gameState.enemyTanks[i].get())->setIsBonusTank(this->et[i].isBonus);
//
//    }

//    for (int i = 0; !this->bul[i].isDestroyed; i++)
//    {
//        gameState.allBullets[i]->setX(this->bul[i].dx);
//        gameState.allBullets[i]->setY(this->bul[i].dy);
//        gameState.allBullets[i]->setIsDestroyed(this->bul[i].isDestroyed);
//        dynamic_cast<Bullet*>(gameState.allBullets[i].get())->setDestination(this->bul[i].destination);
//    }

    for (int i = 0; i < gameState.map.size(); i++) // уничтоженные объекты карты не удаляются из вектора, поэтому его размер неизменен
    {
        gameState.map[i]->setX(this->map[i].dx);
        gameState.map[i]->setY(this->map[i].dy);
        gameState.map[i]->setIsDestroyed(this->map[i].isDestroyed);
    }

    //if (!this->bonus.isDestroyed) // переделать
    //{
    //    this->bonus.dx = gameState.bonus->getX();
    //    this->bonus.dy = gameState.bonus->getY();
    //    this->bonus.isDestroyed = gameState.bonus->getIsDestroyed();
    //    this->bonus.isPicked = dynamic_cast<Bonus*>(gameState.bonus.get())->getIsPicked();
    //}


    if (gameState.levelNum != this->levelNum)
    {
        gameState.levelNum = this->levelNum;
        Map::loadMap(gameState.map, gameState.levelNum);
    }
    gameState.remainingEnemies = this->remainingEnemies;
    gameState.isPaused = this->isPaused;
}

void SendReceiveMessage :: parsingMessageForServer(GameState &gameState)
{
    if (gameState.playerTank2.use_count() != 0)
    {

        gameState.playerTank2->setX(this->pt[1].dx);
        gameState.playerTank2->setY(this->pt[1].dy);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setTime(this->pt[1].time);
        gameState.playerTank2->setIsDestroyed(this->pt[1].isDestroyed);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setStars(this->pt[1].stars);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setDestination(this->pt[1].destination);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setIsInvulnerable(this->pt[1].isInvulnerable);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setAnimation(this->pt[1].animation);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setSprite();
    }

//    for (int i = 0; !this->bul[i].isDestroyed; i++)
//    {
//        if (gameState.allBullets.size() > i+1)
//        {
//            gameState.allBullets[i]->setX(this->bul[i].dx);
//            gameState.allBullets[i]->setY(this->bul[i].dy);
//            gameState.allBullets[i]->setIsDestroyed(this->bul[i].isDestroyed);
//            dynamic_cast<Bullet *>(gameState.allBullets[i].get())->setDestination(this->bul[i].destination);
//        }
//        else
//        {
//            gameState
//        }
//    }

}