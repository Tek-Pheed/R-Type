/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "Factory.hpp"
#include "GameSystems.hpp"
#include <unordered_map>

namespace RType
{
    class GameInstance {
      public:
        static constexpr uint16_t CLIENT_REFRESH_RATE = 60U;
        static constexpr uint16_t SERVER_REFRESH_RATE = 60U;
        static constexpr uint16_t DEFAULT_UDP_PORT = 8082;
        static constexpr uint16_t DEFAULT_TCP_PORT = 8081;
        static constexpr const char *DEFAULT_IP = "127.0.0.1";
        static constexpr size_t DEFAULT_MAX_PLAYERS = 4U;

        static constexpr size_t RESOLUTION_X = 1280U;
        static constexpr size_t RESOLUTION_Y = 720U;

        static constexpr auto KILLZONE = -100.0f;
        static constexpr auto ENEMY_VELOCITY = -200.0f;
        static constexpr auto ENEMY_SHOOTER_VELOCITY = -150.0f;

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

        // Server state
        bool isConnectedToServer();
        ssize_t getNetClientID();

        // Texture Utilities
        void loadAssets();

        ecs::Entity &buildBackground();
        ecs::Entity &buildButton(std::string text, int buttonNb);
        ecs::Entity &buildInput(std::string str, int buttonID);
        void levelMainMenu();
        void createPersistentLevel();
        void levelSettingsMenu();
        void levelContinueMenu();
        void handleConfigButtons(sf::Keyboard::Key pressedKey, int actionType);
        void handleAutoFireButton(
            std::string newAutoFireValue, ecs::Entity &entity);
        void handleInputButtons(const std::vector<sf::Keyboard::Key> &keys);

        void loadLevel(const std::string &filename);

        // Player functions and utilities
        std::vector<std::reference_wrapper<ecs::Entity>> getAllPlayers();
        bool hasLocalPlayer(void) const;
        ecs::Entity &getLocalPlayer();
        ecs::Entity &getPlayerById(size_t id);
        void updatePlayerPosition(size_t playerID, float newX, float newY);
        void sendPlayerPosition(size_t playerID);
        void deletePlayer(size_t playerID);
        void playerAnimations(ecs::Entity &player);
        void setPlayerEntityID(int id);
        void damagePlayer(size_t playerID, int damage);
        size_t getHealthId();
        void setHealthId(size_t id);

        std::vector<ecs::Entity> &getEntities();

        std::string _playerName;

        // Enemies
        ecs::Entity &buildEnemy(
            size_t id, float posX, float posY, float health);

        ecs::Entity &buildEnemyShooter(
            size_t id, float posX, float posY, float health);

        ecs::Entity &getEnemyById(size_t enemyID);
        void sendEnemyPosition(size_t enemyID);
        void deleteEnemy(size_t enemyID);
        void handleNetworkEnemies(
            int code, const std::vector<std::string> &tokens);

        // Boss
        ecs::Entity &buildBoss(
            size_t id, float posX, float posY, float health = 100.0f);
        ecs::Entity &getBossById(size_t bossID);
        void sendBossPosition(size_t bossID);
        void deleteBoss(size_t bossID);
        void handleNetworkBosses(
            int code, const std::vector<std::string> &tokens);
        void clientHandleDisconnected(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);

        // Networking
        int is_code_valid(int code);
        int manageBuffers();
        void connectToGame();
        void clientStartLevel();
        void clientHandlerConnection(
            int code, const std::vector<std::string> &tokens);
        void serverHanlderValidateConnection(
            int code, const std::vector<std::string> &tokens);
        void handleNetworkPlayers(
            int code, const std::vector<std::string> &tokens);
        void serverSendGameState(size_t clientID);
        void handleLoby(int code, const std::vector<std::string> &tokens);

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

        bool _isSettingsUpButtonClicked = false;
        bool _isSettingsRightButtonClicked = false;
        bool _isSettingsLeftButtonClicked = false;
        bool _isSettingsDownButtonClicked = false;

        bool _isSettingsNicknameButtonClicked = false;
        std::vector<sf::Keyboard::Key> _nicknameKeys;
        ecs::Entity *_nicknameInputEntity = nullptr;

        // MENU
        std::vector<ecs::Entity> _buttonList;
        size_t _lastButtonIdClicked;

        std::vector<ecs::Entity> _inputList;
        size_t _lastInputIdClicked;

        sf::Sound _currentMusic;

        // Ticks
        uint64_t getTicks() const;

      private:
        size_t _maxPlayers = DEFAULT_MAX_PLAYERS;
        int _playerEntityID = -1;
        ssize_t _netClientID = -1;
        bool _isServer;
        bool _isConnectedToServer = false;
        bool _gameStarted = false;
        uint16_t _udpPort = DEFAULT_UDP_PORT;
        uint16_t _tcpPort = DEFAULT_TCP_PORT;
        std::string _ip = DEFAULT_IP;

        Factory _factory;

        uint64_t _ticks = 0U;
        uint64_t _lastNetTick = 0U;
        ssize_t _clientGameMasterId = -1;
        std::unique_ptr<sf::RenderWindow> _window;
        std::recursive_mutex _gameLock;
        std::unordered_map<size_t, uint64_t> _clientTicks;
        sf::Clock _autoFireClock;
        size_t _healthId;
    };
}; // namespace RType
