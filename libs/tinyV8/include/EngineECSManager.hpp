/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

#ifndef TINY_V8_ECS_MANAGER_HPP
#define TINY_V8_ECS_MANAGER_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>
#include "Engine.hpp"
#include "Entity.hpp"
#include "Systems.hpp"

namespace Engine
{
    size_t generateNewId(void);
    namespace Feature
    {

        template <typename GameClass>
        class ECSManager : public AEngineFeature {
          public:
            explicit ECSManager(Core &engineRef)
                : AEngineFeature(engineRef) {};
            ~ECSManager() {};

            ecs::Entity &createEntity()
            {
                const size_t id = generateNewId();

                _entities[id] = ecs::Entity(id);
                return (_entities[id]);
            }

            bool addEntity(ecs::Entity &entity)
            {
                if (doesEntityExists(entity.getID()))
                    return (false);
                _entities[entity.getID()] = entity;
                return (true);
            }

            bool destroyEntityById(size_t id)
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

            bool doesEntityExists(size_t id) const
            {
                for (auto &entity : _entities) {
                    if (entity.getID() == id)
                        return (true);
                }
                return (false);
            }

            ecs::Entity &getEntityById(size_t id)
            {
                for (auto &entity : _entities) {
                    if (entity.getID() == id)
                        return (entity);
                }
                throw std::out_of_range("Id not found");
            }

            std::vector<std::reference_wrapper<ecs::Entity>> getEntities(void)
            {
                std::vector<std::reference_wrapper<ecs::Entity>> vect;

                for (auto &entity : _entities) {
                    vect.emplace_back(entity);
                }
                return (vect);
            }

            std::vector<ecs::Entity> &getEntitiesVect(void)
            {
                return (_entities);
            }

            bool changeEntityId(size_t oldId, size_t newId)
            {
                if (!doesEntityExists(oldId))
                    return (false);
                ecs::Entity ent = getEntityById(oldId);
                destroyEntityById(oldId);
                ent.setID(newId);
                addEntity(ent);
                return (true);
            }

            template <typename ComponentType>
            std::vector<size_t> findEntitiesIdByComponent()
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
            findEntitiesByComponent()
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

            template <class SubSystem> SubSystem &getSubsystem(void)
            {
                return (*(static_cast<SubSystem *>(
                    _systems.at(typeid(SubSystem)).sys.get())));
            }

            template <class SubSystem>
            SubSystem &createSubsystem(bool updateOnTick = true)
            {
                _systems.emplace(typeid(SubSystem),
                    SubSys_t(std::make_unique<SubSystem>(), updateOnTick));
                return (getSubsystem<SubSystem>());
            }

            template <class SubSystem> bool doesSubsystemExists()
            {
                if (_systems.find(typeid(SubSystem)) != _systems.end())
                    return (true);
                return (false);
            }

            template <class SubSystem> bool destroySubsystem()
            {
                if (doesSubsystemExists<SubSystem>()) {
                    _systems.erase(typeid(SubSystem));
                    return (true);
                }
                return (false);
            }

          protected:
            void onStart(void) override {};
            void onStop(void) override {};
            void onTick(float deltaTimeSec) override
            {
                (void) deltaTimeSec;

                for (auto const &[typeIndex, subsystem] : _systems) {
                    if (subsystem.update)
                        subsystem.sys->update(_entities, deltaTimeSec);
                }
            }

            struct SubSys_t {
                explicit SubSys_t(
                    std::unique_ptr<ecs::ISystem<GameClass>> s, bool updt)
                    : sys(std::move(s)), update(updt) {};

                std::unique_ptr<ecs::ISystem<GameClass>> sys;
                bool update;
            };

            std::vector<ecs::Entity> _entities;
            std::map<std::type_index, SubSys_t> _systems;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_ECS_MANAGER_HPP */