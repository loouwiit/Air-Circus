#pragma once
#include <SFML/Graphics.hpp>
class Camera
{
public:
	sf::View& get_View();
	void set_Center(float x,float y);
	void set_Size(float x,float y);
	void set_Center(sf::Vector2f center);
	void set_Size(sf::Vector2f size);

private:
	sf::View self_View;
};
