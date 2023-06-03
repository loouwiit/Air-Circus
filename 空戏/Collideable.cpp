#include "Collideable.hpp"
#include "Fighter.hpp"

constexpr float PIf = (float)(3.14159265358979323846);

Boom* Collideable::self_Boom_Ptr = nullptr;
Particle* Collideable::self_Particle_Ptr = nullptr;
unsigned Collideable::error_Times = 0;
sf::Color Buoy::Default_Color = sf::Color::Green;
sf::Color Buoy::Active_Color = sf::Color::Yellow;
Camera* Buoy::self_Camera = nullptr;

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

void Collideable::collide(Collideable& B, float delta_Time, int now_Time)
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

	//速度
	{
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
			a_Base_Velocity = rotate(a_Velocity, sin, cos);
			b_Base_Velocity = rotate(b_Velocity, sin, cos);
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
			a_New = rotate(a_New_Base, -sin, cos);
			b_New = rotate(b_New_Base, -sin, cos);
		}

		change_Velocity(a_New - a_Velocity,delta_Time);
		B.change_Velocity(b_New - b_Velocity, delta_Time);
	}

	//角动量
	{
		sf::Vector2f delta_Velocity = a_Base_Velocity - b_Base_Velocity;
		this->change_Angular(delta_Velocity.y / 1000.0f);
		B.change_Angular(delta_Velocity.y / 1000.0f);
		//printf("[debug]Augnlar = %f\n", delta_Velocity.y);
	}

	this->be_Collided(B, now_Time, true);
	B.be_Collided(*this, now_Time, false);

	if (is_Collide(B))
	{
		error_Times++;
		printf("collideable::collide: error %u, collide after collided\n", error_Times);
		sf::sleep(sf::milliseconds(1000));
	}

	//printf("\tP^2 %f to %f\n", a_Mass * a_Mass * test(a_Velocity) + b_Mass * b_Mass * test(b_Velocity), a_Mass * a_Mass * test(a_New) + b_Mass * b_Mass * test(b_New));
}

float Collideable::distance(Collideable& B)
{
	return abs(get_Position() - B.get_Position());
}

float Collideable::distance_Square(Collideable& B)
{
	return abss(get_Position() - B.get_Position());
}

bool Collideable::is_Collide(Collideable& B)
{
	return this->get_Collision_Box().intersects(B.get_Collision_Box());
}

void Collideable::set_Boom(Boom* boom)
{
	self_Boom_Ptr = boom;
}

void Collideable::set_Particle(Particle* particle)
{
	self_Particle_Ptr = particle;
}

Buoy::Buoy()
{
	self_Circle.setRadius(100.f);
	self_Circle.setFillColor(Default_Color);
	self_Circle.setPointCount(20);
	self_Circle.setOrigin(self_Circle.getRadius(), self_Circle.getRadius());
	self_Mass = 0;

	self_Track = new sf::CircleShape;
	self_Track->setRadius(35);
	self_Track->setFillColor(Default_Color);
	self_Track->setPointCount(20);
	self_Track->setOrigin(self_Track->getRadius(), self_Track->getRadius());
}

Buoy::~Buoy()
{
	delete self_Track;
	self_Track = nullptr;
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

void Buoy::change_Angular(float delta_Angular, float time)
{}

void Buoy::set_Next_Buoy(Buoy* next)
{
	self_Next_Buoy = next;
}

void Buoy::set_Active(bool flag)
{
	if (flag)
	{
		self_Circle.setFillColor(Active_Color);
		self_Track->setFillColor(Active_Color);
	}
	else
	{
		self_Circle.setFillColor(Default_Color);
		self_Track->setFillColor(Default_Color);
	}

	is_Active = flag;
}

void Buoy::set_Touch_Score(int score)
{
	self_Touch_Score = score;
}

int Buoy::get_Touch_Score()
{
	return self_Touch_Score;
}

bool Buoy::be_Touched()
{
	if (is_Active == false) return false;
	set_Active(false);
	self_Next_Buoy->set_Active(true);

	return true;
}

void Buoy::set_Color(sf::Color default_Color, sf::Color active_Color)
{
	Default_Color = default_Color;
	Active_Color = active_Color;
}

void Buoy::set_Camera(Camera* camera)
{
	self_Camera = camera;
}

void Buoy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (self_Camera->intersects(self_Circle.getGlobalBounds()))
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
	if (!self_Camera->contains(self_Circle.getPosition()))
	{
		sf::Vector2f center = self_Camera->get_Center();
		sf::Vector2f size = self_Camera->get_Size() * 0.5f - sf::Vector2f(100,100); //边界
		sf::Vector2f vector = self_Circle.getPosition() - center;
		float tan = vector.y / vector.x;
		vector *= sqrt(std::min(abss(sf::Vector2f(size.x, size.x * tan)), abss((sf::Vector2f(size.y / tan, size.y)))) / abss(vector));

		self_Track->setPosition(center + vector);

		target.draw(*self_Track);
	}
}

Buoy::Child_Class Buoy::get_My_Child_Class()
{
	return Child_Class::Buoy;
}

void Buoy::be_Collided(Collideable& A, int now_Time, bool is_Self_Determiner)
{}

Point::~Point()
{
	if (self_Number != 0)
	{
		delete[] self_Vertex;
		self_Number = 0;
	}
}

void Point::init(sf::Vector2f start_Position, sf::Vector2f delta_Position, sf::Vector2u delta_Times, sf::Vector2f offset, sf::Color color)
{
	if (self_Number != 0)
	{
		delete[] self_Vertex;
		self_Number = 0;
	}
	
	self_Number = delta_Times.x * delta_Times.y;
	self_Vertex = new Quad[self_Number];

	unsigned short index = 0;

	for (unsigned short i = 0; i < delta_Times.x; i++) for (unsigned short j = 0; j < delta_Times.y; j++)
	{
		index = i * delta_Times.y + j;

		self_Vertex[index].v1.color = color;
		self_Vertex[index].v2.color = color;
		self_Vertex[index].v3.color = color;
		self_Vertex[index].v4.color = color;

		self_Vertex[index].v1.position = start_Position + delta_Position * sf::Vector2f(i, j) + offset * sf::Vector2f(+1, +1);
		self_Vertex[index].v2.position = start_Position + delta_Position * sf::Vector2f(i, j) + offset * sf::Vector2f(+1, -1);
		self_Vertex[index].v3.position = start_Position + delta_Position * sf::Vector2f(i, j) + offset * sf::Vector2f(-1, -1);
		self_Vertex[index].v4.position = start_Position + delta_Position * sf::Vector2f(i, j) + offset * sf::Vector2f(-1, +1);
	}
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw((sf::Vertex*)self_Vertex, self_Number * 4, sf::Quads, states);
}

void Boom::init(unsigned Default_Sprite_Number)
{
	if (self_Sprites_Number != 0)
	{
		delete[] self_Sprite;
		delete[] self_Boom_End_Time;
		delete[] self_Boom_Actived;
		self_Sprites_Number = 0;
	}

	self_Sprites_Number = Default_Sprite_Number;

	self_Sprite = new sf::Sprite[self_Sprites_Number];
	self_Boom_End_Time = new int[self_Sprites_Number];
	self_Boom_Actived = new bool[self_Sprites_Number];

	for (unsigned j = 0; j < self_Sprites_Number; j++)
	{
		self_Boom_End_Time[j] = 0;
		self_Boom_Actived[j] = false;
	}

	std::cout << "Boom::init: boom buffer size = " << self_Sprites_Number << '\n';
}

Boom::~Boom()
{
	if (self_Sprites_Number != 0)
	{
		delete[] self_Sprite;
		delete[] self_Boom_End_Time;
		delete[] self_Boom_Actived;
		self_Sprites_Number = 0;
	}
}

void Boom::add_Boom(sf::Vector2f Position, const sf::Texture* texture, int now_Time, unsigned continue_Time, sf::Vector2f scale, float rotation, sf::Color color)
{
	unsigned i = 0;
	while (i < self_Sprites_Number)
	{
		if (self_Boom_End_Time[i] < now_Time) break;
		i++;
	}

	if (i == self_Sprites_Number)
	{
		//不足，需要补充内存

		sf::Sprite* sprite = new sf::Sprite[self_Sprites_Number + 1];
		int* end_Time = new int[self_Sprites_Number + 1];
		bool* actived = new bool[self_Sprites_Number + 1];

		for (unsigned j = 0; j < self_Sprites_Number; j++)
		{
			sprite[j] = self_Sprite[j];
			end_Time[j] = self_Boom_End_Time[j];
			actived[j] = self_Boom_Actived[j];
		}

		delete[] self_Sprite;
		delete[] self_Boom_End_Time;
		delete[] self_Boom_Actived;
		self_Sprite = sprite;
		self_Boom_End_Time = end_Time;
		self_Boom_Actived = actived;
		self_Sprites_Number++;

		std::cout << "Boom::add_Boom: boom buffer size = " << self_Sprites_Number << '\n';
	}

	self_Sprite[i].setTexture(*texture, true);
	self_Sprite[i].setOrigin((float)(texture->getSize().x / 2), (float)(texture->getSize().y / 2));
	self_Sprite[i].setColor(sf::Color::White);
	self_Sprite[i].setPosition(Position);
	self_Sprite[i].setScale(scale);
	self_Sprite[i].setColor(color);
	self_Sprite[i].setRotation(rotation);

	self_Boom_End_Time[i] = now_Time + continue_Time;
	self_Boom_Actived[i] = true;
}

void Boom::compute(int now_Time)
{
	for (unsigned i = 0; i < self_Sprites_Number; i++)
	{
		if (!self_Boom_Actived[i]) continue; //跳过

		if (self_Boom_End_Time[i] < now_Time)
		{

			sf::Color color = self_Sprite[i].getColor();
			color.a -= 10;
			self_Sprite[i].setColor(color);

			if (color.a <= 10) self_Boom_Actived[i] = false;
		}
	}
}

void Boom::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (unsigned i = 0; i < self_Sprites_Number; i++)
	{
		if (self_Boom_Actived[i]) target.draw(self_Sprite[i]);
#ifdef _DEBUG
		if (self_Boom_Actived[i])
		{
			static sf::RectangleShape box;
			box.setPosition(self_Sprite[i].getGlobalBounds().left, self_Sprite[i].getGlobalBounds().top);
			box.setSize(sf::Vector2f(self_Sprite[i].getGlobalBounds().width, self_Sprite[i].getGlobalBounds().height));
			box.setFillColor(sf::Color::Transparent);
			box.setOutlineColor(sf::Color(sf::Color::Cyan.r, sf::Color::Cyan.g, sf::Color::Cyan.b, 50));
			box.setOutlineThickness(20);
			target.draw(box);
		}
#endif
	}
}

void Particle::init(unsigned Particle_Number)
{
	if (self_Number != 0)
	{
		self_Number = 0;

		for (unsigned char i = 0; i < self_Number; i++)
			if (self_Triangle[i] != nullptr) delete[] self_Triangle[i];
		delete[] self_Triangle;
		delete[] self_Transform;
		delete[] self_Triangle_Number;

		delete[] self_End_Time;
		delete[] self_Actived;
	}

	self_Number = Particle_Number;

	self_Triangle = new Triangle*[self_Number];
	self_Transform = new sf::Transformable[self_Number];
	self_Triangle_Number = new unsigned short[self_Number];
	self_End_Time = new int[self_Number];
	self_Actived = new bool[self_Number];

	std::fill(self_Triangle, self_Triangle + self_Number, nullptr);
	std::fill(self_Triangle_Number, self_Triangle_Number + self_Number, 0);
	std::fill(self_End_Time, self_End_Time + self_Number, 0);
	std::fill(self_Actived, self_Actived + self_Number, false);

	std::cout << "Particle::init: particle buffer size = " << (int)self_Number << '\n';
}

Particle::~Particle()
{
	if (self_Number != 0)
	{
		self_Number = 0;

		for (unsigned char i = 0; i < self_Number; i++)
			if (self_Triangle[i] != nullptr) delete[] self_Triangle[i];
		delete[] self_Triangle;
		delete[] self_Transform;
		delete[] self_Triangle_Number;

		delete[] self_End_Time;
		delete[] self_Actived;
	}
}

void Particle::add_Particle(sf::Vector2f Position, int now_Time, unsigned continue_Time, sf::Vector2f size, sf::Color color, float rotation, unsigned short Particle_Number)
{
	unsigned index = 0;
	while (index < self_Number)
	{
		if (!self_Actived[index]) break;
		index++;
	}

	if (index == self_Number)
	{
		//不足，需要补充内存

		Triangle** triangle = new Triangle * [self_Number + 1];
		sf::Transformable* transform = new sf::Transformable[self_Number + 1];
		unsigned short* vertex_Number = new unsigned short[self_Number + 1];
		int* end_Time = new int[self_Number + 1];
		bool* actived = new bool[self_Number + 1];

		for (unsigned i = 0; i < self_Number; i++)
		{
			triangle[i] = self_Triangle[i];
			transform[i] = self_Transform[i];
			vertex_Number[i] = self_Triangle_Number[i];
			end_Time[i] = self_End_Time[i];
			actived[i] = self_Actived[i];
		}

		vertex_Number[index] = 0;

		delete[] self_Triangle;
		delete[] self_Transform;
		delete[] self_Triangle_Number;
		delete[] self_End_Time;
		delete[] self_Actived;

		self_Triangle = triangle;
		self_Transform = transform;
		self_Triangle_Number = vertex_Number;
		self_End_Time = end_Time;
		self_Actived = actived;
		self_Number++;

		std::cout << "Particle::add_Particle: particle buffer size = " << (int)self_Number << '\n';
	}

	self_Transform[index].setOrigin(size * 0.5f);
	self_Transform[index].setRotation(rotation / PIf * 180);
	self_Transform[index].setPosition(Position);
	self_End_Time[index] = now_Time + continue_Time;
	self_Actived[index] = true;

	if (self_Triangle_Number[index] != Particle_Number)
	{
		if (self_Triangle_Number[index] != 0) delete[] self_Triangle[index];
		self_Triangle_Number[index] = Particle_Number;
		self_Triangle[index] = new Triangle[Particle_Number];
	}

	Triangle*& triangle = self_Triangle[index];
	unsigned short& number = self_Triangle_Number[index];

	for (unsigned short i = 0; i < number; i++)
	{
		triangle[i].v1.color = color;
		triangle[i].v1.position.x = ((float)rand() / (float)RAND_MAX) * size.x;
		triangle[i].v1.position.y = ((float)rand() / (float)RAND_MAX) * size.y;

		triangle[i].v2.color = color;
		triangle[i].v2.position.x = ((float)rand() / (float)RAND_MAX) * size.x;
		triangle[i].v2.position.y = ((float)rand() / (float)RAND_MAX) * size.y;

		triangle[i].v3.color = color;
		triangle[i].v3.position.x = ((float)rand() / (float)RAND_MAX) * size.x;
		triangle[i].v3.position.y = ((float)rand() / (float)RAND_MAX) * size.y;
	}
}

void Particle::compute(int now_Time)
{
	for (unsigned index = 0; index < self_Number; index++)
	{
		if (!self_Actived[index]) continue; //跳过

		if (self_End_Time[index] < now_Time)
		{
			//减少alpha
			for (unsigned short i = 0; i < self_Triangle_Number[index]; i++)
			{
				self_Triangle[index][i].v1.color.a -= 10;
				self_Triangle[index][i].v2.color.a -= 10;
				self_Triangle[index][i].v3.color.a -= 10;
			}

			if (self_Triangle[index]->v1.color.a <= 10) self_Actived[index] = false;
		}
	}
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates state;
	for (unsigned i = 0; i < self_Number; i++)
	{
		if (!self_Actived[i]) continue;
		state = states;
		state.transform *= self_Transform[i].getTransform();
		target.draw((sf::Vertex*)self_Triangle[i], self_Triangle_Number[i] * 3, sf::Triangles, state);
	}
}
