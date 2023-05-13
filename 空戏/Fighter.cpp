#include "Fighter.hpp"
#include "vector.inl"

constexpr float PIf = (float)(3.14159265358979323846);

sf::Texture Fighter::Default_Texture{};
float(*&Fighter::sin)(float) = SinCos::sin;
float(*&Fighter::cos)(float) = SinCos::cos;

Fighter::Fighter()
{
	self_Sprite.setTexture(get_Default_Texture());
	self_Sprite.setOrigin(get_Default_Texture().getSize().x / 2.0f, get_Default_Texture().getSize().y / 2.0f);
	self_Sprite.setScale(3.0f, 3.0f);
	rotate(-PIf / 2);
}

void Fighter::set_Color(unsigned int Color)
{
	self_Sprite.setColor(sf::Color(Color));
}

void Fighter::set_Color(sf::Color Color)
{
	self_Sprite.setColor(Color);
}

float Fighter::get_Rotation()
{
	return self_Rotation;
}

void Fighter::set_Rotation(float rotation)
{
	self_Rotation = rotation;
	self_Sprite.setRotation(self_Rotation / PIf * 180.f);
	self_Rotation_SinCos[0] = sin(self_Rotation);
	self_Rotation_SinCos[1] = cos(self_Rotation);
}

void Fighter::rotate(float rotation)
{
	self_Rotation += rotation;
	self_Sprite.setRotation(self_Rotation / PIf * 180.f);
	self_Rotation_SinCos[0] = sin(self_Rotation);
	self_Rotation_SinCos[1] = cos(self_Rotation);
}

float Fighter::get_Sin_Rotate()
{
	return self_Rotation_SinCos[0];
}

float Fighter::get_Cos_Rotate()
{
	return self_Rotation_SinCos[1];
}

void Fighter::set_Position(float x, float y)
{
	self_Position.x = x;
	self_Position.y = y;
	self_Sprite.setPosition(self_Position);
}

void Fighter::set_Key(unsigned i, bool flag)
{
	self_Key[i] = flag;
}

bool Fighter::get_Key(unsigned i)
{
	return self_Key[i];
}

void Fighter::set_Key(Key i, bool flag)
{
	self_Key[(unsigned)i] = flag;
}

bool Fighter::get_Key(Key i)
{
	return self_Key[(unsigned)i];
}

sf::Vector2f Fighter::get_Velocity()
{
	return self_Velocity;
}

sf::Vector2f Fighter::get_Position()
{
	return self_Position;
}

void Fighter::set_Velocity(sf::Vector2f velocity)
{
	self_Velocity = velocity;
	self_Acceleration.x = 0;
	self_Acceleration.y = 0;
}

void Fighter::set_Velocity(float x, float y)
{
	self_Velocity.x = x;
	self_Velocity.y = y;
	self_Acceleration.x = 0;
	self_Acceleration.y = 0;
}

void Fighter::froce(sf::Vector2f acceleration, float time)
{
	float ts = time * time * 0.5f;

	self_Acceleration += acceleration;
	self_Position += acceleration * ts;
}

void Fighter::froce(float x, float y, float time)
{
	float ts = time * time * 0.5f;

	self_Acceleration.x += x;
	self_Acceleration.y += y;
	self_Position.x += x * ts;
	self_Position.y += y * ts;
}

void Fighter::move(float delta_Time)
{
	if (self_Key[(unsigned)Key::Forward] != self_Key_old[(unsigned)Key::Forward])
	{
		self_Key_old[(unsigned)Key::Forward] = self_Key[(unsigned)Key::Forward];
		froce((self_Key_old[(unsigned)Key::Forward] ? +250.f : -250) * self_Rotation_SinCos[1],
			(self_Key_old[(unsigned)Key::Forward] ? +250.f : -250) * self_Rotation_SinCos[0], delta_Time);
	}
	if (self_Key[(unsigned)Key::Back] != self_Key_old[(unsigned)Key::Back])
	{
		self_Key_old[(unsigned)Key::Back] = self_Key[(unsigned)Key::Back];
		froce((self_Key_old[(unsigned)Key::Back] ? -250.f : +250) * self_Rotation_SinCos[1],
			(self_Key_old[(unsigned)Key::Back] ? -250.f : +250) * self_Rotation_SinCos[0], delta_Time);
	}
	if (self_Key[(unsigned)Key::Left] != self_Key_old[(unsigned)Key::Left])
	{
		self_Key_old[(unsigned)Key::Left] = self_Key[(unsigned)Key::Left];
		froce((self_Key_old[(unsigned)Key::Left] ? +250.f : -250) * self_Rotation_SinCos[0],
			(self_Key_old[(unsigned)Key::Left] ? -250.f : +250) * self_Rotation_SinCos[1], delta_Time);
	}
	if (self_Key[(unsigned)Key::Right] != self_Key_old[(unsigned)Key::Right])
	{
		self_Key_old[(unsigned)Key::Right] = self_Key[(unsigned)Key::Right];
		froce((self_Key_old[(unsigned)Key::Right] ? -250.f : +250) * self_Rotation_SinCos[0],
			(self_Key_old[(unsigned)Key::Right] ? +250.f : -250) * self_Rotation_SinCos[1], delta_Time);
	}
	if (self_Key[(unsigned)Key::Turn_Left]) rotate(-1.0f * delta_Time);
	if (self_Key[(unsigned)Key::Turn_Right]) rotate(+1.0f * delta_Time);

	self_Position += self_Velocity * delta_Time;
	self_Velocity += self_Acceleration * delta_Time;

	self_Sprite.setPosition(self_Position);
}

sf::Texture& Fighter::get_Default_Texture()
{
	if (Default_Texture.getSize().x!= 0)
		return Default_Texture;

	{
		std::cout << "Fighter::Get_Default_Texture:texture Rendering\n";

		struct Point
		{
			float x;
			float y;
		};
		constexpr char Point_Number = 8;
		//constexpr Point Points[Point_Number] = { {32.f,0.f},{63.f,63.f},{46.f,63.f},{46.f,125.f},{18.f,125.f},{18.f,63.f},{0.f,63.f},{32.f,0.f} };
		constexpr Point Points[Point_Number] = { {127.f,32.f},{63.f,63.f},{63.f,45.f},{0.f,45.f},{0.f,18.f},{63.f,18.f},{63.f,0.f},{127.f,32.f} };

		sf::RenderTexture texture;
		sf::ConvexShape convex;

		texture.create(128, 64);
		texture.clear(sf::Color::Transparent);
		convex.setPointCount(Point_Number);
		for (char i = 0; i < Point_Number; i++)
			convex.setPoint(i, sf::Vector2f(Points[i].x, Points[i].y));
		convex.setFillColor(sf::Color::White);
		texture.draw(convex);

		Default_Texture = texture.getTexture();
		Default_Texture.setSmooth(true);
	}

	return Default_Texture;
}

void Fighter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Sprite, states);
}
