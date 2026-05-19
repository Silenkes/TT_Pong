#pragma once

#include "Core/Entity.h"

enum class EMovementDirection : uint8_t
{
	None,
	Up,
	Down,
};

class Paddle : public Entity
{
public:
	Paddle(const sf::Vector2f& Position, const sf::Color& InDefaultColor, const sf::Color& InHitFlashColor);

	virtual void Draw(sf::RenderWindow& Window) override;
	virtual void Update(float DeltaTime) override;
	virtual void Reset() override;
	void Move(float DeltaTime, EMovementDirection Direction);
	void HandleHit();
	void BlockMovement(EMovementDirection DirectionToBlock);

	const sf::RectangleShape& GetShape() const { return Shape; }

private:
	sf::Color DefaultColor;
	sf::Color HitFlashColor;
	sf::RectangleShape Shape;

	bool bBlockedUp = false;
	bool bBlockedDown = false;
	bool bIsFlashing = false;
	float FlashTimer = 0.f;
	float FlashDuration = 0.6f;
	float MoveSpeed = 1000.f;
};