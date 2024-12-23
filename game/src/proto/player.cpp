// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** player
// */

// #include "Entity.hpp"
// #include "RenderClass.hpp"
// #include "game.hpp"
// #include "protocol.hpp"

// void game::createNewPlayer(std::vector<std::string> &tokens)
// {
//     std::cout << "Create new player" << std::endl;
//     if (tokens.size() < 4)
//         return;
//     const int id = std::stoi(tokens[0]);
//     const std::string name = tokens[1];
//     const float x = std::stof(tokens[2]);
//     const float y = std::stof(tokens[3]);

//     auto player = std::make_shared<ecs::Entity>(id);
//     player->addComponent(std::make_shared<ecs::PlayerComponent>());
//     player->addComponent(std::make_shared<ecs::PositionComponent>(x, y));
//     player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0, 0.0));

//     player->addComponent(
//         std::make_shared<ecs::RenderComponent>(ecs::ObjectType::SPRITE));

//     sf::Sprite playerSP(_refRender.getPlayerTexture());
//     playerSP.setTextureRect(sf::Rect(66, 0, 33, 14));
//     playerSP.setScale(sf::Vector2f(3, 3));

//     player->addComponent(
//         std::make_shared<ecs::SpriteComponent<sf::Sprite>>(playerSP, 3, 3));

//     add_entity(player);
// }

// void game::setNewPosition(std::vector<std::string> &tokens)
// {
//     if (tokens.size() < 3)
//         return;
//     const int id = std::stoi(tokens[0]);
//     const float x = std::stof(tokens[1]);
//     const float y = std::stof(tokens[2]);

//     for (auto &entity : _entities) {
//         if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
//             auto playerComp = entity->getComponent<ecs::PlayerComponent>();
//             if (playerComp == nullptr)
//                 return;
//             auto position = entity->getComponent<ecs::PositionComponent>();
//             position->setX(static_cast<float>(x));
//             position->setY(static_cast<float>(y));
//         }
//     }
// }

// void game::playerDead(std::vector<std::string> &tokens)
// {
//     if (tokens.size() < 1)
//         return;
//     const int id = std::stoi(tokens[0]);

//     for (auto &entity : _entities) {
//         if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
//             auto playerComp = entity->getComponent<ecs::PlayerComponent>();
//             if (playerComp == nullptr)
//                 return;
//             _entities.erase(
//                 std::remove(_entities.begin(), _entities.end(), entity),
//                 _entities.end());
//         }
//     }
// }

// void game::createProjectile(std::vector<std::string> &tokens)
// {
//     if (tokens.size() < 1)
//         return;
//     const int id = std::stoi(tokens[0]);
//     float x = 0.0f;
//     float y = 0.0f;

//     for (auto &entity : _entities) {
//         if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
//             auto playerComp = entity->getComponent<ecs::PlayerComponent>();
//             if (playerComp == nullptr)
//                 return;
//             auto position = entity->getComponent<ecs::PositionComponent>();
//             x = position->getX();
//             y = position->getY();
//             break;
//         }
//     }

//     auto bullet = std::make_shared<ecs::Entity>(rand());
//     bullet->addComponent(std::make_shared<ecs::BulletComponent>(1));
//     bullet->addComponent(
//         std::make_shared<ecs::PositionComponent>(x + 100, y + 25));
//     bullet->addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));

//     bullet->addComponent(
//         std::make_shared<ecs::RenderComponent>(ecs::ObjectType::SPRITE));

//     sf::Sprite bulletSP(_refRender.getBulletTexture());
//     bulletSP.setTextureRect(sf::Rect(137, 153, 64, 16));

//     bullet->addComponent(
//         std::make_shared<ecs::SpriteComponent<sf::Sprite>>(bulletSP, 1, 1));
//     _entities.push_back(bullet);
// }

// void game::setPlayerHealth(std::vector<std::string> &tokens)
// {
//     if (tokens.size() < 2)
//         return;
//     const int id = std::stoi(tokens[0]);
//     const int dmg = std::stoi(tokens[1]);

//     for (auto &entity : _entities) {
//         if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
//             auto playerComp = entity->getComponent<ecs::PlayerComponent>();
//             if (playerComp == nullptr)
//                 return;
//             auto health = entity->getComponent<ecs::HealthComponent>();
//             health->setHealth(health->getHealth() - dmg);
//         }
//     }
// }

// void game::playerDisconnection(std::vector<std::string> &tokens)
// {
//     if (tokens.size() < 1)
//         return;
//     const int id = std::stoi(tokens[0]);

//     for (auto &entity : _entities) {
//         if (entity != nullptr && entity->getID() == static_cast<size_t>(id)) {
//             auto playerComp = entity->getComponent<ecs::PlayerComponent>();
//             if (playerComp == nullptr)
//                 return;
//             _entities.erase(
//                 std::remove(_entities.begin(), _entities.end(), entity),
//                 _entities.end());
//         }
//     }
// }

// void game::handlePlayer(int code, std::vector<std::string> &tokens)
// {
//     switch (code) {
//         case P_CONN: {
//             createNewPlayer(tokens);
//             break;
//         }
//         case P_POS: {
//             setNewPosition(tokens);
//             break;
//         }
//         case P_DEAD: {
//             playerDead(tokens);
//             break;
//         }
//         case P_SHOOT: {
//             createProjectile(tokens);
//             break;
//         }
//         case P_DMG: {
//             setPlayerHealth(tokens);
//             break;
//         }
//         case P_DISCONN: {
//             playerDisconnection(tokens);
//             break;
//         }
//         default: {
//             break;
//         }
//     }
// }
