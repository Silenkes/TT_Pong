#pragma once

#include <SFML/Graphics.hpp>

// * Base class for all entities placed in the game window
class Entity
{
public:
	virtual ~Entity() = default;

	virtual void Update(float DeltaTime) { };
	virtual void Draw(sf::RenderWindow& Window) = 0;
	virtual void Reset() { };

protected:
	sf::Color DefaultColor = sf::Color(255, 255, 255, 255);
	sf::Vector2f StartPosition = sf::Vector2f(0.f, 0.f);
};