#pragma once
#include <SFML/Graphics.hpp>

namespace SinCos
{
	extern float(*sin)(float);
	extern float(*cos)(float);
}

class Collideable
{
public:
	float get_Mass();
	void set_Mass(float Mass);

	void collide(Collideable& B, float delta_Time);
	float distance(Collideable& B);
	float distance_Square(Collideable& B);

	bool is_Collide(Collideable& B);

	virtual sf::Vector2f get_Position() = 0;
	virtual sf::Vector2f get_Velocity() = 0;
	virtual sf::FloatRect get_Collision_Box() = 0;
	virtual void change_Velocity(sf::Vector2f delta_Velocity, float time = 0) = 0;

protected:
	float self_Mass = 0;

private:
	float abs(sf::Vector2f V);
};

class Buoy : public Collideable, public sf::Drawable
{
public:
	Buoy();
	void set_Position(float x, float y);
	sf::Vector2f get_Position();

	sf::Vector2f get_Velocity();
	sf::FloatRect get_Collision_Box();
	void change_Velocity(sf::Vector2f delta_Velocity, float time = 0);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::CircleShape self_Circle;
};

class Point : public sf::Drawable
{
public:
	Point();

	void set_Position(float x, float y);
	void set_Color(unsigned int Color);
	void set_Color(sf::Color Color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape self_Renctangle;
};
