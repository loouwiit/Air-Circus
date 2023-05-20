#include "Camera.hpp"

//sf::View& Camera::get_View()
//{
//	return self_View;
//}

void Camera::set_Target(sf::RenderTarget* target)
{
	self_Target = target;
}

void Camera::compute()
{
	if (self_Center == self_Target_Center && self_Size == self_Target_Size) return;

	if (self_Target_Center.x - self_Center.x < 10) self_Center = self_Target_Center;
	else self_Center += (self_Target_Center - self_Center) * 0.05f;

	if (self_Target_Size.x - self_Size.x < 10) self_Size = self_Target_Size;
	else self_Size += (self_Target_Size - self_Size) * 0.05f;

	self_View.setCenter(self_Center);
	self_View.setSize(self_Size);
	self_Target->setView(self_View);
}

void Camera::set_Center(float x, float y)
{
	self_Target_Center.x = x;
	self_Target_Center.y = y;
}

void Camera::set_Size(float x, float y)
{
	self_Target_Size.x = x;
	self_Target_Size.y = y;
}

void Camera::set_Center(sf::Vector2f center)
{
	self_Target_Center = center;
}

void Camera::set_Size(sf::Vector2f size)
{
	self_Target_Size = size;
}

sf::Vector2f Camera::get_Size()
{
	return self_Target_Size;
}

sf::Vector2f Camera::get_Center()
{
	return self_Target_Center;
}
