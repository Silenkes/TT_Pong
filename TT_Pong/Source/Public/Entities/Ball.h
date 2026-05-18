#pragma once

#include <SFML/Graphics.hpp>

class Ball
{
public:
	Ball(const sf::Vector2f& StartPosition, float InRadius, const sf::Color& InColor);

	void Update(float DeltaTime);
	void BounceVertical();
	void BounceHorizontal();

	const sf::CircleShape& GetShape() const { return Shape; }

private:
	sf::CircleShape Shape;
	sf::Vector2f Velocity;
	float MoveSpeed = 500.f;
};