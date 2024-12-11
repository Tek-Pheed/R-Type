/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>
#include <mutex>

#include "Entity.hpp"
class RenderClass;
#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

class client {
    /* data */
  public:
    client(RenderClass &render);
    ~client();
    int create_connection(const char *ip, int portTCP, int portUDP);
    int manage_buffers();
    void writeToServer(
        const std::string &data, System::Network::ISocket::Type socketType);
    void receive_message();
    void handle_connection(int code, std::vector<std::string> &tokens);
    void handle_enemy(int code, std::vector<std::string> &tokens);
    void handle_terrain(int code, std::vector<std::string> &tokens);
    void handle_mechs(int code, std::vector<std::string> &tokens);
    void add_entity(std::shared_ptr<ecs::Entity> entity);
    std::vector<std::shared_ptr<ecs::Entity>> &get_entities();
    int get_id();

    void update_localplayer_position();
    std::shared_ptr<ecs::Entity> &getLocalPlayer();

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

  private:
    System::Network::TCPSocket _clientSocketTCP;
    System::Network::UDPSocket _clientSocketUDP;

    int _id;
    std::shared_ptr<ecs::Entity> _player;
    std::vector<std::string> _buffers;
    std::mutex _mutex;
    std::vector<std::shared_ptr<ecs::Entity>> _entities;
    RenderClass &_refRender;
};

#endif /*CLIENT_HPP*/
