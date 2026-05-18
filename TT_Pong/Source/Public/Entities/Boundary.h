#pragma once

#include <SFML/Graphics.hpp>

enum class EBoundaryType : uint8_t
{
	None,
	Top,
	Bottom,
	Left,
	Right,
};

class Boundary
{
public:
	Boundary(const sf::Vector2f& Position, const sf::Vector2f& Size, bool bVisible, EBoundaryType InType);
	const sf::RectangleShape& GetShape() const { return Shape; }
	EBoundaryType GetType() const { return Type; }

private:
	sf::RectangleShape Shape;
	EBoundaryType Type = EBoundaryType::None;
};