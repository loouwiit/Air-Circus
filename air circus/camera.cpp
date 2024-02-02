#include "camera.hpp"

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

	if (abs(self_Target_Center - self_Center) < 100) self_Center = self_Target_Center;
	else self_Center += (self_Target_Center - self_Center) * 0.05f;

	if (abs(self_Target_Size - self_Size) < 100) self_Size = self_Target_Size;
	else self_Size += (self_Target_Size - self_Size) * 0.05f;

	self_View.setCenter(self_Center);
	self_View.setSize(self_Size);
	self_Target->setView(self_View);
}

void Camera::set_Proportion(float Proportion)
{
	self_Proportion = Proportion;
	//printf("Camera::set_Proportion: Proportion set to %f\n", self_Proportion);
	compute_Target();
}

sf::Vector2f Camera::get_Center()
{
	return self_Center;
}

bool Camera::intersects(sf::FloatRect Rect)
{
	return sf::FloatRect(self_Center - self_Size * 0.5f, self_Size).intersects(Rect);
}

sf::Vector2f Camera::get_Size()
{
	return self_Size;
}

bool Camera::contains(sf::Vector2f V)
{
	return sf::FloatRect(self_Center - self_Size * 0.5f, self_Size).contains(V);
}

void Camera::set_Position(sf::Vector2f LeftTop,sf::Vector2f RightBotton)
{
	self_LeftTop = LeftTop;
	self_RightBotton = RightBotton;
	compute_Target();
}

//void Camera::set_Center(float x, float y)
//{
//	self_Target_Center.x = x;
//	self_Target_Center.y = y;
//}
//
//void Camera::set_Size(float x, float y)
//{
//	self_Target_Size.x = x;
//	self_Target_Size.y = y;
//}
//
//void Camera::set_Center(sf::Vector2f center)
//{
//	self_Target_Center = center;
//}
//
//void Camera::set_Size(sf::Vector2f size)
//{
//	self_Target_Size = size;
//}
//
//sf::Vector2f Camera::get_Size()
//{
//	return self_Target_Size;
//}
//
//sf::Vector2f Camera::get_Center()
//{
//	return self_Target_Center;
//}

void Camera::set_Is_Full(bool flag)
{
	self_Is_Full = flag;
}

bool Camera::is_Full()
{
	return self_Is_Full;
}

void Camera::compute_Target()
{
	self_Target_Size.x = self_RightBotton.x - self_LeftTop.x + 1000;
	self_Target_Size.y = self_RightBotton.y - self_LeftTop.y + 1000;
	self_Target_Center = (self_RightBotton + self_LeftTop) * 0.5f;

	if (self_Target_Size.y / self_Target_Size.x > self_Proportion)
	{
		self_Target_Size.x = self_Target_Size.y / self_Proportion;
	}
	else
	{
		self_Target_Size.y = self_Target_Size.x * self_Proportion;
	}
}
