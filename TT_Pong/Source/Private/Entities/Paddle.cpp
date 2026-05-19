#include "Entities/Paddle.h"

Paddle::Paddle(const sf::Vector2f& Position, const sf::Color& InDefaultColor, const sf::Color& InHitFlashColor)
{
	Shape.setSize({ 20.f, 80.f });
	Shape.setOrigin(Shape.getSize() / 2.f);
	Shape.setPosition(Position);
	Shape.setFillColor(InDefaultColor);
	Shape.setOutlineColor(sf::Color::Black);
	Shape.setOutlineThickness(5.f);

	DefaultColor = InDefaultColor;
	HitFlashColor = InHitFlashColor;
	StartPosition = Position;
}

void Paddle::Draw(sf::RenderWindow& Window)
{
	if (Window.isOpen())
		Window.draw(Shape);
}

void Paddle::Update(float DeltaTime)
{
	bBlockedDown = false;
	bBlockedUp = false;

	if (bIsFlashing)
	{
		FlashTimer -= DeltaTime;

		if (FlashTimer <= 0.f)
		{
			bIsFlashing = false;
			Shape.setFillColor(DefaultColor);
		}
	}
}

void Paddle::Reset()
{
	Shape.setPosition(StartPosition);
	Shape.setFillColor(DefaultColor);
	bBlockedUp = false;
	bBlockedDown = false;
	bIsFlashing = false;
}

void Paddle::Move(float DeltaTime, EMovementDirection Direction)
{
	float DirectionValue = 0.f;

	if (Direction == EMovementDirection::None)
		return;

	if (Direction == EMovementDirection::Up && !bBlockedUp)
		DirectionValue = -1.f;

	if (Direction == EMovementDirection::Down && !bBlockedDown)
		DirectionValue = 1.f;

	Shape.move({ 0.f, DirectionValue * MoveSpeed * DeltaTime });
}

void Paddle::HandleHit()
{
	if (bIsFlashing)
		return;

	bIsFlashing = true;
	FlashTimer = FlashDuration;

	Shape.setFillColor(HitFlashColor);
}

void Paddle::BlockMovement(EMovementDirection DirectionToBlock)
{
	switch (DirectionToBlock)
	{
	case EMovementDirection::None:
		break;

	case EMovementDirection::Up:
		bBlockedUp = true;
		break;

	case EMovementDirection::Down:
		bBlockedDown = true;
		break;

	default:
		break;
	}
}
