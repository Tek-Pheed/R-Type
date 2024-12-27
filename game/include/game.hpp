/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include <typeindex>

#include "Engine.hpp"
#include "EngineAssetManager.hpp"
#include "EngineECSManager.hpp"
#include "Entity.hpp"
#include "GameSystems.hpp"
#include <unordered_map>

class Game {
    /* data */
  public:
    Game(Engine::Core &engineRef);
    ~Game();
    void gameLoop(float deltaTime);

    bool getServerMode();
    void setServerMode(bool mode);

    void setupClient();
    void setupServer();

    // Window Utilities
    sf::RenderWindow &getWindow();

    // Texture Utilities
    void LoadTexture();

    // Player Utilities
    void playEvent();
    void playerAnimations(ecs::Entity &player, std::string direction);
    void playerShoot(ecs::Entity &player);

    std::vector<ecs::Entity> &getEntities();
    int manageBuffers();
    void updateLocalPlayerPosition();
    ecs::Entity &getLocalPlayer();
    size_t getPlayerId();

    bool isServer() const;

    Engine::Core &refGameEngine;
    Engine::Feature::ECSManager<Game> &entityManager;
    Engine::Feature::AssetManager &assetManager;

    //     int createConnection(const char *ip, int portTCP, int portUDP);
    //     void writeToServer(
    //         const std::string &data, System::Network::ISocket::Type
    //         socketType);
    //     void receiveMessage();

    //     void handleConnection(int code, std::vector<std::string>
    //     &tokens); void handleEnemy(int code, std::vector<std::string>
    //     &tokens); void handleTerrain(int code, std::vector<std::string>
    //     &tokens); void handleMechs(int code, std::vector<std::string>
    //     &tokens); void addEntity(ecs::Entity & entity);

    //     std::vector<ecs::Entity &> &getEntities();
    //     int getId();

    //     void setServerMode(bool mode);
    //     bool isServer(void);

    //     void updateLocalplayerPosition();
    //     ecs::Entity & &getLocalPlayer();

    //     // These functions should be moved in a game Engine class

    //     // --

    //     // Client Management
    //     void setRenderClass(RenderClass *refRender);
    //     RenderClass *getRenderClass();
    //     void loadTexture();
    //     void createPlayer();

    //     // Player Management
    //     void handlePlayer(int code, std::vector<std::string> &tokens);
    //     void createNewPlayer(std::vector<std::string> &tokens);
    //     void setNewPosition(std::vector<std::string> &tokens);
    //     void playerDead(std::vector<std::string> &tokens);
    //     void createProjectile(std::vector<std::string> &tokens);
    //     void setPlayerHealth(std::vector<std::string> &tokens);
    //     void playerDisconnection(std::vector<std::string> &tokens);

    //     // Enemy Management
    //     void createEnemy(std::vector<std::string> &tokens);
    //     void enemyDead(std::vector<std::string> &tokens);
    //     void enemyShoot(std::vector<std::string> &tokens);
    //     void enemyDamage(std::vector<std::string> &tokens);

  private:
    size_t _PlayerId;
    bool _isServer;

    // Engine::Feature::Networking &_networkManager;

    std::unique_ptr<sf::RenderWindow> _window;

    // Client Sprites
    sf::Sprite _backgroundSprite;

    //     std::vector<ecs::Entity &> _entities;

    //     // To move into game Engine
    //     std::unordered_map<std::type_index, ecs::ISystem> _subsystems;
};

#endif /*game_HPP*/
