/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ecsManager
*/

/**
 * @file Built-in ECS Manager of the game engine.
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
    /**
     * @brief Generates a new ID for creating entities.

     * @return: A new ID
     */
    size_t generateNewId(void);
    namespace Feature
    {

        /**
         * @brief A manager class for the ECS.
         *
         * This class is responsible of creating, storing and deleting entities
         * and subsystems.
         * @tparam GameClass: The class of your game. This game class will be
         * used by subsystems so they can access the context of your game when
         * they are updated.
         */
        template <typename GameClass>
        class ECSManager : public AEngineFeature {
          public:
            explicit ECSManager(Core &engineRef)
                : AEngineFeature(engineRef) {};
            ~ECSManager() {};

            /**
             * @brief Create a new Entity

             * This functions constructs an Entity object owned by this class.
             * @return ecs::Entity&: A reference to the constructed object.
             */
            ecs::Entity &createEntity()
            {
                const size_t id = generateNewId();

                _entities[id] = ecs::Entity(id);
                return (_entities[id]);
            }

            /**
             * @brief Add an entity into the manager list.

             * @param entity A reference to an entity.
             * @return true: The entity was successfully added into the
             manager's list.
             * @return false: The entity coud not be added into the manager's
             list.
             */
            bool addEntity(ecs::Entity &entity)
            {
                if (doesEntityExists(entity.getID()))
                    return (false);
                _entities[entity.getID()] = entity;
                return (true);
            }

            /**
             * @brief Destroy en entity owned by the manager

             * @param id The ID of the entity to detroy.
             * @return true: The entity was successfully deleted.
             * @return false: The entity could not be delete.
             */
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

            /**
             * @brief Check whether an entity exists, given an ID.

             * @param id: The ID to check against.
             * @return true: The entity is present in the manager's list.
             * @return false: The entity could not be found in the manager's
             list.
             */
            bool doesEntityExists(size_t id) const
            {
                for (auto &entity : _entities) {
                    if (entity.getID() == id)
                        return (true);
                }
                return (false);
            }

            /**
             * @brief Find an entity given it's ID.

             * @param id: The ID to check agains.
             * @return ecs::Entity&: A reference to the found entity.

             * @note: If the entity was not found, an std::out_of_range
             exception will be raised.
             */
            ecs::Entity &getEntityById(size_t id)
            {
                for (auto &entity : _entities) {
                    if (entity.getID() == id)
                        return (entity);
                }
                throw std::out_of_range("Id not found");
            }

            /**
             * @brief Return a vector containing references the manager's
             entities.

             * @return std::vector<std::reference_wrapper<ecs::Entity>>: A
             vector of reference to entities.
             */
            std::vector<std::reference_wrapper<ecs::Entity>> getEntities(void)
            {
                std::vector<std::reference_wrapper<ecs::Entity>> vect;

                for (auto &entity : _entities) {
                    vect.emplace_back(entity);
                }
                return (vect);
            }

            /**
             * @brief Returns the manager's entity vector.

             * @return std::vector<ecs::Entity>&: A reference to the manager's
             vector of entities.
             */
            std::vector<ecs::Entity> &getEntitiesVect(void)
            {
                return (_entities);
            }

            /**
             * @brief Change an entity ID.

             * @param oldId: The ID of the entity to change.
             * @param newId: The new ID.
             * @return true: The operation was performed successfully.
             * @return false: The operation could not be done.
             */
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

            /**
             * @brief Find all ids of entities matching a specific component.

             * @tparam ComponentType: The type of the component class to check
             against.
             * @return std::vector<size_t>: A vector of IDs.
             */
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

            /**
             * @brief Find all entities matching a specific component.

             * @tparam ComponentType: The type of the component class to check
             against.
             * @return std::vector<size_t>: A vector of entity reference.
             */
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

            /**
             * @brief Create a Subsystem object

             + This function created a System object derived from ecs::ISystem.
             * @tparam SubSystem: A class derived from ecs::ISystem
             * @param updateOnTick: Constrols if the system be updated each
             tick by the game engine.
             * @return SubSystem&: A reference to the newly created system
             object.
             */
            template <class SubSystem>
            SubSystem &createSubsystem(bool updateOnTick = true)
            {
                _systems.emplace(typeid(SubSystem),
                    SubSys_t(std::make_unique<SubSystem>(), updateOnTick));
                return (getSubsystem<SubSystem>());
            }

            /**
             * @brief Return a stored subsystem.

             * @tparam SubSystem: A class derived from ecs::ISystem
             * @return SubSystem&: A reference to the System object.
             */
            template <class SubSystem> SubSystem &getSubsystem(void)
            {
                return (*(static_cast<SubSystem *>(
                    _systems.at(typeid(SubSystem)).sys.get())));
            }

            /**
             * @brief Check whether a System was already created.

             * @tparam SubSystem: A class derived from ecs::ISystem
             * @return true: The system was found.
             * @return false: The system could not be found.
             */
            template <class SubSystem> bool doesSubsystemExists()
            {
                if (_systems.find(typeid(SubSystem)) != _systems.end())
                    return (true);
                return (false);
            }

            /**
             * @brief Destroy a system

             * @tparam SubSystem: A class derived from ecs::ISystem.
             * @return true: The system was successfully destroyed.
             * @return false: The system could not be destroyed.
             */
            template <class SubSystem> bool destroySubsystem()
            {
                if (doesSubsystemExists<SubSystem>()) {
                    _systems.erase(typeid(SubSystem));
                    return (true);
                }
                return (false);
            }

          protected:
            void engineOnStart(void) override {};
            void engineOnStop(void) override {};
            void engineOnTick(float deltaTimeSec) override
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