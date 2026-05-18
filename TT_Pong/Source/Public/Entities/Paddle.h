#pragma once

#include <SFML/Graphics.hpp>

enum class EMovementDirection : uint8_t
{
	None,
	Up,
	Down,
};

class Paddle
{
public:
	Paddle(const sf::Vector2f& StartPosition, const sf::Color& InDefaultColor, const sf::Color& InHitFlashColor);

	void Move(float DeltaTime, EMovementDirection Direction);
	void Update(float DeltaTime);
	void HandleHit();

	const sf::RectangleShape& GetShape() const { return Shape; }

private:
	sf::Color DefaultColor;
	sf::Color HitFlashColor;
	sf::RectangleShape Shape;

	bool bIsFlashing = false;
	float FlashTimer = 0.f;
	float FlashDuration = 0.4f;
	float MoveSpeed = 1000.f;
};