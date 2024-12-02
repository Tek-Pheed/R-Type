/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** system_dp
*/

#ifndef SYSTEM_UDP_HPP
#define SYSTEM_UDP_HPP

#include <string>
#include <cstdint>
#include <optional>
#include <string>
#include "system.hpp"

#if defined(LINUX)
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(WIN32)
    #include <winsock2.h>
#else
    #error "This target is not supported by R-Type libSystem"
#endif

namespace System
{
    namespace Network
    {

    }
} // namespace System

#endif /* SYSTEM_UDP_HPP */
