/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
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
        static constexpr uint16_t REFRESH_RATE = 60U;
        static constexpr uint16_t DEFAULT_UDP_PORT = 8082;
        static constexpr uint16_t DEFAULT_TCP_PORT = 8081;
        static constexpr const char *DEFAULT_IP = "127.0.0.1";

        GameInstance(Engine::Core &engineRef);
        ~GameInstance();
        void gameLoop(float deltaTime);

        bool getServerMode();

        void setupClient(const std::string &ip = DEFAULT_IP,
            uint16_t tcpPort = DEFAULT_TCP_PORT,
            uint16_t udpPort = DEFAULT_UDP_PORT);
        void setupServer(uint16_t tcpPort = DEFAULT_TCP_PORT,
            uint16_t udpPort = DEFAULT_UDP_PORT);

        // Window Utilities
        sf::RenderWindow &getWindow();

        // Texture Utilities
        void loadTexture();

        // Player Utilities
        void playEvent();
        void playerAnimations(ecs::Entity &player, std::string direction);
        void playerShoot(ecs::Entity &player);

        std::vector<ecs::Entity> &getEntities();
        int manageBuffers();
        void updateLocalPlayerPosition();
        ecs::Entity &getLocalPlayer();
        size_t getPlayerId();

        void gameUpdate(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        bool isServer() const;

        void createPlayer();

        Engine::Core &refGameEngine;
        Engine::Feature::LevelManager<GameInstance> &entityManager;
        Engine::Feature::AssetManager &assetManager;
        Engine::Feature::NetworkingManager &networkManager;

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

        //     void setServerMode(bool mode);

        //     // These functions should be moved in a game Engine class

        //     // --

        //     // Client Management
        //     void setRenderClass(RenderClass *refRender);
        //     RenderClass *getRenderClass();
        //     void loadTexture();
        //     void createPlayer();

        //     // Player Management
        //     void handlePlayer(int code, std::vector<std::string> &tokens);
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
        size_t _PlayerId;
        bool _isServer;
        uint16_t _udpPort = DEFAULT_UDP_PORT;
        uint16_t _tcpPort = DEFAULT_TCP_PORT;
        std::string _ip = DEFAULT_IP;

        std::unique_ptr<sf::RenderWindow> _window;

        // Client Sprites
        sf::Sprite _backgroundSprite;

        //     std::vector<ecs::Entity &> _entities;

        //     // To move into game Engine
        //     std::unordered_map<std::type_index, ecs::ISystem> _subsystems;
    };
}; // namespace Game

#endif /*game_HPP*/
