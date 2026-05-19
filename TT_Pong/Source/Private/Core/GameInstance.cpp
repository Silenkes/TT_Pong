#include "Core/GameInstance.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Entities/Boundary.h"
#include <iostream>

GameInstance::GameInstance() = default;
GameInstance::~GameInstance() = default;

void GameInstance::Init()
{
	if (bInitialized)
		return;

	bInitialized = true;

	Window.create(sf::VideoMode({ 1280, 720 }), "UnrealPongine_Haponov");

	WindowSizeY = Window.getSize().y;
	WindowSizeX = Window.getSize().x;
	GameState = EGameState::Running;

	CreateBG();
	CreateEntities();
	SetBGSize();

	while (Window.isOpen())
	{
		float DeltaTime = DeltaClock.restart().asSeconds();

		Tick(DeltaTime);
		HandleEvents();
		HandleIntersections();
		Render();
	}
}

void GameInstance::CreateBG()
{
	BGTexture = std::make_unique<sf::Texture>();

	if (!BGTexture->loadFromFile("Assets/Textures/GameBG.png"))
	{
		std::cout << "BG texture not found!" << std::endl;

		return;
	}

	BGSprite = std::make_unique<sf::Sprite>(*BGTexture);
}

void GameInstance::SetBGSize()
{
	if (!BGTexture || !BGSprite)
		return;

	sf::Vector2u BGTextureSize = BGTexture->getSize();

	BGSprite->setScale({WindowSizeX / BGTextureSize.x, WindowSizeY / BGTextureSize.y });
}

void GameInstance::CreateEntities()
{
	PlayerPaddle = std::make_unique<Paddle>(sf::Vector2f(WindowSizeX / 8.f, WindowSizeY / 2), sf::Color::Green, sf::Color::Magenta);
	BotPaddle = std::make_unique<Paddle>(sf::Vector2f(WindowSizeX / 1.15f, WindowSizeY / 2.f), sf::Color::Red, sf::Color::Yellow);
	BallInstance = std::make_unique<Ball>(sf::Vector2f(WindowSizeX / 2.f, WindowSizeY / 2.f), 15.f, sf::Color(125, 34, 112, 255));

	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, 0.f), sf::Vector2f(WindowSizeX, 20.f), true, EBoundaryType::Top));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, WindowSizeY - 20.f), sf::Vector2f(WindowSizeX, 20.f), true, EBoundaryType::Bottom));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, 0.f), sf::Vector2f(20.f, WindowSizeY), true, EBoundaryType::Left));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(WindowSizeX - 20.f, 0.f), sf::Vector2f(20.f, WindowSizeY), true, EBoundaryType::Right));
}

void GameInstance::HandleEvents()
{
	while (const std::optional Event = Window.pollEvent())
	{
		if (Event->is<sf::Event::Closed>())
			Window.close();

		if (const auto* KeyPressed = Event->getIf<sf::Event::KeyPressed>())
		{
			if (KeyPressed->code == sf::Keyboard::Key::Escape)
			{
				if (GameState == EGameState::Running)
					Pause(EGameState::PausedByUser, -1.f);

				else if (GameState == EGameState::PausedByUser)
					UnPause();
			}
		}
	}
}

void GameInstance::HandleIntersections()
{
	if (!BallInstance || GameState != EGameState::Running)
		return;

	for (const auto& Boundary : Boundaries)
	{
		if (Boundary)
		{
			if (BallInstance->GetShape().getGlobalBounds().findIntersection(Boundary->GetShape().getGlobalBounds()))
			{
				switch (Boundary->GetType())
				{
				case EBoundaryType::None:
					break;

				case EBoundaryType::Top:
				case EBoundaryType::Bottom:
					BallInstance->BounceVertical();
					break;

				case EBoundaryType::Left:
					BotScore += 1;
					BallInstance->BounceHorizontal();
					ResetRound();
					break;

				case EBoundaryType::Right:
					PlayerScore += 1;
					BallInstance->BounceHorizontal();
					ResetRound();
					break;

				default:
					break;
				}
			}

			if (PlayerPaddle && PlayerPaddle->GetShape().getGlobalBounds().findIntersection(Boundary->GetShape().getGlobalBounds()))
			{
				switch (Boundary->GetType())
				{
				case EBoundaryType::None:
					break;

				case EBoundaryType::Top:
					PlayerPaddle->BlockMovement(EMovementDirection::Up);
					break;

				case EBoundaryType::Bottom:
					PlayerPaddle->BlockMovement(EMovementDirection::Down);
					break;

				default:
					break;
				}
			}
		}
	}

	if (PlayerPaddle && BallInstance->GetShape().getGlobalBounds().findIntersection(PlayerPaddle->GetShape().getGlobalBounds()))
	{
		BallInstance->BounceHorizontal();
		PlayerPaddle->HandleHit();
	}

	if (BotPaddle && BallInstance->GetShape().getGlobalBounds().findIntersection(BotPaddle->GetShape().getGlobalBounds()))
	{
		BallInstance->BounceHorizontal();
		BotPaddle->HandleHit();
	}
}

void GameInstance::HandlePlayerInput(float DeltaTime)
{
	if (!PlayerPaddle)
		return;

	if (GameState == EGameState::Running)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			PlayerPaddle->Move(DeltaTime, EMovementDirection::Up);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			PlayerPaddle->Move(DeltaTime, EMovementDirection::Down);
	}
}

void GameInstance::Tick(float DeltaTime)
{
	if (!PlayerPaddle || !BotPaddle || !BallInstance)
		return;

	if (GameState == EGameState::Running)
	{
		HandlePlayerInput(DeltaTime);

		PlayerPaddle->Update(DeltaTime);
		BotPaddle->Update(DeltaTime);
		BallInstance->Update(DeltaTime);
	}

	else
	{
		PauseTimer += DeltaTime;

		if (PauseDuration > 0 && PauseTimer >= PauseDuration)
		{
			UnPause();
		}
	}
}

void GameInstance::Render()
{
	Window.clear();

	if (BGSprite)
		Window.draw(*BGSprite);

	if (PlayerPaddle)
		PlayerPaddle->Draw(Window);

	if (BotPaddle)
		BotPaddle->Draw(Window);

	if (BallInstance)
		BallInstance->Draw(Window);

	if (!Boundaries.empty())
		for (const auto& Boundary : Boundaries)
			if (Boundary)
				Boundary->Draw(Window);

	Window.display();
}

void GameInstance::ResetRound()
{
	if (GameState == EGameState::Running)
	{
		PlayerPaddle->Reset();
		BallInstance->Reset();
		Pause(EGameState::PausedBetweenRounds, 2.f);
	}
}

void GameInstance::Pause(EGameState NewGameState, float Duration)
{
	if (GameState == NewGameState)
		return;

	GameState = NewGameState;
	PauseTimer = 0.f;
	PauseDuration = Duration;
}

void GameInstance::UnPause()
{
	if (GameState == EGameState::Running)
		return;

	GameState = EGameState::Running;
	PauseTimer = 0.f;
	PauseDuration = 0.f;
}
