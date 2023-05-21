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
