/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include "Game.hpp"
#include <any>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <sstream>
#include <vector>
#include "Components.hpp"
#include "Config.hpp"
#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineLevelManager.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameEvents.hpp"
#include "GameSystems.hpp"
#include "LevelConfig.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

using namespace RType;

constexpr auto BUILD_BASIC_ENEMY = "BASIC_ENEMY";
constexpr auto BUILD_SHOOTER_ENEMY = "SHOOTER_ENEMY";
constexpr auto BUILD_BOSS = "BOSS";
// constexpr auto CHANGE_MUSIC = "MUSIC";

size_t RType::getNewId()
{
    static size_t id = 0;

    id++;
    return (id);
}

void GameInstance::loadLevelContent(const std::string &filename)
{
    std::unique_lock lock(_gameLock);
    LevelConfig l(filename);
    auto map = l.parseLevelConfig();

    for (auto &[key, value] : map) {
        if (key == BUILD_BASIC_ENEMY) {
            if (value.size() < 3)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create basic "
                    "enemy from level config");
            _factory.buildEnemy(getNewId(),
                (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[2].c_str()));
        }
        if (key == BUILD_SHOOTER_ENEMY) {
            if (value.size() < 3)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create shooter "
                    "enemy from level config");
            _factory.buildEnemyShooter(getNewId(),
                (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[2].c_str()));
        }
        if (key == BUILD_BOSS) {
            if (value.size() < 3)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create boss "
                    "from level config");
            _factory.buildBoss(getNewId(), (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[2].c_str()));
        }
        // if (key == CHANGE_MUSIC) {
        //     if (value.size() < 1)
        //         throw ErrorClass("loadLevelContent: Failed to create music
        //         from level config");

        // }
    }
}

const std::vector<const Asset::AssetStore *> getAllAsset()
{
    std::vector<const Asset::AssetStore *> vect;

    for (size_t i = 0; i < sizeof(Asset::assets) / sizeof(Asset::assets[0]);
         i++) {
        vect.emplace_back(&Asset::assets[i]);
    }
    return (vect);
}

void GameInstance::loadAssets()
{
    std::unique_lock lock(_gameLock);

    try {
        for (const auto asset : Asset::getAllAssetsOfType<sf::Texture>()) {
            refAssetManager.loadAsset(asset->path, asset->identifier,
                &sf::Texture::loadFromFile, sf::IntRect());
        }
        for (const auto asset : Asset::getAllAssetsOfType<sf::Font>()) {
            refAssetManager.loadAsset(
                asset->path, asset->identifier, &sf::Font::loadFromFile);
        }
        for (const auto asset : Asset::getAllAssetsOfType<sf::SoundBuffer>()) {
            refAssetManager.loadAsset(asset->path, asset->identifier,
                &sf::SoundBuffer::loadFromFile);
        }
    } catch (const std::exception &e) {
        std::cout << CATCH_ERROR_LOCATION
            "Failed to an load asset with error: "
                  << e.what() << std::endl;
    }
}

void GameInstance::gamePreTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;
    std::unique_lock lock(_gameLock);

    if (!isServer()) {
        getWindow().clear();
    }
}

void GameInstance::gameTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) core;
    (void) event;
    (void) arg;

    std::unique_lock lock(_gameLock);
    _ticks++;
    try {
        manageBuffers();
        if (!_isServer) {
            playEvent();
            for (auto &pl : getAllPlayers()) {
                playerAnimations(pl.get());
            }
        } else {
            if (!_gameStarted)
                return;
            float deltaTime_sec = std::any_cast<float>(arg);
            static float time = 0.0f;
            time += deltaTime_sec;
            if (time >= 2.0f) {
                for (auto entID :
                    refEntityManager.getCurrentLevel()
                        .findEntitiesIdByComponent<ecs::EnemyComponent>()) {
                    auto enemy = refEntityManager.getCurrentLevel()
                                     .getEntityById(entID)
                                     .getComponent<ecs::EnemyComponent>();
                    if (enemy && (enemy->getType() == 1)) {
                        _factory.buildBulletFromEnemy(enemy->getEnemyID());
                    }
                    if (enemy && enemy->getType() == 2) {
                        _factory.buildBulletFromBoss(enemy->getEnemyID());
                    }
                }
                time = 0.0f;
            }
        }
    } catch (const std::exception &e) {
        std::cout << THROW_ERROR_LOCATION "An error occured while playing: "
                  << e.what() << std::endl;
    }
}

void GameInstance::gamePostTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;

    std::unique_lock lock(_gameLock);

    if (!isServer()) {
        getWindow().display();
    }
}

int RType::GameInstance::manageBuffers()
{
    if (!isServer() && !_isConnectedToServer)
        return (-1);
    auto packets = refNetworkManager.readAllPackets();
    if (packets.size() == 0)
        return 0;

    for (auto &buff : packets) {
        std::string buffer = buff;
        std::string codeStr = buffer.substr(0, 3);
        int code = atoi(codeStr.c_str());
        int code_int = is_code_valid(code);
        std::vector<std::string> tokens;
        if (code_int == -1) {
            std::cout << "Invalid packet: " << buffer << std::endl;
            return -1;
        }
        std::string str = buffer.substr(4, buffer.size() - 4);
        std::istringstream ss(str);
        std::string token;
        std::cout << "Managing Buffer: " << buff << std::endl;
        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }
        switch (code_int) {
            case 0: handleNetworkPlayers(code, tokens); break;
            case 1: handleNetworkEnemies(code, tokens); break;
            // case 2: handle_terrain(code, tokens); break;
            // case 3: handle_mechs(code, tokens); break;
            case 24: handleLobby(code, tokens); break;
            case 9:
                if (isServer()) {
                    serverHanlderValidateConnection(code, tokens);
                } else {
                    clientHandlerConnection(code, tokens);
                }
                break;
            default: {
                std::cout << "Invalid packet: " << buffer << std::endl;
                break;
            }
        }
        ss.clear();
    }
    return 0;
}

static std::string getConfigPath()
{
    const char *appImagePath = std::getenv("APPIMAGE");

    if (appImagePath == NULL) {
        return (GameInstance::USER_CONFIG_FILE);
    } else {
        std::string fullAppImagePath = appImagePath;
        size_t lastSlashPos = fullAppImagePath.find_last_of("/");
        if (lastSlashPos != std::string::npos) {
            std::string appImageDir = fullAppImagePath.substr(0, lastSlashPos);
            return (appImageDir + "/" + GameInstance::USER_CONFIG_FILE);
        } else {
            return (GameInstance::USER_CONFIG_FILE);
        }
    }
}

GameInstance::GameInstance(Engine::Core &engineRef)
    : refGameEngine(engineRef),
      refEntityManager(
          engineRef.getFeature<Engine::Feature::LevelManager<GameInstance>>()),
      refAssetManager(engineRef.getFeature<Engine::Feature::AssetManager>()),
      refNetworkManager(
          engineRef.getFeature<Engine::Feature::NetworkingManager>()),
      _gameConfig(Config(getConfigPath())), _factory(Factory(*this))
{
}

GameInstance::~GameInstance()
{
    refNetworkManager.stopNetworking();
    refGameEngine.stop();
}

bool GameInstance::getServerMode()
{
    return _isServer;
}

bool GameInstance::isServer() const
{
    return (_isServer);
}

std::vector<ecs::Entity> &RType::GameInstance::getEntities()
{
    return (refEntityManager.getCurrentLevel().getEntitiesVect());
}

uint64_t GameInstance::getTicks() const
{
    return _ticks;
}
