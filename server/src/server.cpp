/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#include "server.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>

#include "protocol.hpp"
#include "server.hpp"

#define PORT    8081
#define MAXLINE 1400

server::server()
{
}

server::~server()
{
}

void server::create_server()
{
    if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&_servaddr, 0, sizeof(_servaddr));
    memset(&_cliaddr, 0, sizeof(_cliaddr));

    _servaddr.sin_family = AF_INET;
    _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servaddr.sin_port = htons(PORT);

    if (bind(_sockfd, (const struct sockaddr *) &_servaddr, sizeof(_servaddr))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

int is_code_valid(int code)
{
    if (code >= P_CONN && code <= P_DISCONN)
        return 0;
    if (code >= E_SPAWN && code <= E_DMG)
        return 1;
    if (code >= T_SPAWN && code <= T_DEAD)
        return 2;
    if (code >= M_WAVE && code <= M_GOVER)
        return 3;
    return -1;
}

int manage_buffer(char *buffer, ssize_t n)
{
    std::string code = std::string(buffer).substr(0, 3);
    int i_code = atoi(code.c_str());
    int code_pro = is_code_valid(atoi(code.c_str()));
    std::vector<std::string> tokens;

    if (code_pro == -1) {
        free(buffer);
        return -1;
    }
    std::string str =
        std::string(buffer).substr(4, static_cast<size_t>(n) - 4);
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    switch (code_pro) {
        case 0: handle_player(i_code, tokens); break;
        default: break;
    }
    free(buffer);
    return 0;
}

void server::receive_message()
{
    socklen_t len;
    ssize_t n;
    char *buffer = (char *) malloc(MAXLINE * sizeof(char));

    len = sizeof(_cliaddr);
    n = recvfrom(_sockfd, (char *) buffer, MAXLINE, MSG_WAITALL,
        (struct sockaddr *) &_cliaddr, &len);

    while (buffer[n - 1] != '\n' || buffer[n - 2] != '\t') {
        n += recvfrom(_sockfd, (char *) buffer + n, MAXLINE, MSG_WAITALL,
            (struct sockaddr *) &_cliaddr, &len);
    }
    buffer[n] = '\0';
    manage_buffer(buffer, n);
    return;
}

int main()
{
    server s;
    s.create_server();

    while (true) {
        s.receive_message();
    }
}
