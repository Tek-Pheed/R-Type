/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

#include "ECSManager.hpp"
#include <cstdint>
#include <memory>

using namespace Engine::Feature;

static size_t generateNewId(void)
{
    static size_t entityCounter = 0U;

    if (entityCounter >= SIZE_MAX - 2)
        entityCounter = 0;
    return (entityCounter++);
}

ECSManager::ECSManager(Core &engineRef) : AEngineFeature(engineRef)
{
}

ECSManager::~ECSManager()
{
}

bool ECSManager::destroyEntityById(size_t id)
{
    if (!doesEntityExists(id))
        return (false);
    _entities.erase(id);
    return (true);
}

bool ECSManager::doesEntityExists(size_t id) const
{
    if (_entities.find(id) != _entities.end())
        return (true);
    return (false);
}

bool ECSManager::changeEntityId(size_t oldId, size_t newId)
{
    std::shared_ptr<ecs::Entity> ent;

    if (!doesEntityExists(oldId))
        return (false);
    ent = _entities[oldId];
    destroyEntityById(oldId);
    ent->setID(newId);
    addEntity(ent);
    return (true);
}

std::vector<std::shared_ptr<ecs::Entity>> ECSManager::getEntities(void)
{
    std::vector<std::shared_ptr<ecs::Entity>> vect;

    for (auto const &[id, entity] : _entities) {
        vect.emplace_back(entity);
    }
    return (vect);
}

std::shared_ptr<ecs::Entity> &ECSManager::getEntityById(size_t id)
{
    return (_entities[id]);
}

std::shared_ptr<ecs::Entity> &ECSManager::createEntity()
{
    const size_t id = generateNewId();

    _entities[id] = std::make_shared<ecs::Entity>(id);
    return (_entities[id]);
}

bool ECSManager::addEntity(std::shared_ptr<ecs::Entity> entity)
{
    if (_entities.contains(entity->getID()))
        return (false);
    _entities[entity->getID()] = entity;
    return (true);
}

template <typename ComponentType>
std::vector<size_t> ECSManager::findEntitiesByComponent()
{
    std::vector<size_t> ent;
    std::shared_ptr<ComponentType> comp = nullptr;

    for (auto const &[id, entity] : _entities) {
        comp = entity->template getComponent<ComponentType>();
        if (comp != nullptr)
            ent.emplace_back(id);
    }
    return (ent);
}

template <typename ComponentType>
std::vector<std::shared_ptr<ecs::Entity>> ECSManager::findEntitiesByComponent()
{
    std::vector<std::shared_ptr<ecs::Entity>> ent;
    std::shared_ptr<ComponentType> comp = nullptr;

    for (auto const &[id, entity] : _entities) {
        comp = entity->template getComponent<ComponentType>();
        if (comp != nullptr)
            ent.emplace_back(entity);
    }
    return (ent);
}

void ECSManager::onStart()
{
    return;
}

void ECSManager::onTick(float deltaTimeSec)
{
    (void) deltaTimeSec;
    // TODO: Replace SFML references in ECS
    // otherwise this will not work

    // for (auto const &[typeIndex, subsystem] : _systems) {
    //     if (subsystem.update)
    //         subsystem.sys->update(std::vector<std::shared_ptr<ecs::Entity>>
    //         &entity, sf::RenderWindow *window, float deltaTime, bool
    //         isServer)
    // }
}
void ECSManager::onStop()
{
    return;
}