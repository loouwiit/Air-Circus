#include "Camera.hpp"

sf::View& Camera::get_View()
{
	return self_View;
}

void Camera::set_Center(float x, float y)
{
	self_View.setCenter(x, y);
}

void Camera::set_Size(float x, float y)
{
	self_View.setSize(x, y);
}

void Camera::set_Center(sf::Vector2f center)
{
	self_View.setCenter(center);
}

void Camera::set_Size(sf::Vector2f size)
{
	self_View.setSize(size);
}
