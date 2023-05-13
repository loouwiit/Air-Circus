#pragma once
#include <SFML/Graphics.hpp>
class Collideable
{
public:
	bool is_Collide(Collideable& B);
	virtual sf::FloatRect get_Collision_Box() = 0;
};

class Buoy : public Collideable, public sf::Drawable
{
public:
	Buoy();
	void set_Position(float x, float y);

	sf::FloatRect get_Collision_Box();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::CircleShape self_Circle;
};

