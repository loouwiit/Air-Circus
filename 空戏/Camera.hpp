#pragma once
#include <SFML/Graphics.hpp>
class Camera
{
public:
	//sf::View& get_View();
	void set_Target(sf::RenderTarget* target);
	void compute();

	void set_Center(float x,float y);
	void set_Size(float x,float y);
	void set_Center(sf::Vector2f center);
	void set_Size(sf::Vector2f size);
	
	sf::Vector2f get_Size();
	sf::Vector2f get_Center();

private:
	sf::Vector2f self_Target_Size;
	sf::Vector2f self_Target_Center;
	sf::Vector2f self_Size;
	sf::Vector2f self_Center;
	sf::View self_View;
	sf::RenderTarget* self_Target;
};
