/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

#ifndef TINY_V8_ECS_MANAGER_HPP
#define TINY_V8_ECS_MANAGER_HPP

#include <cstddef>
#include <vector>
#include "Engine.hpp"

#include "Entity.hpp"
#include "Systems.hpp"
#include <unordered_map>

namespace Engine
{
    namespace Feature
    {
        class ECSManager : public AEngineFeature {
          public:
            explicit ECSManager(Core &engineRef);
            ~ECSManager();

            std::shared_ptr<ecs::Entity> &createEntity();

            bool addEntity(std::shared_ptr<ecs::Entity> entity);
            bool destroyEntityById(size_t id);
            bool doesEntityExists(size_t id) const;
            std::shared_ptr<ecs::Entity> &getEntityById(size_t id);

            std::vector<std::shared_ptr<ecs::Entity>> getEntities(void);

            template <typename ComponentType>
            std::vector<size_t> findEntitiesByComponent();

            template <typename ComponentType>
            std::vector<std::shared_ptr<ecs::Entity>>
            findEntitiesByComponent();

            bool changeEntityId(size_t oldId, size_t newId);

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
            void onStart(void) override;
            void onTick(float deltaTimeSec) override;
            void onStop(void) override;

            struct SubSys_t {
                std::unique_ptr<ecs::ISystem> sys;
                bool update;
            };

            std::unordered_map<size_t, std::shared_ptr<ecs::Entity>> _entities;
            std::map<std::type_index, SubSys_t> _systems;
        };

    } // namespace Feature

} // namespace Engine

#endif /* TINY_V8_ECS_MANAGER_HPP */