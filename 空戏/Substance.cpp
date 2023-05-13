#include "Substance.hpp"

Buoy::Buoy()
{
	self_Circle.setRadius(100.f);
	self_Circle.setFillColor(sf::Color::Green);
	self_Circle.setPointCount(20);
	self_Circle.setOrigin(self_Circle.getRadius(), self_Circle.getRadius());
}

void Buoy::set_Position(float x, float y)
{
	self_Circle.setPosition(x, y);
}

void Buoy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Circle);
}
