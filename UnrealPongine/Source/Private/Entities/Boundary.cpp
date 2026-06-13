#include "Entities/Boundary.h"

Boundary::Boundary(const sf::Vector2f& Position, const sf::Vector2f& Size, bool bVisible, EBoundaryType InType) : Type(InType)
{
	Shape.setPosition(Position);
	Shape.setSize(Size);
	bVisible ? Shape.setFillColor(DefaultColor) : Shape.setFillColor(sf::Color(255, 255, 255, 0));

	StartPosition = Position;
}

void Boundary::Draw(sf::RenderWindow& Window)
{
	if (Window.isOpen())
		Window.draw(Shape);
}
