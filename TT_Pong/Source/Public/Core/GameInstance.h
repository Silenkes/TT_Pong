#pragma once

#include <SFML/Graphics.hpp>

class Paddle;
class Ball;
class Boundary;

enum class EGameState : uint8_t
{
    None,
    Running,
    PausedByUser,
    PausedBetweenRounds,
};

class GameInstance
{
public:
    GameInstance();
    ~GameInstance();

    void Tick(float DeltaTime);
    void Init();
    void CreateEntities();
    void ResetRound();

    // * If pause is permanent (e.a., the game paused by user), Duration must be <= 0!!!
    void Pause(EGameState NewGameState, float Duration);
    void UnPause();

private:
    void CreateBG();
    void CreateUI();
    void SetBGSize();
    void HandleEvents();
    void HandleIntersections();
    void HandlePlayerInput(float DeltaTime);
    void Render();

private:
    sf::RenderWindow Window;
    sf::Clock DeltaClock;

    std::unique_ptr<sf::Texture> BGTexture;
    std::unique_ptr<sf::Sprite> BGSprite;
    std::unique_ptr<sf::Font> TextFont;
    std::unique_ptr<sf::Text> PlayerScoreText;
    std::unique_ptr<sf::Text> BotScoreText;
    std::unique_ptr<sf::Text> PauseText;
    std::unique_ptr<Paddle> PlayerPaddle;
    std::unique_ptr<Paddle> BotPaddle;
    std::unique_ptr<Ball> BallInstance;

    std::vector<std::unique_ptr<Boundary>> Boundaries;

    bool bInitialized = false;
    float WindowSizeY = 0.f;
    float WindowSizeX = 0.f;
    float PauseTimer = 0.f;
    float PauseDuration = 0.f;
    int PlayerScore = 0;
    int BotScore = 0;

    EGameState GameState = EGameState::None;
};