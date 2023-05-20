#include "Collideable.hpp"

constexpr float PIf = (float)(3.14159265358979323846);

//inline float test(sf::Vector2f v)
//{
//	return v.x * v.x + v.y * v.y;
//}

float Collideable::get_Mass()
{
	return self_Mass;
}

void Collideable::set_Mass(float Mass)
{
	self_Mass = Mass;
}

void Collideable::collide(Collideable& B, float delta_Time)
{
	if (!is_Collide(B)) return;

	sf::Vector2f a_Velocity = get_Velocity();
	sf::Vector2f b_Velocity = B.get_Velocity();

	float a_Mass = get_Mass();
	float b_Mass = B.get_Mass();

	float sin = 0;
	float cos = 0;

	sf::Vector2f a_Base_Velocity{}; //转换后的速度
	sf::Vector2f b_Base_Velocity{};

	sf::Vector2f a_New_Base{}; //转换后的碰撞速度
	sf::Vector2f b_New_Base{};

	sf::Vector2f a_New{}; //最终的速度
	sf::Vector2f b_New{};

	//SinCos
	{
		sf::Vector2f delta_Position = get_Position() - B.get_Position();
		float theta = atanf(-delta_Position.y / delta_Position.x);
		sin = ::sin(theta);
		cos = ::cos(theta);

		//printf("Collieable::colliede:P at (%f,%f) and (%f,%f)\n", get_Position().x, get_Position().y, B.get_Position().x, B.get_Position().y);
		//printf("\t theta is %f, %f in degree\n", theta, theta * 180 / PIf);
		//printf("\t sin is %f, cos is %f\n", sin, cos);
	}

	//base
	{
		a_Base_Velocity.x = +a_Velocity.x * cos - a_Velocity.y * sin;
		a_Base_Velocity.y = +a_Velocity.x * sin + a_Velocity.y * cos;
		b_Base_Velocity.x = +b_Velocity.x * cos - b_Velocity.y * sin;
		b_Base_Velocity.y = +b_Velocity.x * sin + b_Velocity.y * cos;
	}

	a_New_Base.y = a_Base_Velocity.y;
	b_New_Base.y = b_Base_Velocity.y;

	if (a_Mass != 0 && b_Mass != 0)
	{
		//均有限质量
		float totol_Mass = a_Mass + b_Mass;

		a_New_Base.x = (a_Mass - b_Mass) / totol_Mass * a_Base_Velocity.x + 2 * b_Mass / totol_Mass * b_Base_Velocity.x;
		b_New_Base.x = (b_Mass - a_Mass) / totol_Mass * b_Base_Velocity.x + 2 * a_Mass / totol_Mass * a_Base_Velocity.x;

		//a_New_Base.x = a_Base_Velocity.x;
		//b_New_Base.x = b_Base_Velocity.x;

		//a_New_Base.x = b_Base_Velocity.x;
		//b_New_Base.x = a_Base_Velocity.x;
	}
	else if (a_Mass == 0 && b_Mass != 0)
	{
		//自己无穷大
		a_New_Base.x = a_Base_Velocity.x;
		b_New_Base.x = -b_Base_Velocity.x;
	}
	else if (a_Mass != 0 && b_Mass == 0)
	{
		//对面无穷大
		a_New_Base.x = -a_Base_Velocity.x;
		b_New_Base.x = b_Base_Velocity.x;
	}
	else if (a_Mass == 0 && b_Mass == 0)
	{
		//都无穷大
		a_New_Base.x = -a_Base_Velocity.x;
		b_New_Base.x = -b_Base_Velocity.x;
	}

	//New
	{
		a_New.x = +a_New_Base.x * cos + a_New_Base.y * sin;
		a_New.y = -a_New_Base.x * sin + a_New_Base.y * cos;
		b_New.x = +b_New_Base.x * cos + b_New_Base.y * sin;
		b_New.y = -b_New_Base.x * sin + b_New_Base.y * cos;
	}

	change_Velocity(a_New - a_Velocity,delta_Time);
	B.change_Velocity(b_New - b_Velocity, delta_Time);

	//printf("\tP^2 %f to %f\n", a_Mass * a_Mass * test(a_Velocity) + b_Mass * b_Mass * test(b_Velocity), a_Mass * a_Mass * test(a_New) + b_Mass * b_Mass * test(b_New));
}

float Collideable::distance(Collideable& B)
{
	return sqrt(distance_Square(B));
}

float Collideable::distance_Square(Collideable& B)
{
	return abs(get_Position() - B.get_Position());
}

bool Collideable::is_Collide(Collideable& B)
{
	return this->get_Collision_Box().intersects(B.get_Collision_Box());
}

float Collideable::abs(sf::Vector2f V)
{
	return V.x * V.x + V.y * V.y;
}

Buoy::Buoy()
{
	self_Circle.setRadius(100.f);
	self_Circle.setFillColor(sf::Color::Green);
	self_Circle.setPointCount(20);
	self_Circle.setOrigin(self_Circle.getRadius(), self_Circle.getRadius());
	self_Mass = 0;
}

void Buoy::set_Position(float x, float y)
{
	self_Circle.setPosition(x, y);
}

sf::Vector2f Buoy::get_Position()
{
	return self_Circle.getPosition();
}

sf::Vector2f Buoy::get_Velocity()
{
	return sf::Vector2f(0, 0);
}

sf::FloatRect Buoy::get_Collision_Box()
{
	return self_Circle.getGlobalBounds();
}

void Buoy::change_Velocity(sf::Vector2f delta_Velocity, float time)
{}

void Buoy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Circle);
#ifdef _DEBUG
	static sf::RectangleShape box;
	box.setPosition(self_Circle.getGlobalBounds().left, self_Circle.getGlobalBounds().top);
	box.setSize(sf::Vector2f(self_Circle.getGlobalBounds().width, self_Circle.getGlobalBounds().height));
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(self_Circle.getFillColor());
	box.setOutlineThickness(20);
	target.draw(box);
#endif
}

Point::Point()
{
	self_Renctangle.setSize(sf::Vector2f(20, 20));
}

void Point::set_Position(float x, float y)
{
	self_Renctangle.setPosition(x, y);
}

void Point::set_Color(unsigned int Color)
{
	self_Renctangle.setFillColor(sf::Color(Color));
}

void Point::set_Color(sf::Color Color)
{
	self_Renctangle.setFillColor(Color);
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Renctangle);
}
