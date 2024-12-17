/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <mutex>

#include "Entity.hpp"
#include "RenderClass.hpp"
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"
#include "Networking.hpp"

class game {
    /* data */
  public:
    game(Networking &net);
    ~game();

    int createConnection(const char *ip, int portTCP, int portUDP);
    int manageBuffers();
    void writeToServer(
        const std::string &data, System::Network::ISocket::Type socketType);
    void receiveMessage();

    void handleConnection(int code, std::vector<std::string> &tokens);
    void handleEnemy(int code, std::vector<std::string> &tokens);
    void handleTerrain(int code, std::vector<std::string> &tokens);
    void handleMechs(int code, std::vector<std::string> &tokens);
    void addEntity(std::shared_ptr<ecs::Entity> entity);

    std::vector<std::shared_ptr<ecs::Entity>> &getEntities();
    int getId();

    void setServerMode(bool mode);
    bool isServer(void);

    void updateLocalplayerPosition();
    std::shared_ptr<ecs::Entity> &getLocalPlayer();

    // Client Management
    void setRenderClass(RenderClass *refRender);
    RenderClass *getRenderClass();
    void loadTexture();
    void createPlayer();

    // Player Management
    void handlePlayer(int code, std::vector<std::string> &tokens);
    void createNewPlayer(std::vector<std::string> &tokens);
    void setNewPosition(std::vector<std::string> &tokens);
    void playerDead(std::vector<std::string> &tokens);
    void createProjectile(std::vector<std::string> &tokens);
    void setPlayerHealth(std::vector<std::string> &tokens);
    void playerDisconnection(std::vector<std::string> &tokens);

    // Enemy Management
    void createEnemy(std::vector<std::string> &tokens);
    void enemyDead(std::vector<std::string> &tokens);
    void enemyShoot(std::vector<std::string> &tokens);
    void enemyDamage(std::vector<std::string> &tokens);

  private:
    int _id;
    std::shared_ptr<ecs::Entity> _player;
    std::vector<std::string> _buffers;
    std::mutex _mutex;
    std::vector<std::shared_ptr<ecs::Entity>> _entities;
    RenderClass *_refRender;
    bool _isServer;
    Networking &_refNetwork;
};

#endif /*game_HPP*/
