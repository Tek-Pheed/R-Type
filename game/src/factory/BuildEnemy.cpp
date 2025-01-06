/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** enemy
*/

#if defined(WIN32)
    #define NOMINMAX
#endif

#include <sstream>
#include "Factory.hpp"
#include "GameAssets.hpp"
#include "GameProtocol.hpp"

using namespace RType;

ecs::Entity &RType::Factory::buildEnemy(
    size_t id, float posX, float posY, float health)
{
    std::cout << "Adding new enemy to the game" << std::endl;
    auto &enemy = _game.refEntityManager.getCurrentLevel().createEntity();
    enemy.addComponent(std::make_shared<ecs::EnemyComponent>(id));
    enemy.addComponent(std::make_shared<ecs::PositionComponent>(posX, posY));
    enemy.addComponent(std::make_shared<ecs::HealthComponent>(health));
    enemy.addComponent(
        std::make_shared<ecs::VelocityComponent>(-200.0f, 0.0f));
    if (!_game.isServer()) {
        auto &texture =
            _game.refAssetManager.getAsset<sf::Texture>(Asset::ENEMY_TEXTURE);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::Rect(0, 16, 32, 32));
        sprite.setScale(sf::Vector2f(2, 2));
        enemy.addComponent(std::make_shared<ecs::RenderComponent>(
            ecs::RenderComponent::ObjectType::SPRITE));
        enemy.addComponent(std::make_shared<ecs::SpriteComponent<sf::Sprite>>(
            sprite, 3.0, 3.0));
    } else {
        auto pos = enemy.getComponent<ecs::PositionComponent>();
        auto ene = enemy.getComponent<ecs::EnemyComponent>();
        if (pos) {
            std::stringstream sss;
            sss << E_SPAWN << " " << ene->getEnemyID() << " " << pos->getX()
                << " " << pos->getY() << PACKET_END;
            _game.refNetworkManager.sendToAll(
                System::Network::ISocket::Type::TCP, sss.str());
        }
    }
    return (enemy);
}
