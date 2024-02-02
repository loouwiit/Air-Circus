#pragma once
#include <SFML/Graphics.hpp>
#include "vector.inl"

class Camera
{
public:
	//sf::View& get_View();
	void set_Target(sf::RenderTarget* target);
	void compute();

	void set_Proportion(float Proportion);
	void set_Position(sf::Vector2f LeftTop, sf::Vector2f RightBotton);

	bool intersects(sf::FloatRect Rect);
	bool contains(sf::Vector2f V);
	sf::Vector2f get_Center();
	sf::Vector2f get_Size();

	void set_Is_Full(bool flag);
	bool is_Full();

private:
	sf::Vector2f self_LeftTop;
	sf::Vector2f self_RightBotton;

	sf::Vector2f self_Target_Size;
	sf::Vector2f self_Target_Center;
	sf::Vector2f self_Size;
	sf::Vector2f self_Center;

	sf::View self_View;
	sf::RenderTarget* self_Target;

	float self_Proportion = 9.f / 16.f;
	bool self_Is_Full = false;

	void compute_Target();
};
