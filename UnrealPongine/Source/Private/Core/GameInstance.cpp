#include "Core/GameInstance.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Entities/Boundary.h"
#include "Audio/SoundBase.h"
#include <iostream>
#include <random>

GameInstance::GameInstance() = default;
GameInstance::~GameInstance() = default;

void GameInstance::Init()
{
	if (bInitialized)
		return;

	bInitialized = true;

	Window.create(sf::VideoMode({ 1280, 720 }), "UnrealPongine");

	WindowSizeY = Window.getSize().y;
	WindowSizeX = Window.getSize().x;
	GameState = EGameState::PausedByUser;
	
	CreateBG();
	CreateEntities();
	SetBGSize();
	CreateUI();
	SetPlayerControls();
	SoundSystem = std::make_unique<SoundBase>();

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

void GameInstance::CreateUI()
{
	TextFont = std::make_unique<sf::Font>();

	if (!TextFont->openFromFile("Assets/Fonts/Chomage.ttf"))
	{
		std::cout << "Chomage font not found!" << std::endl;
		return;
	}

	PlayerScoreText = std::make_unique<sf::Text>(*TextFont);
	BotScoreText = std::make_unique<sf::Text>(*TextFont);
	PauseText = std::make_unique<sf::Text>(*TextFont);
	BindActionText = std::make_unique<sf::Text>(*TextFont);

	PlayerScoreText->setFont(*TextFont);
	PlayerScoreText->setCharacterSize(56);
	PlayerScoreText->setFillColor(sf::Color::White);
	PlayerScoreText->setOutlineThickness(5.f);
	PlayerScoreText->setPosition({ WindowSizeX / 2 - 400, WindowSizeY / 2 - 300 });
	PlayerScoreText->setString(std::to_string(PlayerScore));

	BotScoreText->setFont(*TextFont);
	BotScoreText->setCharacterSize(56);
	BotScoreText->setFillColor(sf::Color::White);
	BotScoreText->setOutlineThickness(5.f);
	BotScoreText->setPosition({ WindowSizeX / 2 + 400, WindowSizeY / 2 - 300 });
	BotScoreText->setString(std::to_string(BotScore));

	PauseText->setFont(*TextFont);
	PauseText->setCharacterSize(120);
	PauseText->setFillColor(sf::Color::Yellow);
	PauseText->setOutlineThickness(10.f);
	PauseText->setLineAlignment(sf::Text::LineAlignment::Center);
	PauseText->setPosition({ WindowSizeX / 2, WindowSizeY / 2 - 150.f});
	PauseText->setString("PAUSE");

	BindActionText->setFont(*TextFont);
	BindActionText->setCharacterSize(72);
	BindActionText->setFillColor(sf::Color::Cyan);
	BindActionText->setOutlineThickness(7.f);
	BindActionText->setLineAlignment(sf::Text::LineAlignment::Center);
	BindActionText->setPosition({ WindowSizeX / 2, WindowSizeY / 2 + 230 });
	BindActionText->setString("Bindings");
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
	PlayerPaddle = std::make_unique<Paddle>(sf::Vector2f(WindowSizeX / 8.f, WindowSizeY / 2), sf::Color::Green, sf::Color::Magenta, 1000.f);
	BotPaddle = std::make_unique<Paddle>(sf::Vector2f(WindowSizeX / 1.15f, WindowSizeY / 2.f), sf::Color::Red, sf::Color::Yellow, 750.f);
	BallInstance = std::make_unique<Ball>(sf::Vector2f(WindowSizeX / 2.f, WindowSizeY / 2.f), 15.f, sf::Color(125, 34, 112, 255));

	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, 0.f), sf::Vector2f(WindowSizeX, 20.f), false, EBoundaryType::Top));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, WindowSizeY - 20.f), sf::Vector2f(WindowSizeX, 20.f), false, EBoundaryType::Bottom));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(0.f, 0.f), sf::Vector2f(20.f, WindowSizeY), false, EBoundaryType::Left));
	Boundaries.push_back(std::make_unique<Boundary>(sf::Vector2f(WindowSizeX - 20.f, 0.f), sf::Vector2f(20.f, WindowSizeY), false, EBoundaryType::Right));
}

void GameInstance::HandleEvents()
{
	while (const std::optional Event = Window.pollEvent())
	{
		if (Event->is<sf::Event::Closed>())
			Window.close();

		if (const auto* KeyPressed = Event->getIf<sf::Event::KeyPressed>())
		{
			if (KeyPressed->code == PlayerInputMapping[EPlayerInputs::Pause])
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
			// Ball intersection with boundary
			if (BallInstance->GetShape().getGlobalBounds().findIntersection(Boundary->GetShape().getGlobalBounds()))
			{
				switch (Boundary->GetType())
				{
				case EBoundaryType::None:
					break;

				case EBoundaryType::Top:
				case EBoundaryType::Bottom:
					BallInstance->BounceVertical();
					SoundSystem->Play(ESoundType::BoundaryHit);
					break;

				case EBoundaryType::Left:
					BotScore += 1;
					BallInstance->BounceHorizontal();
					SoundSystem->Play(ESoundType::BotGoal);
					ResetRound();
					break;

				case EBoundaryType::Right:
					PlayerScore += 1;
					BallInstance->BounceHorizontal();
					SoundSystem->Play(ESoundType::PlayerGoal);
					ResetRound();
					break;

				default:
					break;
				}
			}

			// Paddle intersection with boundary
			Paddle* OverlappedPaddle = nullptr;

			if (PlayerPaddle && Boundary->GetShape().getGlobalBounds().findIntersection(PlayerPaddle->GetShape().getGlobalBounds()))
				OverlappedPaddle = PlayerPaddle.get();

			else if (BotPaddle && Boundary->GetShape().getGlobalBounds().findIntersection(BotPaddle->GetShape().getGlobalBounds()))
				OverlappedPaddle = BotPaddle.get();

			if (OverlappedPaddle)
			{
				switch (Boundary->GetType())
				{
				case EBoundaryType::None:
					break;

				case EBoundaryType::Top:
					OverlappedPaddle->BlockMovement(EMovementDirection::Up);
					break;

				case EBoundaryType::Bottom:
					OverlappedPaddle->BlockMovement(EMovementDirection::Down);
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
		SoundSystem->Play(ESoundType::PlayerHit);
		PlayerPaddle->HandleHit();
	}

	if (BotPaddle && BallInstance->GetShape().getGlobalBounds().findIntersection(BotPaddle->GetShape().getGlobalBounds()))
	{
		BallInstance->BounceHorizontal();
		SoundSystem->Play(ESoundType::BotHit);
		BotPaddle->HandleHit();
	}
}

void GameInstance::HandlePlayerInput(float DeltaTime)
{
	if (!PlayerPaddle)
		return;

	if (GameState == EGameState::Running)
	{
		if (sf::Keyboard::isKeyPressed(PlayerInputMapping[EPlayerInputs::MoveUp]))
			PlayerPaddle->Move(DeltaTime, EMovementDirection::Up);

		if (sf::Keyboard::isKeyPressed(PlayerInputMapping[EPlayerInputs::MoveDown]))
			PlayerPaddle->Move(DeltaTime, EMovementDirection::Down);
	}
}

void GameInstance::HandleBotMovement(float DeltaTime)
{
	if (!BotPaddle || !BallInstance)
		return;

	std::random_device RandomDevice;
	std::mt19937 Generator(RandomDevice());
	std::uniform_int_distribution<int> Distribution(-100, 100);

	const float BallY = BallInstance->GetShape().getPosition().y;
	const float BotY = BotPaddle->GetShape().getPosition().y;
	const float Error = Distribution(Generator);

	if (BallInstance->GetShape().getPosition().x > WindowSizeX / 2)
	{
		if (BallY + Error < BotY + BotPaddle->GetShape().getSize().y / 2)
			BotPaddle->Move(DeltaTime, EMovementDirection::Up);

		else if (BallY + Error > BotY + BotPaddle->GetShape().getSize().y / 2)
			BotPaddle->Move(DeltaTime, EMovementDirection::Down);
	}
}

void GameInstance::Tick(float DeltaTime)
{
	if (!PlayerPaddle || !BotPaddle || !BallInstance)
		return;

	if (GameState == EGameState::Running)
	{
		HandlePlayerInput(DeltaTime);
		HandleBotMovement(DeltaTime);

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

	if (PlayerScoreText)
		Window.draw(*PlayerScoreText);

	if (BotScoreText)
		Window.draw(*BotScoreText);

	if (BindActionText)
		Window.draw(*BindActionText);

	if (PauseText && GameState == EGameState::PausedByUser)
		Window.draw(*PauseText);

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

void GameInstance::SetPlayerControls()
{
	BindActionText->setString("Press a key to bind: Move Up");
	PlayerInputMapping[EPlayerInputs::MoveUp] = HandlePlayerBinding();

	BindActionText->setString("Press a key to bind: Move Down");
	PlayerInputMapping[EPlayerInputs::MoveDown] = HandlePlayerBinding();

	BindActionText->setString("Press a key to bind: Pause");
	PlayerInputMapping[EPlayerInputs::Pause] = HandlePlayerBinding();

	BindActionText.reset();
}

sf::Keyboard::Key GameInstance::HandlePlayerBinding()
{
	while (Window.isOpen())
	{
		Render();

		while (const auto Event = Window.pollEvent())
		{
			if (Event->is<sf::Event::Closed>())
				Window.close();

			if (const auto* KeyPressed = Event->getIf<sf::Event::KeyPressed>())
			{
				return KeyPressed->code;
			}
		}
	}

	return sf::Keyboard::Key::Unknown;
}

void GameInstance::ResetRound()
{
	if (GameState == EGameState::Running)
	{
		if (BotScoreText)
			BotScoreText->setString(std::to_string(BotScore));

		if (PlayerScoreText)
			PlayerScoreText->setString(std::to_string(PlayerScore));

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
