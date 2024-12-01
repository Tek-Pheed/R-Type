/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system
*/

#ifndef SYSTEM_NETWORK_HPP
#define SYSTEM_NETWORK_HPP

#include <string>
#include "system.hpp"
#include "system_tcp.hpp"

namespace System
{
    namespace Network
    {
        class NetworkException : public std::exception {
          public:
            explicit NetworkException(const std::string &str);
            const char *what() const noexcept override;

          private:
            std::string _content;
        };
        class TCPClient;
    } // namespace Network
} // namespace System

#endif /* SYSTEM_NETWORK_HPP */
