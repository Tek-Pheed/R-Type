/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Events.hpp"
#include "Factory.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "LevelConfig.hpp"
#include "system_network.hpp"

using namespace RType;

void GameInstance::handleLobby(
    int code, const std::vector<std::string> &tokens)
{
    EventManager eventManager(*this);
    if (RType::GameInstance::DEBUG_LOGS)
        std::cout << "Handling lobby: " << code << std::endl;
    switch (code) {
        case Protocol::L_STARTGAME: {
            _gameStarted = true;
            if (_isServer) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                std::stringstream sss;
                sss << L_STARTGAME << " " << id << PACKET_END;
                refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::TCP, sss.str());
                std::string levelFileName =
                    "assets/levels/level" + std::to_string(_level) + ".txt";
                ;
                if (_gamemode == 0)
                    loadLevelContent(levelFileName);
                else
                    loadPvPLevel();

            } else {
                auto songEntity = refEntityManager.getPersistentLevel()
                                      .findEntitiesByComponent<
                                          ecs::MusicComponent<sf::Sound>>()[0];
                auto currentSong =
                    songEntity.get()
                        .getComponent<ecs::MusicComponent<sf::Sound>>();
                auto &newMusic = refAssetManager.getAsset<sf::SoundBuffer>(
                    Asset::GAME_SONG);

                if (currentSong->getMusicType().getStatus()
                    == sf::SoundSource::Playing) {
                    currentSong->getMusicType().stop();
                    currentSong->getMusicType().setBuffer(newMusic);
                    currentSong->getMusicType().play();
                }
                if (_gamemode == 1)
                    loadPvPLevel();
                launchGame();
            }
            break;
        }
        case Protocol::L_SETMAXPLAYRS: {
            if (tokens.size() >= 2) {
                _maxPlayers = (size_t) atoi(tokens[1].c_str());
                auto enti = refEntityManager.getCurrentLevel().getEntities();
                std::cout << "Max players set to : " << _maxPlayers
                          << std::endl;
                if (!_isServer)
                    for (auto &entity : enti) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        if (text
                            && text->getStr().find("NUMBER OF PLAYER")
                                != std::string::npos) {
                            std::stringstream ss;
                            ss << "NUMBER OF PLAYER : " << _maxPlayers;
                            text->setStr(ss.str());
                        }
                    }
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_SETMAXPLAYRS << " " << id << " " << _maxPlayers
                        << " " << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
            default: break;
        }
        case Protocol::L_SETDIFFICULTY: {
            if (tokens.size() >= 2) {
                _difficulty = (size_t) atoi(tokens[1].c_str());
                auto enti = refEntityManager.getCurrentLevel().getEntities();
                if (!_isServer)
                    for (auto &entity : enti) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        if (text
                            && text->getStr().find("DIFFICULTY")
                                != std::string::npos) {
                            std::stringstream ss;
                            ss << "DIFFICULTY : "
                               << (_difficulty == 3          ? "HARD"
                                          : _difficulty == 2 ? "MEDIUM"
                                                             : "EASY");
                            text->setStr(ss.str());
                        }
                    }
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_SETDIFFICULTY << " " << id << " " << _difficulty
                        << " " << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
        }
        case Protocol::L_SETBONUS: {
            if (tokens.size() >= 2) {
                _bonus = (bool) atoi(tokens[1].c_str());
                auto enti = refEntityManager.getCurrentLevel().getEntities();
                if (!_isServer)
                    for (auto &entity : enti) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        if (text
                            && text->getStr().find("BONUS")
                                != std::string::npos) {
                            std::stringstream ss;
                            ss << "BONUS : " << (_bonus ? "YES" : "NO");
                            text->setStr(ss.str());
                        }
                    }
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_SETBONUS << " " << id << " " << _bonus << " "
                        << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
        }
        case Protocol::L_SETLEVEL: {
            if (tokens.size() >= 2) {
                _level = (size_t) atoi(tokens[1].c_str());
                auto enti = refEntityManager.getCurrentLevel().getEntities();
                if (!_isServer)
                    for (auto &entity : enti) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        if (text
                            && text->getStr().find("LEVEL")
                                != std::string::npos) {
                            std::stringstream ss;
                            ss << "LEVEL : " << _level;
                            text->setStr(ss.str());
                        }
                    }
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_SETLEVEL << " " << id << " " << _level << " "
                        << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
        }
        case Protocol::L_GAMEMODE: {
            if (tokens.size() >= 2) {
                _gamemode = (size_t) atoi(tokens[1].c_str());
                auto enti = refEntityManager.getCurrentLevel().getEntities();
                if (!_isServer)
                    for (auto &entity : enti) {
                        auto text =
                            entity.get()
                                .getComponent<ecs::TextComponent<sf::Text>>();
                        if (text
                            && text->getStr().find("GAMEMODE")
                                != std::string::npos) {
                            std::stringstream ss;
                            ss << "GAMEMODE : "
                               << (_gamemode ? "PVP" : "WAVE");
                            text->setStr(ss.str());
                        }
                    }
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_GAMEMODE << " " << id << " " << _gamemode << " "
                        << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
        }
        case Protocol::L_SENDLEVELS: {
            if (tokens.size() >= 2) {
                if (!_isServer) {
                    this->_nbTxtFiles = std::atoi(tokens[0].c_str());
                }
            }
        }
    }
}

void GameInstance::handleNetworkPlayers(
    int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::P_CONN: {
            if (tokens.size() >= 4) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                std::shared_ptr<ecs::PositionComponent> pos;
                if (isServer()) {
                    auto &pl = _factory.buildPlayer(true, id, tokens[3]);
                    pos = pl.getComponent<ecs::PositionComponent>();
                } else {
                    auto &pl = _factory.buildPlayer(false, id, tokens[3]);
                    pos = pl.getComponent<ecs::PositionComponent>();
                }
                updatePlayerPosition(id, (float) std::atof(tokens[1].c_str()),
                    (float) std::atof(tokens[2].c_str()));
            }
            break;
        }
        case Protocol::P_POS: {
            if (tokens.size() >= 4) {
                uint64_t tick = (uint64_t) atoi(tokens[0].c_str());
                size_t id = (size_t) atoi(tokens[1].c_str());
                if (!isServer()) {
                    if (tick < _lastNetTick) {
                        return;
                    } else {
                        _lastNetTick = tick;
                    }
                } else {
                    if (tick < _clientTicks[id]) {
                        return;
                    } else {
                        _clientTicks[id] = tick;
                    }
                }
                auto &player = getPlayerById(id);
                auto pos = player.getComponent<ecs::PositionComponent>();
                pos->setX((float) std::atof(tokens[2].c_str()));
                pos->setY((float) std::atof(tokens[3].c_str()));
                if (isServer()) {
                    std::stringstream ss;
                    ss << P_POS << " " << _ticks << " "
                       << player.getComponent<ecs::PlayerComponent>()
                              ->getPlayerID()
                       << " " << pos->getX() << " " << pos->getY()
                       << PACKET_END;
                    refNetworkManager.sendToOthers(
                        (size_t) std::atoi(tokens[1].c_str()),
                        System::Network::ISocket::Type::UDP, ss.str());
                }
            }
            break;
        }
        case Protocol::P_DEAD: {
            if (!_isServer && getLocalPlayer().getComponent<ecs::PlayerComponent>()->getPlayerID() == (size_t) atoi(tokens[0].c_str())) {
                auto &youLoseSound = refAssetManager.getAsset<sf::SoundBuffer>(Asset::YOU_LOSE_SOUND);
                _factory.buildSoundEffect(youLoseSound, "youLoseSound", 100.0f);
                std::string title = "YOU ARE DEAD";
                auto textWidth = title.size() * 20;
                _factory.buildText(0, (float)getWindow().getSize().x / 2 - (float)textWidth,
                    (float)getWindow().getSize().y / 2 - 50, title, sf::Color::Red, 100);
            }
            if (_isServer)
                return;
            if (tokens.size() >= 1) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                deletePlayer(id);
            }
            break;
        }
        case Protocol::P_SHOOT: {
            if (tokens.size() >= 1) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                std::unique_lock lock(_gameLock);
                _factory.buildBulletFromPlayer(id);
                break;
            }
            break;
        }
        case Protocol::P_DMG: {
            if (tokens.size() >= 3) {
                uint64_t tick = (uint64_t) atoll(tokens[0].c_str());
                size_t id = (size_t) atoi(tokens[1].c_str());
                if (!_isServer) {
                    if (tick < _lastNetTick) {
                        return;
                    } else {
                        _lastNetTick = tick;
                    }
                } else {
                    if (tick < _clientTicks[id]) {
                        return;
                    } else {
                        _clientTicks[id] = tick;
                    }
                }
                int health = atoi(tokens[2].c_str());
                auto &player = getPlayerById(id);
                auto healthComp = player.getComponent<ecs::HealthComponent>();
                if (healthComp) {
                    damagePlayer(id, healthComp->getHealth() - health);
                }
                if (!isServer()
                    && player.getID() == getLocalPlayer().getID()) {
                    try {
                        refEntityManager.getCurrentLevel().getEntityById(
                            static_cast<size_t>(getHealthId()));
                    } catch (const std::exception &e) {
                        std::cout << CATCH_ERROR_LOCATION << e.what()
                                  << std::endl;
                        return;
                    }
                    auto healthEnt =
                        refEntityManager.getCurrentLevel().getEntityById(
                            static_cast<size_t>(getHealthId()));
                    auto healthText =
                        healthEnt.getComponent<ecs::TextComponent<sf::Text>>();
                    if (healthText && health) {
                        if (floor(healthComp->getHealth()) <= 0) {
                            healthText->setStr("Health: Dead");
                        } else
                            healthText->setStr("Health: "
                                + std::to_string(healthComp->getHealth()));
                    } else if (healthText && !health) {
                        healthText->setStr("Health: Dead");
                    }
                }
                break;
            }
            break;
        }
        default: break;
    }
}

bool GameInstance::hasLocalPlayer(void) const
{
    if (_playerEntityID == -1) {
        return (false);
    }
    return (true);
}

ecs::Entity &GameInstance::getRandomPlayer(void)
{
    std::unique_lock lock(_gameLock);

    auto ent = refEntityManager.getCurrentLevel()
                   .findEntitiesByComponent<ecs::PlayerComponent>();

    size_t range = (ent.size() - 1) - 0 + 1;
    size_t num = ((size_t) rand()) % range;
    return (ent[num]);
}

ecs::Entity &GameInstance::getLocalPlayer()
{
    if (!hasLocalPlayer())
        throw ErrorClass(
            THROW_ERROR_LOCATION "No player was attached to the client");
    try {
        refEntityManager.getCurrentLevel().getEntityById(
            (size_t) _playerEntityID);
    } catch (const std::exception &e) {
        throw ErrorClass(THROW_ERROR_LOCATION "Player entity not found: "
            + std::string(e.what()));
    }
    return (refEntityManager.getCurrentLevel().getEntityById(
        (size_t) _playerEntityID));
}

size_t GameInstance::getHostClient()
{
    auto players = getAllPlayers();
    size_t host =
        getLocalPlayer().getComponent<ecs::PlayerComponent>()->getPlayerID();

    for (auto &pl : players) {
        auto playerID =
            pl.get().getComponent<ecs::PlayerComponent>()->getPlayerID();
        if (playerID < host)
            host = playerID;
    }
    return host;
}

std::vector<std::reference_wrapper<ecs::Entity>> GameInstance::getAllPlayers()
{
    return (refEntityManager.getCurrentLevel()
            .findEntitiesByComponent<ecs::PlayerComponent>());
}

ecs::Entity &GameInstance::getPlayerById(size_t id)
{
    std::unique_lock lock(_gameLock);
    auto players = getAllPlayers();

    for (auto &pl : players) {
        if (pl.get().getComponent<ecs::PlayerComponent>()->getPlayerID() == id)
            return (pl.get());
    }
    throw ErrorClass(
        THROW_ERROR_LOCATION "Player not found id=" + std::to_string(id));
}

void GameInstance::deletePlayer(size_t playerID)
{
    if (isServer() || _isConnectedToServer) {
        auto players = getAllPlayers();
        auto &pl = getPlayerById(playerID);
        if (!isServer()) {
            _factory.buildExplosionPlayer(
                pl.getComponent<ecs::PositionComponent>()->getX(),
                pl.getComponent<ecs::PositionComponent>()->getY());
            if (pl.getID() == (size_t) _playerEntityID) {
                _playerEntityID = -1;
            }
        }
        refEntityManager.getCurrentLevel().markEntityForDeletion(pl.getID());
        std::stringstream ss;
        ss << P_DEAD << " " << playerID << " " << PACKET_END;
        if (isServer()) {
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
        }
        // Switch scene if no more players
        // if (players.size() == 0) {}
    }
}

void GameInstance::damagePlayer(size_t playerID, int damage)
{
    if (isServer() || _isConnectedToServer) {
        auto &pl = getPlayerById(playerID);
        auto health = pl.getComponent<ecs::HealthComponent>();
        if (health) {
            health->setHealth(health->getHealth() - damage);
            if (isServer()) {
                std::stringstream ss;
                ss << P_DMG << " " << _ticks << " " << playerID << " "
                   << health->getHealth() << PACKET_END;
                refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::UDP, ss.str());
            }
        }
    }
}

void GameInstance::sendPlayerPosition(size_t playerID)
{
    if (isServer() || _isConnectedToServer) {
        auto &player = getPlayerById(playerID);
        auto position = player.getComponent<ecs::PositionComponent>();
        std::stringstream ss;
        ss << P_POS << " " << _ticks << " " << playerID << " "
           << position->getX() << " " << position->getY() << PACKET_END;
        if (isServer()) {
            refNetworkManager.sendToOthers(
                playerID, System::Network::ISocket::Type::UDP, ss.str());
        } else {
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
        }
    }
}

void GameInstance::updatePlayerPosition(
    size_t playerID, float newX, float newY)
{
    std::unique_lock lock(_gameLock);
    auto &player = getPlayerById(playerID);
    auto position = player.getComponent<ecs::PositionComponent>();

    if (position) {
        float oldX = position->getOldX();
        float oldY = position->getOldY();
        if (oldX != newX || oldY != newY) {
            float x = position->getX();
            float y = position->getY();
            position->setOldX(x);
            position->setOldY(y);
            position->setX(newX);
            position->setY(newY);
            if ((!isServer() && playerID == (size_t) _netClientID)
                || isServer())
                sendPlayerPosition(playerID);
        }
    }
}

void GameInstance::playerAnimations(ecs::Entity &player)
{
    static std::unordered_map<size_t, sf::Clock> animationTimers;
    std::string direction = "";
    auto position = player.getComponent<ecs::PositionComponent>();
    auto renderComp = player.getComponent<ecs::SpriteComponent<sf::Sprite>>();
    size_t playerID =
        player.getComponent<ecs::PlayerComponent>()->getPlayerID();

    if (animationTimers[playerID].getElapsedTime().asMilliseconds() < 200) {
        return;
    }

    if (position->getY() < position->getOldY()) {
        direction = "top";
    } else if (position->getY() > position->getOldY()) {
        direction = "down";
    }
    if (direction == "top") {
        renderComp->getSprite().setTextureRect(sf::Rect(
            132, renderComp->getSprite().getTextureRect().top, 33, 17));
    } else if (direction == "down") {
        renderComp->getSprite().setTextureRect(
            sf::Rect(0, renderComp->getSprite().getTextureRect().top, 33, 17));
    } else {
        renderComp->getSprite().setTextureRect(sf::Rect(
            66, renderComp->getSprite().getTextureRect().top, 33, 17));
    }

    animationTimers[playerID].restart();
}

void GameInstance::setPlayerEntityID(int id)
{
    this->_playerEntityID = id;
}

int GameInstance::getHealthId()
{
    return _healthId;
}

void GameInstance::setHealthId(int id)
{
    _healthId = id;
}
