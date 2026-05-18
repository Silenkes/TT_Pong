#pragma once

#include <SFML/Graphics.hpp>

class Paddle;
class Ball;
class Boundary;

class GameInstance
{
public:
    GameInstance();

    void Tick(float DeltaTime);
    void Init();
    void CreateEntities();

    ~GameInstance();

private:
    void CreateBG();

    void SetBGSize();
    void HandleEvents();
    void Render();

private:
    sf::RenderWindow Window;
    sf::Clock DeltaClock;

    std::unique_ptr<sf::Texture> BGTexture;
    std::unique_ptr<sf::Sprite> BGSprite;

    Paddle* PlayerPaddle = nullptr;
    Paddle* BotPaddle = nullptr;
	Ball* BallInstance = nullptr;
    
    std::vector<std::unique_ptr<Boundary>> Boundaries;

    bool bInitialized = false;
    float WindowSizeY = 0.f;
    float WindowSizeX = 0.f;
};