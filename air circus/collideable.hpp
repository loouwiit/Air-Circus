#pragma once
#include <SFML/Graphics.hpp>
#include "camera.hpp"
#include "vector.inl"

namespace SinCos
{
	extern float(*sin)(float);
	extern float(*cos)(float);
}

class Boom : public sf::Drawable
{
public:
	~Boom();
	void init(unsigned Default_Sprite_Number);
	void add_Boom(sf::Vector2f Position, const sf::Texture* texture, int now_Time, unsigned continue_Time, sf::Vector2f scale = sf::Vector2f(1, 1), float rotation = 0.f, sf::Color color = sf::Color::White);
	void compute(int now_Time);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	unsigned self_Sprites_Number = 0;
	sf::Sprite* self_Sprite = nullptr;
	int* self_Boom_End_Time = nullptr;
	bool* self_Boom_Actived = nullptr;
};

class Particle : public sf::Drawable
{
public:
	~Particle();
	void init(unsigned Particle_Number);
	void add_Particle(sf::Vector2f Position, int now_Time, unsigned continue_Time, sf::Vector2f size = sf::Vector2f(2000, 2000), sf::Color color = sf::Color(sf::Color::Yellow.r, sf::Color::Yellow.g, sf::Color::Yellow.b, 50), float rotation = 0, unsigned short Particle_Number = 10);
	void compute(int now_Time);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	struct Triangle
	{
		sf::Vertex v1;
		sf::Vertex v2;
		sf::Vertex v3;
	};
	unsigned char self_Number = 0;
	sf::Transformable* self_Transform = nullptr;
	Triangle** self_Triangle = nullptr;
	unsigned short* self_Triangle_Number = nullptr;
	int* self_End_Time = nullptr;
	bool* self_Actived = nullptr;
};

class Collideable
{
public:
	enum class Child_Class : signed char
	{
		Buoy, Fighter
	};

	constexpr static int Collide_Clam_Time = 50;
	constexpr static float TurnSpeed = 2.0f;
	constexpr static float QuickTurnSpeed = 4.0f;

	float get_Mass();
	void set_Mass(float Mass);

	void collide(Collideable& B, float delta_Time, int now_Time);
	float distance(Collideable& B);
	float distance_Square(Collideable& B);

	bool is_Collide(Collideable& B);

	static void set_Boom(Boom* boom);
	static void set_Particle(Particle* particle);

	virtual sf::Vector2f get_Position() = 0;
	virtual sf::Vector2f get_Velocity() = 0;
	virtual sf::FloatRect get_Collision_Box() = 0;
	virtual void change_Velocity(sf::Vector2f delta_Velocity, float time = 0) = 0;
	virtual void change_Angular(float delta_Angular, float time = 0) = 0;
	virtual Child_Class get_My_Child_Class() = 0;

protected:
	virtual void be_Collided(Collideable& A, int now_Time, bool is_Self_Determiner) = 0;
	float self_Mass = 0;
	static Boom* self_Boom_Ptr;
	static Particle* self_Particle_Ptr;

private:
	static unsigned error_Times;
};

class Buoy : public Collideable, public sf::Drawable
{
public:
	Buoy();
	~Buoy();

	void set_Position(float x, float y);
	sf::Vector2f get_Position();

	sf::Vector2f get_Velocity();
	sf::FloatRect get_Collision_Box();
	void change_Velocity(sf::Vector2f delta_Velocity, float time = 0);
	void change_Angular(float delta_Angular, float time = 0);

	void compute(float delta_Time);

	void set_Next_Buoy(Buoy* next);
	void set_Active(bool flag);
	void set_Touch_Score(int score);
	int get_Touch_Score();
	bool be_Touched();

	static void set_Color(sf::Color default_Color, sf::Color active_Color);
	static void set_Camera(Camera* camera);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Child_Class get_My_Child_Class();
	void be_Collided(Collideable& A, int now_Time, bool is_Self_Determiner);

private:
	constexpr static float Mass = 600;

	static sf::Color Default_Color;
	static sf::Color Active_Color;
	static Camera* self_Camera;

	sf::Vector2f self_Target_Position = { 0, 0 };
	sf::Vector2f self_Velocity = { 0, 0 };

	sf::CircleShape self_Circle;
	sf::CircleShape* self_Track;
	bool is_Active = false;
	Buoy* self_Next_Buoy = nullptr; //draw 是 const 函数，不能对自己进行更改。所以使用指针
	int self_Touch_Score = 2;
};

class Point : public sf::Drawable
{
public:
	~Point();
	void init(sf::Vector2f start_Position, sf::Vector2f delta_Position, sf::Vector2u delta_Times, sf::Vector2f offset, sf::Color color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	struct Quad
	{
		sf::Vertex v1;
		sf::Vertex v2;
		sf::Vertex v3;
		sf::Vertex v4;
	};

	Quad* self_Vertex;
	unsigned short self_Number = 0;
};
