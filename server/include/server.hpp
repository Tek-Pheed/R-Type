/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#ifndef SERVER_HPP
#define SERVER_HPP
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>
struct server {
    int scokfd;
    struct sockaddr_in servaddr, cliaddr;
};

#endif /* SERVER_HPP */
