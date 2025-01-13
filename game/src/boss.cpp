/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** enemy
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <memory>
#include <sstream>
#include <string>
#include "Components.hpp"
#include "EngineNetworking.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"
#include "GameSystems.hpp"
#include "system_network.hpp"

using namespace RType;

// Maybe move networking entity position update into the systems

ecs::Entity &RType::GameInstance::buildBoss(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new boss to the game" << std::endl;
    auto &boss = refEntityManager.getCurrentLevel().createEntity();
    boss.addComponent(std::make_shared<ecs::BossComponent>(id));
    boss.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    boss.addComponent(std::make_shared<ecs::HealthComponent>(health));
    boss.addComponent(std::make_shared<ecs::BulletComponent>(false));
    boss.addComponent(
        std::make_shared<ecs::VelocityComponent>(0.0f, 0.0f));
    if (!_isServer) {
        auto &texture =
            refAssetManager.getAsset<sf::Texture>(Asset::BOSS_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 0, 183, 207));
        sprite.setScale(sf::Vector2f(2, 2));
        boss.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        boss.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
    }
    if (_isServer) {
        auto pos = boss.getComponent<ecs::PositionComponent>();
        auto ene = boss.getComponent<ecs::BossComponent>();
        if (pos) {
            std::stringstream sss;
            sss << B_SPAWN << " " << ene->getBossID() << " " << pos->getX()
                << " " << pos->getY() << PACKET_END;
            refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (boss);
}
