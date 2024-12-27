/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>
#include "EngineECSManager.hpp"
#include "Entity.hpp"

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
    for (size_t i = 0; i < _entities.size(); i++) {
        if (_entities[i].getID() == id) {
            _entities.erase(_entities.begin() + (long) i);
            break;
        }
    }
    return (true);
}

bool ECSManager::doesEntityExists(size_t id) const
{
    for (auto &entity : _entities) {
        if (entity.getID() == id)
            return (true);
    }
    return (false);
}

bool ECSManager::changeEntityId(size_t oldId, size_t newId)
{
    if (!doesEntityExists(oldId))
        return (false);
    ecs::Entity ent = getEntityById(oldId);
    destroyEntityById(oldId);
    ent.setID(newId);
    addEntity(ent);
    return (true);
}

std::vector<std::reference_wrapper<ecs::Entity>> ECSManager::getEntities(void)
{
    std::vector<std::reference_wrapper<ecs::Entity>> vect;

    for (auto &entity : _entities) {
        vect.emplace_back(entity);
    }
    return (vect);
}

std::vector<ecs::Entity> &ECSManager::getEntitiesVect(void)
{
    return (_entities);
}

ecs::Entity &ECSManager::getEntityById(size_t id)
{
    for (auto &entity : _entities) {
        if (entity.getID() == id)
            return (entity);
    }
    throw std::out_of_range("Id not found");
}

ecs::Entity &ECSManager::createEntity()
{
    const size_t id = generateNewId();

    _entities[id] = ecs::Entity(id);
    return (_entities[id]);
}

bool ECSManager::addEntity(ecs::Entity &entity)
{
    if (doesEntityExists(entity.getID()))
        return (false);
    _entities[entity.getID()] = entity;
    return (true);
}

template <typename ComponentType>
std::vector<size_t> ECSManager::findEntitiesByComponent()
{
    std::vector<size_t> ent;
    std::shared_ptr<ComponentType> comp = nullptr;

    for (auto &entity : _entities) {
        comp = entity.template getComponent<ComponentType>();
        if (comp != nullptr)
            ent.emplace_back(entity.getID());
    }
    return (ent);
}

template <typename ComponentType>
std::vector<std::reference_wrapper<ecs::Entity>>
ECSManager::findEntitiesByComponent()
{
    std::vector<std::reference_wrapper<ecs::Entity>> ent;
    std::shared_ptr<ComponentType> comp = nullptr;

    for (auto &entity : _entities) {
        comp = entity.template getComponent<ComponentType>();
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
    // use an initialiser function in ISystem to init all variables and
    // references needed otherwise this will not work

    for (auto const &[typeIndex, subsystem] : _systems) {
        if (subsystem.update)
            subsystem.sys->update(
                _entities, deltaTimeSec, false); // Change here isServer
    }
}

void ECSManager::onStop()
{
    return;
}