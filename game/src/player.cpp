/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** player
*/

#include <mutex>
#if defined(WIN32)
    #define NOMINMAX
#endif

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "ErrorClass.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "system_network.hpp"

using namespace RType;

void GameInstance::handleLoby(int code, const std::vector<std::string> &tokens)
{
    switch (code) {
        case Protocol::L_STARTGAME: {
            _gameStarted = true;
            if (_isServer) {
                size_t id = (size_t) atoi(tokens[0].c_str());
                std::stringstream sss;
                sss << L_STARTGAME << " " << id << PACKET_END;
                refNetworkManager.sendToAll(
                    System::Network::ISocket::Type::TCP, sss.str());
            }
            break;
        }
        case Protocol::L_SETMAXPLAYRS: {
            if (tokens.size() >= 2) {
                _maxPlayers = (size_t) atoi(tokens[1].c_str());
                if (_isServer) {
                    std::stringstream sss;
                    size_t id = (size_t) atoi(tokens[0].c_str());
                    sss << L_SETMAXPLAYRS << " " << id << PACKET_END;
                    refNetworkManager.sendToOthers(
                        id, System::Network::ISocket::Type::TCP, sss.str());
                }
            }
            break;
            default: break;
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
                uint64_t tick = _ticks;
                if (!isServer()) {
                    tick = (uint64_t) atoi(tokens[0].c_str());
                    if (!(_lastNetTick <= tick)) {
                        return;
                    }
                }
                size_t id = (size_t) atoi(tokens[1].c_str());
                auto &player = getPlayerById(id);
                auto pos = player.getComponent<ecs::PositionComponent>();
                pos->setX((float) std::atof(tokens[2].c_str()));
                pos->setY((float) std::atof(tokens[3].c_str()));
                if (isServer()) {
                    std::stringstream ss;
                    ss << P_POS << " " << tick << " "
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
                playerShoot(id);
                break;
            }
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

ecs::Entity &GameInstance::getLocalPlayer()
{
    if (!hasLocalPlayer())
        throw ErrorClass("No player was attached to the client");
    return (refEntityManager.getCurrentLevel().getEntityById(
        (size_t) _playerEntityID));
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
    throw ErrorClass("Player not found id=" + std::to_string(id));
}

void GameInstance::deletePlayer(size_t playerID)
{
    if (isServer() || _isConnectedToServer) {
        auto &pl = getPlayerById(playerID);
        refEntityManager.getCurrentLevel().destroyEntityById(pl.getID());
        std::stringstream ss;
        ss << P_DEAD << " " << playerID << " " << PACKET_END;
        if (isServer()) {
            refNetworkManager.sendToOthers(
                playerID, System::Network::ISocket::Type::TCP, ss.str());
        } else {
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, ss.str());
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

void GameInstance::playerShoot(size_t playerID)
{
    std::unique_lock lock(_gameLock);
    auto player = getPlayerById(playerID);
    auto positionComp = player.getComponent<ecs::PositionComponent>();
    if (!positionComp)
        return;
    auto &bullet = refEntityManager.getCurrentLevel().createEntity();
    bullet.addComponent(std::make_shared<ecs::BulletComponent>(1));
    bullet.addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
    bullet.addComponent(std::make_shared<ecs::PositionComponent>(
        positionComp->getX() + 100, positionComp->getY() + 25));
    bullet.addComponent(std::make_shared<ecs::HitboxComponent>(10.0f, 10.0f));

    std::stringstream ss;
    ss << P_SHOOT << " " << playerID << " " << PACKET_END;
    if (isServer()) {
        refNetworkManager.sendToOthers(
            playerID, System::Network::ISocket::Type::UDP, ss.str());
    } else {
        auto &texture =
            refAssetManager.getAsset<sf::Texture>(Asset::BULLET_TEXTURE);
        bullet.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        sf::Sprite s;
        s.setTexture(texture);
        s.setTextureRect(sf::Rect(137, 153, 64, 16));
        bullet.addComponent(
            std::make_shared<ecs::SpriteComponent<sf::Sprite>>(s, 132, 33));
        if (playerID == (size_t) _netClientID)
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::UDP, ss.str());
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
        renderComp->getSprite().setTextureRect(sf::Rect(132, 0, 33, 14));
    } else if (direction == "down") {
        renderComp->getSprite().setTextureRect(sf::Rect(0, 0, 33, 14));
    } else {
        renderComp->getSprite().setTextureRect(sf::Rect(66, 0, 33, 14));
    }

    animationTimers[playerID].restart();
}

void GameInstance::setPlayerEntityID(int id)
{
    this->_playerEntityID = id;
}
