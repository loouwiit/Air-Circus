#include "Collideable.hpp"

bool Collideable::is_Collide(Collideable& B)
{
	return this->get_Collision_Box().intersects(B.get_Collision_Box());
}

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

sf::FloatRect Buoy::get_Collision_Box()
{
	return self_Circle.getGlobalBounds();
}

void Buoy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Circle);
}

