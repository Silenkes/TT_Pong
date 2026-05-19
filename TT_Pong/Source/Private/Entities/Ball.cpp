#include "Entities/Ball.h"
#include <random>

Ball::Ball(const sf::Vector2f& Position, float InRadius, const sf::Color& InColor)
{
	Shape.setPosition(Position);
	Shape.setRadius(InRadius);
	Shape.setFillColor(InColor);
	Shape.setOrigin({ InRadius, InRadius });
	Shape.setOutlineColor(sf::Color::Black);
	Shape.setOutlineThickness(3.f);

	// ----- Calculating random ball velocity direction -----------|

	std::random_device RandomDevice;
	std::mt19937 Generator(RandomDevice());
	std::uniform_int_distribution<int> Distribution(0, 1);

	const float RandomX = Distribution(Generator) == 0 ? -1.f : 1.f;
	const float RandomY = Distribution(Generator) == 0 ? -1.f : 1.f;
	const float RandomYMultiplier = Distribution(Generator) == 0 ? 0.5f : 1.5f;

	// ------------------------------------------------------------|

	Velocity = { RandomX, RandomY * RandomYMultiplier};
	StartPosition = Position;
}

void Ball::Draw(sf::RenderWindow& Window)
{
	if (Window.isOpen())
		Window.draw(Shape);
}

void Ball::Update(float DeltaTime)
{
	Shape.move(Velocity * MoveSpeed * DeltaTime);
}

void Ball::Reset()
{
	Shape.setPosition(StartPosition);
	
}

void Ball::BounceVertical()
{
	Velocity.y *= -1.f;
}

void Ball::BounceHorizontal()
{
	Velocity.x *= -1.f;
}
