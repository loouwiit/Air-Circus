#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.hpp"
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
	void add_Boom(sf::Vector2f Position, sf::Texture* texture, int now_Time, unsigned continue_Time, sf::Vector2f scale = sf::Vector2f(1, 1), sf::Color color = sf::Color::White);
	void compute(int now_Time);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	unsigned self_Sprites_Number = 0;
	sf::Sprite* self_Sprite = nullptr;
	int* self_Boom_End_Time = nullptr;
	bool* self_Boom_Actived = nullptr;
};

class Collideable
{
public:
	enum class Child_Class : signed char
	{
		Buoy, Fighter
	};

	float get_Mass();
	void set_Mass(float Mass);

	void collide(Collideable& B, float delta_Time, int now_Time);
	float distance(Collideable& B);
	float distance_Square(Collideable& B);

	bool is_Collide(Collideable& B);

	static void set_Boom(Boom* boom);

	virtual sf::Vector2f get_Position() = 0;
	virtual sf::Vector2f get_Velocity() = 0;
	virtual sf::FloatRect get_Collision_Box() = 0;
	virtual void change_Velocity(sf::Vector2f delta_Velocity, float time = 0) = 0;
	virtual Child_Class get_My_Child_Class() = 0;

protected:
	virtual void be_Collided(Collideable& A, int now_Time) = 0;
	float self_Mass = 0;
	static Boom* self_Boom_Ptr;

private:
	unsigned error_Times = 0;
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
	void be_Collided(Collideable& A, int now_Time);

private:
	static sf::Color Default_Color;
	static sf::Color Active_Color;
	static Camera* self_Camera;

	sf::CircleShape self_Circle;
	sf::CircleShape* self_Track;
	bool is_Active = false;
	Buoy* self_Next_Buoy = nullptr; //draw 是 const 函数，不能对自己进行更改。所以使用指针
	int self_Touch_Score = 10;
};

class Point : public sf::Drawable
{
public:
	Point();

	void set_Position(float x, float y);
	void set_Color(unsigned int Color);
	void set_Color(sf::Color Color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape self_Renctangle;
};
