#include "Fighter.hpp"
#include "vector.inl"

constexpr float PIf = (float)(3.14159265358979323846);
constexpr float PIf2 = (float)(3.14159265358979323846 / 2);
constexpr float TAUf = (float)(3.14159265358979323846 * 2);
constexpr char Meter = 100;

sf::Texture Fighter::Default_Texture{};
sf::Texture Fighter::Default_Touched_Texture{};
float(*&Fighter::sin)(float) = SinCos::sin;
float(*&Fighter::cos)(float) = SinCos::cos;
float(*&Fighter::atan2)(float, float) = SinCos::atan2;

Fighter::Fighter()
{
	self_Sprite.setTexture(get_Default_Texture());
	self_Sprite.setOrigin(get_Default_Texture().getSize().x / 2.0f, get_Default_Texture().getSize().y / 2.0f);
	self_Sprite.setScale(3.0f, 3.0f);
	rotate(-PIf / 2);
	self_Mass = 60;

	self_Path[0].init(210);
	self_Path[1].init(1);
}

void Fighter::set_Color(sf::Color Color)
{
	self_Sprite.setColor(Color);
	Color.a = 200;
	self_Path[0].set_Color(Color);
	self_Path[1].set_Color(Color);
}

int Fighter::get_Score()
{
	return self_Score;
}

void Fighter::set_Score(int score)
{
	self_Score = score;
}

sf::Color Fighter::get_Color()
{
	return self_Sprite.getColor();
}

float Fighter::get_Rotation()
{
	return self_Rotation;
}

void Fighter::set_Touch_Score(int score)
{
	self_Touch_Score = score;
}

void Fighter::set_Rotation(float rotation)
{
	self_Rotation = rotation;

	if (self_Rotation > PIf) self_Rotation -= (int)((self_Rotation + PIf) / TAUf) * TAUf;
	if (self_Rotation < -PIf) self_Rotation += (int)((self_Rotation - PIf) / -TAUf) * TAUf;

	self_Sprite.setRotation(self_Rotation / PIf * 180.f);
	self_Rotation_SinCos[0] = sin(self_Rotation);
	self_Rotation_SinCos[1] = cos(self_Rotation);
}

int Fighter::get_Touch_Score()
{
	return self_Touch_Score;
}

void Fighter::rotate(float rotation)
{
	self_Rotation += rotation;

	if (self_Rotation > PIf) self_Rotation -= (int)((self_Rotation + PIf) / TAUf) * TAUf;
	if (self_Rotation < -PIf) self_Rotation += (int)((self_Rotation - PIf) / -TAUf) * TAUf;

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
	self_Position += acceleration / self_Mass * ts;
	self_Velocity += acceleration / self_Mass * time;
}

void Fighter::froce(float x, float y, float time)
{
	float ts = time * time * 0.5f;

	//self_Acceleration.x += x;
	//self_Acceleration.y += y;
	self_Position.x += x / self_Mass * ts;
	self_Position.y += y / self_Mass * ts;

	self_Velocity.x += x / self_Mass * time;
	self_Velocity.y += y / self_Mass * time;
}

void Fighter::move(float delta_Time, int now_Time)
{
	compute(delta_Time, now_Time);
	if (abss(self_Velocity) > 5000000)
	{
		change_Velocity(self_Velocity * -0.01f);
		//printf("Fighter::Move: |Speed| = %f\n", abss(self_Velocity));
	}

	if (self_Position.x > +120 * Meter) froce(sf::Vector2f(-3000.f * (self_Position.x - 120 * Meter), 0), delta_Time);
	if (self_Position.x < -120 * Meter) froce(sf::Vector2f(-3000.f * (self_Position.x + 120 * Meter), 0), delta_Time);
	if (self_Position.y > +70 * Meter) froce(sf::Vector2f(0, -3000.f * (self_Position.y - 70 * Meter)), delta_Time);
	if (self_Position.y < -70 * Meter) froce(sf::Vector2f(0, -3000.f * (self_Position.y + 70 * Meter)), delta_Time);

	self_Position += self_Velocity_old * delta_Time;
	//self_Velocity += self_Acceleration * delta_Time;
	self_Velocity_old = self_Velocity;

	self_Sprite.setPosition(self_Position);
	if (self_Path_Is_Double)
	{
		//双航迹云
		self_Path[0].add_Path(self_Position - 90.0f * sf::Vector2f(self_Rotation_SinCos[1], self_Rotation_SinCos[0]) + sf::Vector2f(-self_Rotation_SinCos[0], self_Rotation_SinCos[1]) * 60.0f, now_Time, self_Path_Continue_Time, self_Rotation_SinCos[0], self_Rotation_SinCos[1], 15.0f);
		self_Path[1].add_Path(self_Position - 90.0f * sf::Vector2f(self_Rotation_SinCos[1], self_Rotation_SinCos[0]) - sf::Vector2f(-self_Rotation_SinCos[0], self_Rotation_SinCos[1]) * 60.0f, now_Time, self_Path_Continue_Time, self_Rotation_SinCos[0], self_Rotation_SinCos[1], 15.0f);
		self_Path[0].compute(now_Time);
		self_Path[1].compute(now_Time);
	}
	else
	{
		//单航迹云
		self_Path->add_Path(self_Position - 90.0f * sf::Vector2f(self_Rotation_SinCos[1], self_Rotation_SinCos[0]), now_Time, self_Path_Continue_Time, self_Rotation_SinCos[0], self_Rotation_SinCos[1], 50.f);
		self_Path->compute(now_Time);
	}
}

void Fighter::change_Velocity(sf::Vector2f delta_Velocity, float time)
{
	self_Velocity_old += delta_Velocity;
	self_Velocity += delta_Velocity;

	self_Position.x += delta_Velocity.x * time;
	self_Position.y += delta_Velocity.y * time;

	self_Sprite.setPosition(self_Position);
}

const sf::Texture& Fighter::get_Default_Texture()
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

const sf::Texture& Fighter::get_Default_Touched_Texture()
{
	if (Default_Touched_Texture.getSize().x != 0)
		return Default_Touched_Texture;

	std::cout << "Fighter::get_Default_Touched_Texutre: texture Rendering\n";

	struct Point
	{
		float x;
		float y;
	};

	constexpr char Point_Number = 4;
	constexpr Point Points[Point_Number] = { {0.f,63.f},{0.f,0.f},{10.f,0.f},{10.f,63.f} };

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

	Default_Touched_Texture = texture.getTexture();
	Default_Touched_Texture.setSmooth(true);

	return Default_Touched_Texture;
}

void Fighter::be_Collided(Collideable& A, int now_Time, bool is_Self_Determiner)
{
	//if (A.get_My_Child_Class() != Child_Class::Buoy) return;
	//printf("Fighter::be_Collided: delta: %f, rotation: %f sub: %f\n",
	//	atan2(self_Position.y - A.get_Position().y , self_Position.x - A.get_Position().x),
	//	self_Rotation, abs(atan2((self_Position.y - A.get_Position().y), (self_Position.x - A.get_Position().x)) -
	//		self_Rotation));
	switch (A.get_My_Child_Class())
	{
	case Collideable::Child_Class::Buoy:
	{
		Buoy& buoy = *((Buoy*)&A);
		if (abs(abs(atan2((self_Position.y - A.get_Position().y), (self_Position.x - A.get_Position().x)) -
			self_Rotation) - PIf) < 0.5)
		{
			//头碰
			if (is_Key(Key::Forward))
			{
				//W按下 右转PIf2
				self_Velocity = ::rotate(self_Velocity, -1, 0);
				self_Velocity_old = ::rotate(self_Velocity_old, -1, 0);
				rotate(PIf2);
			}

			if (buoy.be_Touched()) self_Score += buoy.get_Touch_Score();
		}
		else
		{
			//非头碰 但是慢
			if (abss(self_Velocity) < 600000)
			{
				if (buoy.be_Touched()) self_Score += buoy.get_Touch_Score();
			}
		}

		if (is_Self_Determiner) self_Particle_Ptr->add_Particle((self_Position + buoy.get_Position()) * 0.5f, now_Time, 1000);

		break;
	}
	case Collideable::Child_Class::Fighter:
	{
		Fighter& fighter = *((Fighter*)&A); // 强制类型转换
		if (abs(fighter.get_Rotation() - self_Rotation) < 0.7)
		{
			//同向
			float delta_Rotation = self_Rotation - atan2(fighter.get_Position().y - self_Position.y, fighter.get_Position().x - self_Position.x);

			if (delta_Rotation > PIf)
				delta_Rotation -= (int)((delta_Rotation + PIf) / TAUf) * TAUf;
			if (delta_Rotation < -PIf)
				delta_Rotation += (int)((delta_Rotation - PIf) / -TAUf) * TAUf;

			//printf("[debug]delta_Rotation = %f\n", delta_Rotation);

			if (abs(delta_Rotation) < 0.7)
			{
				//头尾碰
				self_Score += fighter.get_Touch_Score();
				//printf("Fighter::be_Collided: color %s score = %d\n", (get_Color().r == 255 ? "rad" : "blue"), self_Score);

				self_Boom_Ptr->add_Boom(fighter.get_Position(), &get_Default_Touched_Texture(), now_Time, 1000, sf::Vector2f(10, 10), fighter.get_Rotation() / PIf * 180, fighter.get_Color());
				self_Boom_Ptr->add_Boom(fighter.get_Position(), &get_Default_Texture(), now_Time, 1000, sf::Vector2f(3, 3), fighter.get_Rotation() / PIf * 180, fighter.get_Color());
				self_Boom_Ptr->add_Boom(self_Position, &get_Default_Texture(), now_Time, 1000, sf::Vector2f(3, 3), self_Rotation / PIf * 180, self_Sprite.getColor());
			}
		}
		if (is_Self_Determiner) self_Particle_Ptr->add_Particle((self_Position + fighter.get_Position()) * 0.5f,  now_Time, 1000);
		break;
	}
	default:
		break;
	}

}

Fighter::Child_Class Fighter::get_My_Child_Class()
{
	return Child_Class::Fighter;
}

void Fighter::compute(float delta_Time, int now_Time)
{
	using Type = Key_Type;

	if (is_Key(Key::Forward))
	{
		froce(+120000.f * self_Rotation_SinCos[1], +120000.f * self_Rotation_SinCos[0], delta_Time);
	}

	if (is_Key(Key::Back))
	{
		if (is_Key(Key::Back, now_Time, Type::Click, Type::Click, Type::Click) && now_Time > self_Next_Back_Time)
		{
			//三击
			if (abs(self_Rotation - atan2(get_Velocity().y, get_Velocity().x)) < 0.7)
			{
				set_Rotation(self_Rotation + PIf);
				set_Velocity(-get_Velocity());
			}
			else
			{
				set_Velocity(abs(self_Velocity) * sf::Vector2f(self_Rotation_SinCos[1], self_Rotation_SinCos[0]));
			}
			sf::Color color = get_Color();
			color.a -= 60;
			self_Particle_Ptr->add_Particle(self_Position - 90.0f * sf::Vector2f(self_Rotation_SinCos[1], self_Rotation_SinCos[0]), now_Time, 1000, sf::Vector2f(1000, 100), color, self_Rotation + PIf2);
			//printf("Fighter::compute: triple back\n");
			self_Next_Back_Time = now_Time + self_Back_Time_Limit;
		}
		else if (is_Key(Key::Back, now_Time, Type::Pass, Type::Click))
		{
			//上次是点 减速
			froce(self_Velocity * -300.0f, delta_Time);
			//printf("Fighter::compute: double back\n");
		}
		else
		{
			//其他 后退
			froce(-30000 * self_Rotation_SinCos[1], -30000 * self_Rotation_SinCos[0], delta_Time);
		}
	}

	if (self_Key[(Key_Base)Key::Turn_Left])
	{
		rotate(-2.0f * delta_Time);
	}

	if (self_Key[(Key_Base)Key::Turn_Right])
	{
		rotate(+2.0f * delta_Time);
	}

	if (self_Key[(Key_Base)Key::Left])
	{
		froce(+30000 * self_Rotation_SinCos[0], -30000 * self_Rotation_SinCos[1], delta_Time);
	}

	if (self_Key[(Key_Base)Key::Right])
	{
		froce(-30000 * self_Rotation_SinCos[0], +30000 * self_Rotation_SinCos[1], delta_Time);
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

	target.draw(self_Path[0], states);
	if (self_Path_Is_Double) target.draw(self_Path[1], states);
}

Path_Line::~Path_Line()
{
	if (self_Number != 0)
	{
		self_Number = 0;
		delete[] self_Time;
		delete[] self_Vertex;
		self_Time = nullptr;
		self_Vertex = nullptr;
		self_Last_Vertex = nullptr;
	}
}

void Path_Line::init(unsigned short line_Number)
{
	if (self_Number != 0)
	{
		delete[] self_Time;
		delete[] self_Vertex;
		self_Time = nullptr;
		self_Vertex = nullptr;
		self_Last_Vertex = nullptr;
	}

	self_Now_Index = 0;
	self_Number = line_Number;
	self_Time = new int[self_Number];
	self_Vertex = new Quad[self_Number];
	self_Last_Vertex = self_Vertex;

	std::fill(self_Time, self_Time + self_Number, 0);
	std::fill((sf::Vertex*)self_Vertex, (sf::Vertex*)(self_Vertex + self_Number), sf::Vertex(sf::Vector2f(0, 0), sf::Color::Transparent));
}

void Path_Line::set_Color(sf::Color color)
{
	self_Color = color;
}

void Path_Line::add_Path(sf::Vector2f position, int now_Time, unsigned continue_Time, float sin, float cos, float thickness)
{
	if (self_Vertex[self_Now_Index].v3.color.a != 0)
	{
		unsigned char alpha = self_Vertex[self_Now_Index].v3.color.a;
		printf("Path_Line::add_Path: warning, buffer size is too small, %d's alpha is %d\n", self_Now_Index, alpha);
		if (alpha < 10) alpha = 10;
		int* time = new int[self_Number + alpha];
		Quad* vertex = new Quad[self_Number + alpha];
		self_Last_Vertex = vertex + self_Number - 1; //保证区域连续
		self_Now_Index = self_Number; //从新创建的区域开始
		for (unsigned short i = 0; i < self_Number; i++)
		{
			time[i] = self_Time[i];
			vertex[i] = self_Vertex[i];
		}
		for (unsigned short i = self_Number; i < self_Number + alpha; i++)
		{
			time[i] = 0;
			vertex[i].v1.color.a = 0;
			vertex[i].v2.color.a = 0;
			vertex[i].v3.color.a = 0;
			vertex[i].v4.color.a = 0;
		}
		delete[] self_Time;
		delete[] self_Vertex;
		self_Time = time;
		self_Vertex = vertex;
		self_Number += alpha;
		printf("Path_Line::add_Path: new vertex size is %d\n", self_Number);
	}
	
	self_Time[self_Now_Index] = now_Time + continue_Time;

	self_Vertex[self_Now_Index].v1 = self_Last_Vertex->v4;
	self_Vertex[self_Now_Index].v2 = self_Last_Vertex->v3; //复制上一次的点
	self_Vertex[self_Now_Index].v3.position.x = position.x - sin * thickness;
	self_Vertex[self_Now_Index].v3.position.y = position.y + cos * thickness;
	self_Vertex[self_Now_Index].v4.position.x = position.x + sin * thickness;
	self_Vertex[self_Now_Index].v4.position.y = position.y - cos * thickness;

	self_Vertex[self_Now_Index].v3.color = self_Color;
	self_Vertex[self_Now_Index].v4.color = self_Color;

	self_Last_Vertex = self_Vertex + self_Now_Index; //标记此次位置
	self_Now_Index++;
	if (self_Now_Index >= self_Number) self_Now_Index = 0;
}

void Path_Line::compute(int now_Time)
{
	for (unsigned short i = 0; i < self_Number; i ++)
	{
		if (self_Time[i] > now_Time) continue;

		if (self_Vertex[i].v3.color.a < Sub_Speed)
		{
			self_Vertex[i].v1.color.a = 0;
			self_Vertex[i].v2.color.a = 0;
			self_Vertex[i].v3.color.a = 0;
			self_Vertex[i].v4.color.a = 0;
		}
		else
		{
			self_Vertex[i].v1.color.a -= Sub_Speed;
			self_Vertex[i].v2.color.a -= Sub_Speed;
			self_Vertex[i].v3.color.a -= Sub_Speed;
			self_Vertex[i].v4.color.a -= Sub_Speed;
		}
	}
}

void Path_Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw((sf::Vertex*)self_Vertex, (short)self_Number * 4, sf::Quads, states);
}
