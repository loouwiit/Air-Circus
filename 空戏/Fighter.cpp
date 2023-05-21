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
	self_Mass = 60;
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

void Fighter::set_Key(Key i, bool flag, int now_Time)
{
	if (flag == self_Key_Old[(Key_Base)i]) return; // 相同，记过了

	self_Key[(Key_Base)i] = flag;
	self_Key_Old[(Key_Base)i] = flag;
	self_Key_Filp[(Key_Base)i] = true;

	if (now_Time == 0) return; //没有意义，直接返回
	if (flag)
	{
		//pressed
		self_Key_Now_Start_Time[(Key_Base)i] = now_Time;
	}
	else
	{
		//release
		self_Key_Last_Time[1][(Key_Base)i][0] = self_Key_Last_Time[0][(Key_Base)i][0];
		self_Key_Last_Time[1][(Key_Base)i][1] = self_Key_Last_Time[0][(Key_Base)i][1]; //退一

		self_Key_Last_Time[0][(Key_Base)i][0] = self_Key_Now_Start_Time[(Key_Base)i]; //写一
		self_Key_Last_Time[0][(Key_Base)i][1] = now_Time;

		//printf("Fighter::set_Key: key %d relesed, press time: %d, last time %d\n", (Key_Base)i, self_Key_Last_Time[0][(Key_Base)i][1] - self_Key_Last_Time[0][(Key_Base)i][0], self_Key_Last_Time[0][(Key_Base)i][0] - self_Key_Last_Time[1][(Key_Base)i][0]);
	}
}

bool Fighter::get_Key(Key i)
{
	return self_Key[(Key_Base)i];
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
	self_Velocity = self_Velocity_old = velocity;
	//self_Acceleration.x = 0;
	//self_Acceleration.y = 0;
}

void Fighter::set_Velocity(float x, float y)
{
	self_Velocity.x = self_Velocity_old.x = x;
	self_Velocity.y = self_Velocity_old.y = y;
	//self_Acceleration.x = 0;
	//self_Acceleration.y = 0;
}

void Fighter::froce(sf::Vector2f acceleration, float time)
{
	float ts = time * time * 0.5f;

	//self_Acceleration += acceleration;
	self_Position += acceleration * ts;
	self_Velocity += acceleration * time;
}

void Fighter::froce(float x, float y, float time)
{
	float ts = time * time * 0.5f;

	//self_Acceleration.x += x;
	//self_Acceleration.y += y;
	self_Position.x += x * ts;
	self_Position.y += y * ts;

	self_Velocity.x += x * time;
	self_Velocity.y += y * time;
}

void Fighter::move(float delta_Time, int now_Time)
{
	compute(delta_Time, now_Time);
	if (abss(self_Velocity) > 5000000)
	{
		change_Velocity(self_Velocity * -0.05f);
		//printf("Fighter::Move: |Speed| = %f\n", abss(self_Velocity));
	}

	self_Position += self_Velocity_old * delta_Time;
	//self_Velocity += self_Acceleration * delta_Time;
	self_Velocity_old = self_Velocity;

	self_Sprite.setPosition(self_Position);
}

void Fighter::change_Velocity(sf::Vector2f delta_Velocity, float time)
{
	self_Velocity_old += delta_Velocity;
	self_Velocity += delta_Velocity;

	self_Position.x += delta_Velocity.x * time;
	self_Position.y += delta_Velocity.y * time;

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
		//constexpr char Point_Number = 8;
		//constexpr Point Points[Point_Number] = { {32.f,0.f},{63.f,63.f},{46.f,63.f},{46.f,125.f},{18.f,125.f},{18.f,63.f},{0.f,63.f},{32.f,0.f} };
		//constexpr Point Points[Point_Number] = { {127.f,32.f},{63.f,63.f},{63.f,45.f},{0.f,45.f},{0.f,18.f},{63.f,18.f},{63.f,0.f},{127.f,32.f} };

		constexpr char Point_Number = 3;
		constexpr Point Points[Point_Number] = { {0.f,63.f},{0.f,0.f},{63.f,31.f} };
		
		//constexpr char Point_Number = 4;
		//constexpr Point Points[Point_Number] = { {0.f,63.f},{0.f,0.f},{63.f,63.f},{63.f,0.f} };

		sf::RenderTexture texture;
		sf::ConvexShape convex;

		texture.create(64, 64);
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

sf::FloatRect Fighter::get_Collision_Box()
{
	return self_Sprite.getGlobalBounds();
}

void Fighter::compute(float delta_Time, int now_Time)
{
	using Type = Key_Type;

	if (is_Key(Key::Forward))
	{
		froce(+2000 * self_Rotation_SinCos[1], +2000 * self_Rotation_SinCos[0], delta_Time);
	}

	if (is_Key(Key::Back))
	{
		if (is_Key(Key::Back, now_Time, Type::Click, Type::Click, Type::Click) && self_Key_Filp[(Key_Base)Key::Back] == true)
		{
			//三击
			set_Velocity(-self_Velocity);
			//printf("Fighter::compute: triple back\n");
		}
		else if (is_Key(Key::Back, now_Time, Type::Pass, Type::Click))
		{
			//上次是点 减速
			froce(self_Velocity * -5.0f, delta_Time);
			//printf("Fighter::compute: double back\n");
		}
		else
		{
			//其他 后退
			froce(-500 * self_Rotation_SinCos[1], -500 * self_Rotation_SinCos[0], delta_Time);
		}
	}

	if (self_Key[(Key_Base)Key::Left])
	{
		rotate(-2.0f * delta_Time);
	}

	if (self_Key[(Key_Base)Key::Right])
	{
		rotate(+2.0f * delta_Time);
	}

	if (self_Key[(Key_Base)Key::Turn_Left])
	{
		froce(+500 * self_Rotation_SinCos[0], -500 * self_Rotation_SinCos[1], delta_Time);
	}

	if (self_Key[(Key_Base)Key::Turn_Right])
	{
		froce(-500 * self_Rotation_SinCos[0], +500 * self_Rotation_SinCos[1], delta_Time);
	}


	std::fill(self_Key_Filp, self_Key_Filp + Key_Number, false);
}

bool Fighter::is_Key(Key key,int now_Time, Key_Type T1, Key_Type T2, Key_Type T3)
{
	switch (T1)
	{
	case Fighter::Key_Type::Press:
		if (self_Key[(Key_Base)key] == false) return false;
		if (now_Time - self_Key_Now_Start_Time[(Key_Base)key] < 400) return false;//非长按
		break;

	case Fighter::Key_Type::Click:
		if (self_Key[(Key_Base)key] == false) return false;
		if (now_Time - self_Key_Now_Start_Time[(Key_Base)key] > 400) return false;//非点按
		break;

	case Fighter::Key_Type::On:
		if (self_Key[(Key_Base)key] == false) return false;
		break;

	case Fighter::Key_Type::Off:
		if (self_Key[(Key_Base)key] == true) return false;
		else return false; //既然off了就不用后续判断了

	default:
	case Fighter::Key_Type::None:
		return true; //既然none了就不用后续判断了 此时函数恒成立

	case Fighter::Key_Type::Pass:
		break;
	}

	switch (T2)
	{
	case Fighter::Key_Type::Press:
		if (self_Key_Now_Start_Time[(Key_Base)key] - self_Key_Last_Time[0][(Key_Base)key][1] > 1000) return false; //超时
		if (self_Key_Last_Time[0][(Key_Base)key][1] - self_Key_Last_Time[0][(Key_Base)key][0] < 400) return false;//非长按
		break;

	case Fighter::Key_Type::Click:
		if (self_Key_Now_Start_Time[(Key_Base)key] - self_Key_Last_Time[0][(Key_Base)key][1] > 1000) return false; //超时
		if (self_Key_Last_Time[0][(Key_Base)key][1] - self_Key_Last_Time[0][(Key_Base)key][0] > 400) return false;//非点按
		break;

	case Fighter::Key_Type::On:
		if (self_Key_Now_Start_Time[(Key_Base)key] - self_Key_Last_Time[0][(Key_Base)key][1] > 1000) return false; //超时
		break;

	case Fighter::Key_Type::Off:
		if (self_Key_Now_Start_Time[(Key_Base)key] - self_Key_Last_Time[0][(Key_Base)key][1] < 1000) return false; //近时
		else return true;

	default:
	case Fighter::Key_Type::None:
		return true;

	case Fighter::Key_Type::Pass:
		break;
	}

	switch (T3)
	{
	case Fighter::Key_Type::Press:
		if (self_Key_Last_Time[0][(Key_Base)key][0] - self_Key_Last_Time[1][(Key_Base)key][1] > 1000)return false; //超时
		if (self_Key_Last_Time[1][(Key_Base)key][1] - self_Key_Last_Time[1][(Key_Base)key][0] > 400) return false;//非点按
		break;

	case Fighter::Key_Type::Click:
		if (self_Key_Last_Time[0][(Key_Base)key][0] - self_Key_Last_Time[1][(Key_Base)key][1] > 1000)return false; //超时
		if (self_Key_Last_Time[1][(Key_Base)key][1] - self_Key_Last_Time[1][(Key_Base)key][0] > 400) return false;//非点按
		break;

	case Fighter::Key_Type::On:
		if (self_Key_Last_Time[0][(Key_Base)key][0] - self_Key_Last_Time[1][(Key_Base)key][1] > 1000)return false; //超时
		break;

	case Fighter::Key_Type::Off:
		if (self_Key_Last_Time[0][(Key_Base)key][0] - self_Key_Last_Time[1][(Key_Base)key][1] < 1000)return false; //近时
		else return true;

	default:
	case Fighter::Key_Type::None:
		return true;

	case Fighter::Key_Type::Pass:
		break;
	}

	return true;
}

void Fighter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(self_Sprite, states);
#ifdef _DEBUG
	static sf::RectangleShape box;
	box.setPosition(self_Sprite.getGlobalBounds().left, self_Sprite.getGlobalBounds().top);
	box.setSize(sf::Vector2f(self_Sprite.getGlobalBounds().width, self_Sprite.getGlobalBounds().height));
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(self_Sprite.getColor());
	box.setOutlineThickness(20);
	target.draw(box);
#endif
}
