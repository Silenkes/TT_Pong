#pragma once

#include "Core/Entity.h"

enum class EBoundaryType : uint8_t
{
	None,
	Top,
	Bottom,
	Left,
	Right,
};

class Boundary : public Entity
{
public:
	Boundary(const sf::Vector2f& Position, const sf::Vector2f& Size, bool bVisible, EBoundaryType InType);

	virtual void Draw(sf::RenderWindow& Window) override;
	const sf::RectangleShape& GetShape() const { return Shape; }
	EBoundaryType GetType() const { return Type; }

private:
	sf::RectangleShape Shape;
	EBoundaryType Type = EBoundaryType::None;
};