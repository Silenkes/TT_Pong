#include "Core/GameInstance.h"
#include "Entities/Paddle.h"
#include "Entities/Ball.h"
#include "Entities/Boundary.h"
#include <iostream>

GameInstance::GameInstance()
{

}

void GameInstance::Init()
{
	if (bInitialized)
		return;

	bInitialized = true;

	Window.create(sf::VideoMode({ 1280, 720 }), "UnrealPongine_Haponov");

	WindowSizeY = Window.getSize().y;
	WindowSizeX = Window.getSize().x;

	CreateBG();
	CreateEntities();
	SetBGSize();

	while (Window.isOpen())
	{
		float DeltaTime = DeltaClock.restart().asSeconds();

		HandleEvents();
		Tick(DeltaTime);
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
	if (PlayerPaddle)
	{
		delete PlayerPaddle;
		PlayerPaddle = nullptr;
	}

	if (BotPaddle)
	{
		delete BotPaddle;
		BotPaddle = nullptr;
	}

	if (BallInstance)
	{
		delete BallInstance;
		BallInstance = nullptr;
	}
	
	PlayerPaddle = new Paddle(sf::Vector2f({ WindowSizeX / 8.f, WindowSizeY / 2.f }), sf::Color::Green, sf::Color::Magenta);
	BotPaddle = new Paddle(sf::Vector2f({ WindowSizeX / 1.15f, WindowSizeY / 2.f }), sf::Color::Red, sf::Color::Yellow);
	BallInstance = new Ball(sf::Vector2f({ WindowSizeX / 2.f, WindowSizeY / 2.f }), 15.f, sf::Color(125, 34, 112, 255));

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
	}
}

void GameInstance::Tick(float DeltaTime)
{
	if (!PlayerPaddle || !BotPaddle || !BallInstance)
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		PlayerPaddle->Move(DeltaTime, EMovementDirection::Up);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		PlayerPaddle->Move(DeltaTime, EMovementDirection::Down);

	PlayerPaddle->Update(DeltaTime);
	BotPaddle->Update(DeltaTime);
	BallInstance->Update(DeltaTime);
}

void GameInstance::Render()
{
	Window.clear();

	if (BGSprite)
		Window.draw(*BGSprite);

	if (PlayerPaddle)
		Window.draw(PlayerPaddle->GetShape());

	if (BotPaddle)
		Window.draw(BotPaddle->GetShape());

	if (BallInstance)
		Window.draw(BallInstance->GetShape());

	if (!Boundaries.empty())
		for (const auto& Boundary : Boundaries)
			if (Boundary)
				Window.draw(Boundary->GetShape());

	Window.display();
}

GameInstance::~GameInstance()
{
	delete PlayerPaddle;
	delete BotPaddle;
	delete BallInstance;
}
