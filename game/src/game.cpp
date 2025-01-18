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
#include "ErrorClass.hpp"
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "LevelConfig.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "system_network.hpp"

using namespace RType;

constexpr auto BUILD_BASIC_ENEMY = "BASIC_ENEMY";
constexpr auto BUILD_SHOOTER_ENEMY = "SHOOTER_ENEMY";
constexpr auto BUILD_BOSS = "BOSS";
constexpr auto CHANGE_MUSIC = "MUSIC";
constexpr auto CHANGE_BACKGROUND = "BACKGROUND";
constexpr auto CHANGE_WAVE = "WAVE";
constexpr auto BUILD_BONUS = "BONUS";

size_t RType::getNewId()
{
    static size_t id = 0;

    id++;
    return (id);
}

sf::Sound &GameInstance::getMusicPlayer()
{
    auto songEntity =
        refEntityManager.getPersistentLevel()
            .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()[0];
    auto currentSong =
        songEntity.get().getComponent<ecs::MusicComponent<sf::Sound>>();

    return (currentSong.get()->getMusicType());
}

void GameInstance::handleNetworkMechs(
    int code, const std::vector<std::string> &tokens)
{
    if constexpr (server)
        return;
    switch (code) {
        case Protocol::M_MUSIC: {
            if (tokens.size() >= 1) {
                auto &ref =
                    refAssetManager.loadAsset("assets/sounds/" + tokens[0],
                        tokens[0], &sf::SoundBuffer::loadFromFile);
                auto &mus = getMusicPlayer();
                if (mus.getStatus() == sf::SoundSource::Playing)
                    mus.stop();
                mus.setBuffer(ref);
                mus.setVolume(GameInstance::MUSIC_VOLUME);
                mus.play();
            }
            break;
        }
        case Protocol::M_BG: {
            if (tokens.size() >= 1) {
                auto &ref =
                    refAssetManager.loadAsset("assets/background/" + tokens[0],
                        tokens[0], &sf::Texture::loadFromFile, sf::IntRect());
                auto bg = refEntityManager.getPersistentLevel()
                              .findEntitiesByComponent<
                                  ecs::BackgroundComponent>()[0];
                auto comp =
                    bg.get().getComponent<ecs::SpriteComponent<sf::Sprite>>();

                ref.setRepeated(true);
                comp->getSprite().setTextureRect(
                    sf::Rect(0, 0, (int) GameInstance::DEFAULT_RESOLUTION_X,
                        (int) GameInstance::DEFAULT_RESOLUTION_Y));
                comp->getSprite().setTexture(ref);
            }
            break;
        }
        case Protocol::M_WAVE: {
            if (tokens.size() >= 1) {
                currentWave = std::atoi(tokens[0].c_str());
                if (currentWave > 1) {
                    auto &newWaveInComingSound =
                        this->refAssetManager.getAsset<sf::SoundBuffer>(
                            Asset::NEWWAVEINCOMING);
                    factory.buildSoundEffect(
                        newWaveInComingSound, "newWaveInComingSound", 100.0f);
                }
                if (RType::GameInstance::DEBUG_LOGS)
                    std::cout << "Changing wave: " << currentWave << std::endl;
            }
            break;
        }
        default: break;
    }
}

void GameInstance::loadPvPLevel()
{
    std::unique_lock lock(_gameLock);
    std::stringstream ss;

    if constexpr (server) {
        // Change background
        ss << M_BG << " " << "clouds.jpg" << " " << PACKET_END;
        refNetworkManager.sendToAll(System::Network::ISocket::TCP, ss.str());
        ss.clear();

        // Change music
        ss << M_MUSIC << " " << "pvp.ogg" << " " << PACKET_END;
        refNetworkManager.sendToAll(System::Network::ISocket::TCP, ss.str());
        ss.clear();

        for (auto &entity : getAllPlayers()) {
            auto &player = entity.get();
            auto playerHealth = player.getComponent<ecs::HealthComponent>();
            if (playerHealth)
                playerHealth->setHealth(300);
        }
    }

    if constexpr (!server) {
        for (auto &entity : getAllPlayers()) {
            auto &player = entity.get();
            auto playerComp = player.getComponent<ecs::PlayerComponent>();
            auto playerHealth = player.getComponent<ecs::HealthComponent>();
            if (playerComp && playerComp->getTeam() == 1) {
                const float Width = 0.075f * (float) WinScaleX;
                const float Height = 0.06f * (float) WinScaleY;
                auto spriteComp =
                    player.getComponent<ecs::SpriteComponent<sf::Sprite>>();

                spriteComp->getSprite().setScale(
                    (Width / spriteComp->getSprite().getLocalBounds().width)
                        * -1,
                    Height / spriteComp->getSprite().getLocalBounds().height);
                spriteComp->getSprite().setOrigin(
                    spriteComp->getSprite().getLocalBounds().width / 2.0f,
                    spriteComp->getSprite().getLocalBounds().height / 2.0f);
            }
            if (playerHealth)
                playerHealth->setHealth(300);
        }
        auto songEntity =
            refEntityManager.getPersistentLevel()
                .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()[0];
        auto currentSong =
            songEntity.get().getComponent<ecs::MusicComponent<sf::Sound>>();
        auto &newMusic =
            refAssetManager.getAsset<sf::SoundBuffer>(Asset::PVP_SOUND);

        if (currentSong->getMusicType().getStatus()
            == sf::SoundSource::Playing) {
            currentSong->getMusicType().stop();
            currentSong->getMusicType().setBuffer(newMusic);
            currentSong->getMusicType().play();
        }
    }
}

void GameInstance::loadLevelContent(const std::string &filename)
{
    std::unique_lock lock(_gameLock);
    LevelConfig l(filename);
    auto map = l.parseLevelConfig();
    int wave = 0;

    for (auto &[key, value] : map) {
        if (key == BUILD_BASIC_ENEMY) {
            if (value.size() < 5)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create basic "
                    "enemy from level config");
            factory.buildEnemy(getNewId(), (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[4].c_str()), wave,
                (float) std::atof(value[2].c_str()),
                (float) std::atof(value[3].c_str()));
        }
        if (key == BUILD_SHOOTER_ENEMY) {
            if (value.size() < 5)
                throw ErrorClass(
                    THROW_ERROR_LOCATION "loadLevelContent: Failed to create "
                                         "shooter "
                                         "enemy from level config");
            factory.buildEnemyShooter(getNewId(),
                (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[4].c_str()), wave,
                (float) std::atof(value[2].c_str()),
                (float) std::atof(value[3].c_str()));
        }
        if (key == BUILD_BOSS) {
            if (value.size() < 3)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create boss "
                    "from level config");
            factory.buildBoss(getNewId(), (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                (float) std::atof(value[2].c_str()), wave);
        }
        if (key == CHANGE_MUSIC) {
            if (value.size() < 1)
                throw ErrorClass(
                    THROW_ERROR_LOCATION "loadLevelContent: Failed to create "
                                         "music from level config");
            std::stringstream ss;
            _musicName = value[0];
            if (RType::GameInstance::DEBUG_LOGS)
                std::cout << "Set music: " << _musicName << std::endl;
            ss << M_MUSIC << " " << value[0] << " " << PACKET_END;
            refNetworkManager.sendToAll(
                System::Network::ISocket::TCP, ss.str());
        }
        if (key == CHANGE_BACKGROUND) {
            if (value.size() < 1)
                throw ErrorClass(
                    THROW_ERROR_LOCATION "loadLevelContent: Failed to create "
                                         "background from level config");
            std::stringstream ss;
            if (RType::GameInstance::DEBUG_LOGS)
                std::cout << "Set background: " << _bgName << std::endl;
            _bgName = value[0];
            ss << M_BG << " " << value[0] << " " << PACKET_END;
            refNetworkManager.sendToAll(
                System::Network::ISocket::TCP, ss.str());
        }
        if (key == CHANGE_WAVE) {
            if (value.size() < 1)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to set wave");
            wave = std::atoi(value[0].c_str());
            if (RType::GameInstance::DEBUG_LOGS)
                std::cout << "Set wave to: " << wave << std::endl;
        }
        if (key == BUILD_BONUS && _bonus) {
            if (value.size() < 3)
                throw ErrorClass(THROW_ERROR_LOCATION
                    "loadLevelContent: Failed to create bonus "
                    "from level config");
            std::cout << "Je build un bonus de type : "
                      << std::atoi(value[2].c_str()) << std::endl;
            factory.buildBonus(getNewId(), (float) std::atof(value[0].c_str()),
                (float) std::atof(value[1].c_str()),
                static_cast<ecs::Bonus>(std::atoi(value[2].c_str())), wave);
        }
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
        if constexpr (RType::GameInstance::DEBUG_LOGS)
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

    if constexpr (!server) {
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
        if constexpr (!server) {
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
            if (time >= 1.0f) {
                for (auto entID : refEntityManager.getCurrentLevel()
                         .findEntitiesIdByComponent<ecs::EnemyComponent>()) {
                    auto enemy = refEntityManager.getCurrentLevel()
                                     .getEntityById(entID)
                                     .getComponent<ecs::EnemyComponent>();
                    if (enemy && (enemy->getType() == 1)) {
                        factory.buildBulletFromEnemy(enemy->getEnemyID());
                    }
                    if (enemy && enemy->getType() == 2) {
                        factory.buildBulletFromBoss(enemy->getEnemyID());
                    }
                }
                time = 0.0f;
            }
        }
    } catch (const std::exception &e) {
        if constexpr (RType::GameInstance::DEBUG_LOGS)
            std::cout << CATCH_ERROR_LOCATION
                "An error occured while playing: "
                      << e.what() << std::endl;
    }
}

void GameInstance::gamePostTick(
    Engine::Events::EventType event, Engine::Core &core, std::any arg)
{
    (void) event;
    (void) core;
    (void) arg;
    static bool ended = false;

    std::unique_lock lock(_gameLock);

    if constexpr (!server) {
        for (auto &entity : refEntityManager.getCurrentLevel().getEntities()) {
            auto player = entity.get().getComponent<ecs::PlayerComponent>();
            auto pText =
                entity.get().getComponent<ecs::TextComponent<sf::Text>>();
            if (player && pText) {
                if (getHostClient() == player->getPlayerID()
                    && pText->getStr().find("host") == std::string::npos) {
                    pText->setStr(pText->getStr() + " (host)");
                }
            }
        }
        for (auto &entity : refEntityManager.getCurrentLevel()
                 .findEntitiesByComponent<ecs::MusicComponent<sf::Sound>>()) {
            auto mus =
                entity.get().getComponent<ecs::MusicComponent<sf::Sound>>();
            if (mus->getMusicType().getStatus() != sf::Music::Playing)
                refEntityManager.getCurrentLevel().markEntityForDeletion(
                    entity.get().getID());
        }
        getWindow().display();
    } else {
        if (_gamemode != 1) {
            bool next = true;
            auto entVect = refEntityManager.getCurrentLevel()
                               .findEntitiesByComponent<ecs::EnemyComponent>();
            if (ended || !_gameStarted) {
                return;
            }
            for (auto &ent : entVect) {
                if (ent.get().getComponent<ecs::EnemyComponent>()->getWave()
                    <= currentWave) {
                    next = false;
                    break;
                }
            }
            if (next) {
                if (entVect.size() == 0 && !ended) {
                    if (_level < getTxtFiles("assets/levels").size()) {
                        if (RType::GameInstance::DEBUG_LOGS)
                            std::cout << "Next level" << std::endl;
                        _level++;
                        std::string levelFileName = "assets/levels/level"
                            + std::to_string(_level) + ".txt";
                        loadLevelContent(levelFileName);
                        currentWave = -1;
                    } else {
                        ended = true;
                        std::cout << "Congrats ! You played all levels :)"
                                  << std::endl;
                        std::cout << "Congrats ! You played all levels :)"
                                  << std::endl;
                        std::cout << "Congrats ! You played all levels :)"
                                  << std::endl;
                        std::cout << "Congrats ! You played all levels :)"
                                  << std::endl;
                        refGameEngine.stop();
                    }
                }
                std::stringstream ss;
                currentWave += 1;
                if (RType::GameInstance::DEBUG_LOGS)
                    std::cout << "Changing wave: " << currentWave << std::endl;
                ss << M_WAVE << " " << currentWave << PACKET_END;
                refNetworkManager.sendToAll(
                    System::Network::ISocket::TCP, ss.str());
            }
        }
    }
}

void RType::GameInstance::resetGame()
{
    _maxPlayers = DEFAULT_MAX_PLAYERS;
    _difficulty = DEFAULT_DIFFICULTY;
    _bonus = true;
    _level = 1;
    _gamemode = 0;
    _playerEntityID = -1;
    _netClientID = -1;
    _isConnectedToServer = false;
    _gameStarted = false;
    _udpPort = DEFAULT_UDP_PORT;
    _tcpPort = DEFAULT_TCP_PORT;
    _ip = DEFAULT_IP;
    _ticks = 0U;
    _lastNetTick = 0U;
    _clientGameMasterId = -1;
    _healthId = -1;
}

int RType::GameInstance::manageBuffers()
{
    if (!isServer() && !_isConnectedToServer)
        return (-1);
    auto packets = refNetworkManager.readAllPackets();
    if (packets.size() == 0)
        return 0;

    for (auto &buff : packets) {
        try {
            std::string buffer = buff;
            std::string codeStr = buffer.substr(0, 3);
            int code = atoi(codeStr.c_str());
            int code_int = isCodeValid(code);
            std::vector<std::string> tokens;
            if (code_int == -1) {
                if (RType::GameInstance::DEBUG_LOGS)
                    std::cout << "Invalid packet: " << buffer << std::endl;
                return -1;
            }
            std::string str = buffer.substr(4, buffer.size() - 4);
            std::istringstream ss(str);
            std::string token;
            if (RType::GameInstance::DEBUG_LOGS)
                std::cout << "Managing Buffer: " << buff << std::endl;
            while (std::getline(ss, token, ' ')) {
                tokens.push_back(token);
            }
            switch (code_int) {
                case 0: handleNetworkPlayers(code, tokens); break;
                case 1: handleNetworkEnemies(code, tokens); break;
                case 3: handleNetworkMechs(code, tokens); break;
                case 4: handleNetworkBonuses(code, tokens); break;
                case 24: handleLobby(code, tokens); break;
                case 9:
                    if constexpr (server) {
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
        } catch (const std::exception &e) {
            std::cout << CATCH_ERROR_LOCATION "Failed to handle packet: "
                      << buff << " : " << e.what() << std::endl;
            continue;
        }
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
      gameConfig(Config(getConfigPath())), factory(Factory(*this)),
      levels(Levels(*this))
{
}

GameInstance::~GameInstance()
{
    refNetworkManager.stopNetworking();
    refGameEngine.stop();
}

std::vector<ecs::Entity> &RType::GameInstance::getEntities()
{
    return (refEntityManager.getCurrentLevel().getEntitiesVect());
}

uint64_t GameInstance::getTicks() const
{
    return _ticks;
}

size_t GameInstance::getDifficulty() const
{
    return _difficulty;
}

size_t GameInstance::getGameMode() const
{
    return _gamemode;
}

void GameInstance::setGameMode(size_t mode)
{
    _gamemode = mode;
}

size_t GameInstance::getMaxPlayers() const
{
    return _maxPlayers;
}
