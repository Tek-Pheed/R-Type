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

#if defined(RTYPE_SERVER)
constexpr const bool server = true;
#else
constexpr const bool server = false;
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "Config.hpp"
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "Factory.hpp"
#include "GameSystems.hpp"
#include "Levels.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <unordered_map>

namespace RType
{
    class GameInstance {
      public:
#ifdef NDEBUG
        static constexpr bool DEBUG_LOGS = false;
#else
        static constexpr bool DEBUG_LOGS = true;
#endif
        static constexpr const char *USER_CONFIG_FILE = "config.cfg";
        static constexpr uint16_t CLIENT_REFRESH_RATE = 60U;
        static constexpr uint16_t SERVER_REFRESH_RATE = 60U;
        static constexpr uint16_t DEFAULT_UDP_PORT = 8082;
        static constexpr uint16_t DEFAULT_TCP_PORT = 8081;
        static constexpr const char *DEFAULT_IP = "127.0.0.1";
        static constexpr size_t DEFAULT_MAX_PLAYERS = 4U;
        static constexpr size_t DEFAULT_DIFFICULTY = 1U;

        static constexpr size_t DEFAULT_RESOLUTION_X = 1280U;
        static constexpr size_t DEFAULT_RESOLUTION_Y = 720U;

        static constexpr auto KILLZONE = -0.1f;
        static constexpr auto ENEMY_VELOCITY = -0.156f;
        static constexpr auto ENEMY_SHOOTER_VELOCITY = -0.117f;

        static constexpr float MUSIC_VOLUME = 18.0f;
        static constexpr float BULLET_VOLUME = 5.0f;
        static constexpr bool isServer()
        {
            return (server);
        };

        GameInstance(Engine::Core &engineRef);
        ~GameInstance();

        void resetGame();

        size_t WinScaleX = 1;
        size_t WinScaleY = 1;

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

        void createPersistentLevel();
        void handleConfigButtons(sf::Keyboard::Key pressedKey, int actionType);
        void handleAutoFireButton(
            std::string newAutoFireValue, ecs::Entity &entity);
        void handleResolutionButton(ecs::Entity &entity);
        void handleInputButtons(const std::vector<sf::Keyboard::Key> &keys);

        void loadLevelContent(const std::string &filename);
        void loadPvPLevel();

        // Player functions and utilities
        ecs::Entity &getRandomPlayer(void);
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
        int getHealthId();
        void setHealthId(int id);

        int countTxtFiles(const std::string &path);

        sf::Sound &getMusicPlayer();

        std::vector<std::string> getTxtFiles(const std::string &path);

        std::vector<ecs::Entity> &getEntities();

        std::string playerName;

        int nbTxtFiles;

        // Enemies
        ecs::Entity &getEnemyById(size_t enemyID);
        void sendEnemyPosition(size_t enemyID);
        void deleteEnemy(size_t enemyID);
        void handleNetworkEnemies(
            int code, const std::vector<std::string> &tokens);
        void handleNetworkMechs(
            int code, const std::vector<std::string> &tokens);

        // Boss
        ecs::Entity &getBossById(size_t bossID);
        void sendBossPosition(size_t bossID);
        void deleteBoss(size_t bossID);
        void handleNetworkBosses(
            int code, const std::vector<std::string> &tokens);
        void clientHandleDisconnected(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);

        // Networking
        int isCodeValid(int code);
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
        void handleLobby(int code, const std::vector<std::string> &tokens);
        void launchGame();
        size_t getHostClient();

        // Server Only Events
        void serverEventNewConn(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void serverEventClosedConn(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void serverEventPackets(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);

        // Events
        void gamePreTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void gameTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);
        void gamePostTick(
            Engine::Events::EventType event, Engine::Core &core, std::any arg);

        // Bonus
        void handleNetworkBonuses(
            int code, const std::vector<std::string> &tokens);
        ecs::Entity &getBonusById(size_t bonusID);
        void applyBonus(ecs::Entity &bonus, size_t playerID, int bonusType);

        Engine::Core &refGameEngine;
        Engine::Feature::LevelManager<GameInstance> &refEntityManager;
        Engine::Feature::AssetManager &refAssetManager;
        Engine::Feature::NetworkingManager &refNetworkManager;

        Config gameConfig;

        bool isSettingsUpButtonClicked = false;
        bool isSettingsRightButtonClicked = false;
        bool isSettingsLeftButtonClicked = false;
        bool isSettingsDownButtonClicked = false;

        bool isSettingsNicknameButtonClicked = false;
        std::vector<sf::Keyboard::Key> nicknameKeys;

        // MENU
        std::vector<ecs::Entity> buttonList;
        size_t lastButtonIdClicked;

        std::vector<ecs::Entity> inputList;
        size_t lastInputIdClicked;

        sf::Sound currentMusic;

        // Ticks
        uint64_t getTicks() const;
        int currentWave = 0;

        // Game Value
        size_t getDifficulty() const;
        size_t getGameMode() const;
        void setGameMode(size_t mode);
        size_t getMaxPlayers() const;

        Factory factory;
        Levels levels;

      private:
        size_t _maxPlayers = DEFAULT_MAX_PLAYERS;
        size_t _difficulty = DEFAULT_DIFFICULTY;
        bool _bonus = true;
        size_t _level = 1;
        // 0 for wave, 1 for pvp
        size_t _gamemode = 0;
        int _playerEntityID = -1;
        ssize_t _netClientID = -1;
        bool _isConnectedToServer = false;
        bool _gameStarted = false;
        uint16_t _udpPort = DEFAULT_UDP_PORT;
        uint16_t _tcpPort = DEFAULT_TCP_PORT;
        std::string _ip = DEFAULT_IP;

        std::string _musicName;
        std::string _bgName;

        uint64_t _ticks = 0U;
        uint64_t _lastNetTick = 0U;
        ssize_t _clientGameMasterId = -1;
        std::unique_ptr<sf::RenderWindow> _window;
        std::recursive_mutex _gameLock;
        std::unordered_map<size_t, uint64_t> _clientTicks;
        sf::Clock _fireClock;
        int _healthId = -1;
        sf::Clock _bonusClock;
    };

}; // namespace RType
