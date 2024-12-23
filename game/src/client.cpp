// /*
// ** EPITECH PROJECT, 2024
// ** R-Type
// ** File description:
// ** client
// */

// #include <ctime>
// #include <memory>
// #include <vector>
// #include "Entity.hpp"
// #include "Networking.hpp"
// #include "RenderClass.hpp"
// #include "game.hpp"
// #include "system_network.hpp"

// game::game(Networking &net)
//     : _id(-1), _refNetwork(net)
// {
// }

// game::~game()
// {
//     _id = -1;
//     _refRender->~RenderClass();
// }

// void game::setRenderClass(RenderClass *refRender)
// {
//     _refRender = refRender;
// }

// void game::loadTexture()
// {
//     sf::Texture playerTexture;
//     sf::Texture enemyTexture;
//     sf::Texture bulletTexture;

//     if (playerTexture.loadFromFile("assets/sprites/r-typesheet42.gif"))
//         _refRender->setPlayerTexture(playerTexture);
//     if (enemyTexture.loadFromFile("assets/sprites/r-typesheet31.gif"))
//         _refRender->setEnemyTexture(enemyTexture);
//     if (bulletTexture.loadFromFile("assets/sprites/r-typesheet1.gif"))
//         _refRender->setBulletTexture(bulletTexture);
// }

// void game::addEntity(std::shared_ptr<ecs::Entity> entity)
// {
//     _entities.push_back(entity);
// }

// std::vector<std::shared_ptr<ecs::Entity>> &game::getEntities()
// {
//     return _entities;
// }

// int game::getId()
// {
//     return _id;
// }

// void game::createPlayer()
// {
//     auto &player = getLocalPlayer();

//     player = std::make_shared<ecs::Entity>(getId());
//     player->addComponent(std::make_shared<ecs::PlayerComponent>("Samy"));
//     player->addComponent(std::make_shared<ecs::PositionComponent>(100,
//     100));
//     player->addComponent(std::make_shared<ecs::VelocityComponent>(0.0,
//     0.0)); player->addComponent(std::make_shared<ecs::RenderComponent>(
//         ecs::ObjectType::SPRITE, _refRender->getPlayerTexture()));
//     player->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
//         sf::Rect(66, 0, 33, 14));
//     player->getComponent<ecs::RenderComponent>()->getSprite()->setScale(
//         sf::Vector2f(3, 3));
//     addEntity(player);
// }

// RenderClass *game::getRenderClass()
// {
//     return _refRender;
// }

// #include <SFML/Graphics.hpp>
// #include <sstream>

// #include "EngineECSManager.hpp"
// #include "Entity.hpp"
// #include "game.hpp"

// sf::RenderWindow &Game::getWindow()
// {
//     return *_window;
// }

// void Game::playerShoot(std::shared_ptr<ecs::Entity> player)
// {
//     std::stringstream ss;
//     auto bullet = std::make_shared<ecs::Entity>(rand());
//     auto positionComp = player->getComponent<ecs::PositionComponent>();

//     bullet->addComponent(std::make_shared<ecs::BulletComponent>(1));
//     bullet->addComponent(std::make_shared<ecs::PositionComponent>(
//         positionComp->getX() + 100, positionComp->getY() + 25));
//     bullet->addComponent(std::make_shared<ecs::VelocityComponent>(350.0f, 0));
//     bullet->addComponent(std::make_shared<ecs::RenderComponent>(
//         ecs::ObjectType::SPRITE, this->_bulletTexture));

//     bullet->getComponent<ecs::RenderComponent>()->getSprite()->setTextureRect(
//         sf::Rect(137, 153, 64, 16));
//     ss << "104 " << _ClientId << "\t\n";

//     // writeToServer(ss.str(), System::Network::ISocket::UDP);
//     _refGameEngine.getFeature<Engine::Feature::ECSManager>().addEntity(bullet);
// }

// void Game::playerAnimations(
//     std::shared_ptr<ecs::Entity> player, std::string direction)
// {
//     auto renderComp = player->getComponent<ecs::RenderComponent>();
//     if (direction == "top") {
//         renderComp->getSprite()->setTextureRect(sf::Rect(132, 0, 33, 14));
//     } else if (direction == "down") {
//         renderComp->getSprite()->setTextureRect(sf::Rect(0, 0, 33, 14));
//     } else {
//         renderComp->getSprite()->setTextureRect(sf::Rect(66, 0, 33, 14));
//     }
// }
