// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** server
// */

// #include <chrono>
// #include <ctime>
// #include <memory>
// #include <vector>
// #include "Entity.hpp"
// #include "Networking.hpp"
// #include "RenderClass.hpp"
// #include "game.hpp"
// #include "system_network.hpp"
// #include "server.hpp"

// void serverLoop(game &game)
// {
//     auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(
//         std::chrono::system_clock::now());
//     auto last = std::chrono::time_point_cast<std::chrono::milliseconds>(
//         std::chrono::system_clock::now());

//     while (true) {
//         now = std::chrono::time_point_cast<std::chrono::milliseconds>(
//             std::chrono::system_clock::now());
//         auto next = std::chrono::time_point_cast<std::chrono::milliseconds>(
//             std::chrono::system_clock::now());
//         next += std::chrono::milliseconds(1000 / SERVER_TPS);
//         auto deltaTimeMs =
//             std::chrono::duration_cast<std::chrono::milliseconds>(now - last)
//                 .count();
//         last = now;
//         game.gameUpdate();
//         std::this_thread::sleep_until(next);
//     }
// }