/*
** EPITECH PROJECT, 2024
** Untitled (Workspace)
** File description:
** game
*/

#include "Components.hpp"
#include "Systems.hpp"
#include "protocol.hpp"
#include "server.hpp"

void server::createEnemy(unsigned int id, int x, int y)
{
    std::cout << "Create new enemy" << std::endl;

    auto enemy = std::make_shared<ecs::Entity>(id);
    enemy->addComponent(std::make_shared<ecs::EnemyComponent>("Enemy"));
    enemy->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
    enemy->addComponent(std::make_shared<ecs::VelocityComponent>(-200.0, 0.0));
    enemy->addComponent(std::make_shared<ecs::HealthComponent>(100));

    _globalMutex.lock();
    _gameState.emplace_back(enemy);
    _globalMutex.unlock();
    std::string pack = makePacket(E_SPAWN, (int) id, x, y);
    send_to_all(pack);
}

void server::createProjectile(int id)
{
    float x = 0.0f;
    float y = 0.0f;

    for (auto &entity : _gameState) {
        if (entity->getID() == static_cast<size_t>(id)) {
            auto playerComp = entity->getComponent<ecs::PlayerComponent>();
            if (playerComp == nullptr)
                return;
            auto position = entity->getComponent<ecs::PositionComponent>();
            x = position->getX();
            y = position->getY();
            break;
        }
    }
    auto bullet = std::make_shared<ecs::Entity>(rand());
    bullet->addComponent(std::make_shared<ecs::BulletComponent>(1));
    bullet->addComponent(
        std::make_shared<ecs::PositionComponent>(x + 100, y + 25));
    bullet->addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
    _gameState.push_back(bullet);
}

void server::gameUpdate(long deltaTimeMs)
{
    ecs::PositionSystem positionSystem;
    ecs::BulletSystem bulletSystem;

    if (_enemyTimerMS >= ENEMY_SPAWN_DELAY_MS) {
        _enemyTimerMS = 0;
        createEnemy(
            (unsigned int) rand(), randRange(800, 1200), randRange(50, 700));
    }

    positionSystem.update(_gameState, nullptr, (float) deltaTimeMs / 1000, true);
    bulletSystem.update(_gameState, nullptr, (float) deltaTimeMs / 1000, true);

    auto bullets = getEntitiesByComponent<ecs::BulletComponent>();

    for (const auto &b : bullets) {
        if (b == nullptr)
            continue;
        auto pos = b->getComponent<ecs::PositionComponent>();
        if (pos->getX() > 4000 || pos->getY() > 2200 || pos->getX() < 0
            || pos->getY() < 0) {
            _gameState.erase(
                std::remove(_gameState.begin(), _gameState.end(), b));
        }
    }
    auto enemies = getEntitiesByComponent<ecs::EnemyComponent>();
    for (const auto &b : enemies) {
        if (b == nullptr)
            continue;
        auto pos = b->getComponent<ecs::PositionComponent>();
        if (pos->getX() > 4000 || pos->getY() > 2200 || pos->getX() < 0
            || pos->getY() < 0) {
            std::string pack = makePacket(E_DEAD, b->getID());
            send_to_all(pack);
            _gameState.erase(
                std::remove(_gameState.begin(), _gameState.end(), b));
        }
    }
}

void server::internalUpdate(long deltaTimeMs)
{
    _enemyTimerMS += deltaTimeMs;
    gameUpdate(deltaTimeMs);
}
