#include "Core/GameInstance.h"

void GameInstance::Init()
{
	sf::RenderWindow Window(sf::VideoMode({ 800, 600 }), "UnrealPongine_Haponov");

	while (Window.isOpen())
	{
		while (const std::optional Event = Window.pollEvent())
		{
			if (Event->is<sf::Event::Closed>())
				Window.close();
		}
	}
}