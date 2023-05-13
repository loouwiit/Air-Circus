#pragma once
#include <SFML/Graphics.hpp>
class Substance : public sf::Drawable
{
public:


protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private:

};

class Buoy : public Substance
{
public:
	Buoy();
	void set_Position(float x, float y);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::CircleShape self_Circle;
};

