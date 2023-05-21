#pragma once
#include <SFML/System/Vector2.hpp>


//inline sf::Vector2f& operator*(float& n, sf::Vector2f& v)
//{
//	v.x *= n;
//	v.y *= n;
//	return v;
//}

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
