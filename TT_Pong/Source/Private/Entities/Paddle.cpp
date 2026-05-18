#include "Entities/Paddle.h"

Paddle::Paddle(const sf::Vector2f& StartPosition, const sf::Color& InDefaultColor, const sf::Color& InHitFlashColor)
{
	Shape.setSize({ 20.f, 80.f });
	Shape.setOrigin(Shape.getSize() / 2.f);
	Shape.setPosition(StartPosition);
	Shape.setFillColor(InDefaultColor);
	Shape.setOutlineColor(sf::Color::Black);
	Shape.setOutlineThickness(5.f);

	DefaultColor = InDefaultColor;
	HitFlashColor = InHitFlashColor;
}

void Paddle::Move(float DeltaTime, EMovementDirection Direction)
{
	if (Direction == EMovementDirection::None)
		return;

	float DirectionValue = 0.f;

	switch (Direction)
	{
	case EMovementDirection::Up:
		DirectionValue = -1.f;
		break;

	case EMovementDirection::Down:
		DirectionValue = 1.f;
		break;

	default:
		break;
	}

	Shape.move({ 0.f, DirectionValue * MoveSpeed * DeltaTime });
}

void Paddle::Update(float DeltaTime)
{
	if (!bIsFlashing)
		return;

	FlashTimer -= DeltaTime;

	if (FlashTimer <= 0.f)
	{
		bIsFlashing = false;
		Shape.setFillColor(DefaultColor);
	}
}

void Paddle::HandleHit()
{
	if (bIsFlashing)
		return;

	bIsFlashing = true;
	FlashTimer = FlashDuration;

	Shape.setFillColor(HitFlashColor);
}
