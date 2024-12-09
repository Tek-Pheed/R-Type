/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** client
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "system_network.hpp"
#include "system_tcp.hpp"
#include "system_udp.hpp"

class client {
    /* data */
  public:
    client();
    ~client();
    int create_connection(const char *ip, int portTCP, int portUDP);
    int manage_buffer(std::string buffer);
    void writeToServer(
        std::string &data, System::Network::ISocket::Type socketType);
    void receive_message();
    void handle_connection(int code, std::vector<std::string> &tokens);
    void handle_player(int code, std::vector<std::string> &tokens);
    void handle_enemy(int code, std::vector<std::string> &tokens);
    void handle_terrain(int code, std::vector<std::string> &tokens);
    void handle_mechs(int code, std::vector<std::string> &tokens);

  private:
    System::Network::TCPSocket _clientSocketTCP;
    System::Network::UDPSocket _clientSocketUDP;

    int _id;
};

#endif /*CLIENT_HPP*/
