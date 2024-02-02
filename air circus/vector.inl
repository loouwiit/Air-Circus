#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

inline sf::Vector2f operator*(sf::Vector2f v1, sf::Vector2f v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	return v1;
}

inline sf::Vector2f& operator*=(sf::Vector2f& v, float fact)
{
	v.x *= fact;
	v.y *= fact;
	return v;
}

inline float abss(sf::Vector2f V)
{
	return V.x * V.x + V.y * V.y;
}

inline float abs(sf::Vector2f V)
{
	return sqrt(V.x * V.x + V.y * V.y);
}

inline sf::Vector2f rotate(sf::Vector2f V, float sin, float cos)
{
	sf::Vector2f out;
	out.x = V.x * cos - V.y * sin;
	out.y = V.x * sin + V.y * cos;
	return out;
}
