/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "GameSystems.hpp"

namespace RType
{

    class GameInstance {
      public:
        static constexpr uint16_t CLIENT_REFRESH_RATE = 60U;
        static constexpr uint16_t SERVER_REFRESH_RATE = 30U;
        static constexpr uint16_t DEFAULT_UDP_PORT = 8082;
        static constexpr uint16_t DEFAULT_TCP_PORT = 8081;
        static constexpr const char *DEFAULT_IP = "127.0.0.1";

        GameInstance(Engine::Core &engineRef);
        ~GameInstance();

        bool getServerMode();

        void setupClient(const std::string &ip = DEFAULT_IP,
            uint16_t tcpPort = DEFAULT_TCP_PORT,
            uint16_t udpPort = DEFAULT_UDP_PORT);
        void setupServer(uint16_t tcpPort = DEFAULT_TCP_PORT,
            uint16_t udpPort = DEFAULT_UDP_PORT);

        // Window Utilities
        sf::RenderWindow &getWindow();
        void playEvent();

        // Texture Utilities
        void loadTexture();

        ecs::Entity &buildBackground(void);
        void levelMainMenu(void);
        void createPersistentLevel(void);

        // Player functions and utilities
        ecs::Entity &buildPlayer(bool isLocalPlayer = true, size_t id = 0);
        std::vector<std::reference_wrapper<ecs::Entity>> getAllPlayers();
        bool hasLocalPlayer(void) const;
        ecs::Entity &getLocalPlayer();
        void updateLocalPlayerPosition();
        void playerAnimations(ecs::Entity &player, std::string direction);
        void playerShoot(ecs::Entity &player);

        std::vector<ecs::Entity> &getEntities();

        // Networking
        int is_code_valid(int code);
        int clientManageBuffers();
        int serverManageBuffers();
        void connectToGame();
        void clientHandlerConnection(
            int code, const std::vector<std::string> &tokens);
        void serverHanlderValidateConnection(int code, const std::vector<std::string> &tokens);

        // Server Only Events
        void serverEventNewConn(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void serverEventClosedConn(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void serverEventPackets(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);

        void gamePreTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void gameTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void gamePostTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        bool isServer() const;

        Engine::Core &refGameEngine;
        Engine::Feature::LevelManager<GameInstance> &refEntityManager;
        Engine::Feature::AssetManager &refAssetManager;
        Engine::Feature::NetworkingManager &refNetworkManager;

        //     int createConnection(const char *ip, int portTCP, int portUDP);
        //     void writeToServer(
        //         const std::string &data, System::Network::ISocket::Type
        //         socketType);
        //     void receiveMessage();

        //     void handleConnection(int code, std::vector<std::string>
        //     &tokens); void handleEnemy(int code, std::vector<std::string>
        //     &tokens); void handleTerrain(int code, std::vector<std::string>
        //     &tokens); void handleMechs(int code, std::vector<std::string>
        //     &tokens); void addEntity(ecs::Entity & entity);

        //     // Player Management
        //     void createNewPlayer(std::vector<std::string> &tokens);
        //     void setNewPosition(std::vector<std::string> &tokens);
        //     void playerDead(std::vector<std::string> &tokens);
        //     void createProjectile(std::vector<std::string> &tokens);
        //     void setPlayerHealth(std::vector<std::string> &tokens);
        //     void playerDisconnection(std::vector<std::string> &tokens);

        //     // Enemy Management
        //     void createEnemy(std::vector<std::string> &tokens);
        //     void enemyDead(std::vector<std::string> &tokens);
        //     void enemyShoot(std::vector<std::string> &tokens);
        //     void enemyDamage(std::vector<std::string> &tokens);

      private:
        int _playerEntityID = -1;
        ssize_t _netClientID = -1;
        bool _isServer;
        bool _isConnectedToServer = false;
        uint16_t _udpPort = DEFAULT_UDP_PORT;
        uint16_t _tcpPort = DEFAULT_TCP_PORT;
        std::string _ip = DEFAULT_IP;

        std::unique_ptr<sf::RenderWindow> _window;
    };
}; // namespace RType

#endif /* GAME_HPP */
