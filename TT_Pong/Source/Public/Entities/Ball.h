#pragma once

#include "Core/Entity.h"

class Ball : public Entity
{
public:
	Ball(const sf::Vector2f& Position, float InRadius, const sf::Color& InColor);

	virtual void Draw(sf::RenderWindow& Window) override;
	virtual void Update(float DeltaTime) override;
	virtual void Reset() override;
	void BounceVertical();
	void BounceHorizontal();

	const sf::CircleShape& GetShape() const { return Shape; }

private:
	sf::CircleShape Shape;
	sf::Vector2f Velocity;
	float MoveSpeed = 500.f;
};