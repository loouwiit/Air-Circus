#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collideable.hpp"

namespace SinCos
{
	extern float(*sin)(float);
	extern float(*cos)(float);
}

class Path_Line : public sf::Drawable
{
public:
	~Path_Line();
	void init(unsigned short line_Number);
	void set_Color(sf::Color color);
	void add_Path(sf::Vector2f position, int now_Time, unsigned continue_Time, float sin, float cos, float thickness);
	void compute(int now_Time);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	constexpr static unsigned char Sub_Speed = 1;

	struct Quad
	{
		sf::Vertex v1;
		sf::Vertex v2;
		sf::Vertex v3;
		sf::Vertex v4;
	};

	unsigned short self_Number = 0;
	unsigned short self_Now_Index = 0;
	int* self_Time = nullptr;
	Quad* self_Vertex;
	Quad* self_Last_Vertex;
	sf::Color self_Color = sf::Color::Black;
};

class Fighter : public Collideable, public sf::Drawable
{
public:
	using Key_Base = unsigned char;
	enum class Key : Key_Base
	{
		W = 0, S, A, D, Q, E,
		Forward = W, Back, Turn_Left, Turn_Right, Left, Right
	};

	Fighter();

	void set_Color(sf::Color Color);
	sf::Color get_Color();
	void set_Score(int score);
	void set_Touch_Score(int score);
	int get_Score();
	int get_Touch_Score();

	float get_Rotation();
	void set_Rotation(float rotation);
	void rotate(float rotation);
	float get_Sin_Rotate();
	float get_Cos_Rotate();

	void set_Key(Key i, bool flag, int now_Time);
	bool get_Key(Key i);
	
	sf::Vector2f get_Position();
	sf::Vector2f get_Velocity();

	void set_Position(float x, float y);
	void set_Velocity(sf::Vector2f velocity);
	void set_Velocity(float x, float y);
	void change_Velocity(sf::Vector2f delta_Velocity, float time = 0);
	void froce(sf::Vector2f acceleration, float time = 0);
	void froce(float x, float y, float time = 0);
	void move(float delta_Time, int now_Time);

	static const sf::Texture& get_Default_Texture();
	static const sf::Texture& get_Default_Touched_Texture();

	sf::FloatRect get_Collision_Box();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Child_Class get_My_Child_Class();
	void be_Collided(Collideable& A, int now_Time, bool is_Self_Determiner);

private:
	static float(*&sin)(float);
	static float(*&cos)(float);

	constexpr static char Key_Number = 8;
	constexpr static int Default_Back_Time_Limit = 1000;

	enum class Key_Type : Key_Base
	{
		None, Pass, Press, Click, On, Off
	};

	static sf::Texture Default_Texture;
	static sf::Texture Default_Touched_Texture;

	sf::Sprite self_Sprite{};
	Path_Line self_Path[2];
	bool self_Path_Is_Double = false;
	unsigned self_Path_Continue_Time = 30;

	bool self_Key[Key_Number] = { false };
	bool self_Key_Old[Key_Number] = { false }; //辨别长按中的每一次
	bool self_Key_Filp[Key_Number] = { false };//辨别是否变换

	int self_Key_Now_Start_Time[Key_Number] = { 0 }; //for the self_Key_Last_Time
	int self_Key_Last_Time[2][Key_Number][2] = {0};	//time, in milisecond
													//[2]:single_Last or doubel_Last
													//[Keynumber]:erveryKey
													//[2]:start_Time and end_Time

	int self_Score = 0;
	int self_Touch_Score = 10;

	int self_Back_Time_Limit = Default_Back_Time_Limit;
	int self_Next_Back_Time = 0;
	float self_Rotation = 0;
	float self_Rotation_SinCos[2] = { 0.0f,1.0f };

	//sf::Vector2f self_Acceleration{};
	sf::Vector2f self_Velocity{};
	sf::Vector2f self_Velocity_old{};
	sf::Vector2f self_Position{};

	void compute(float delta_Time, int now_Time);
	bool is_Key(Key key, int now_Time = 0, Key_Type T1 = Key_Type::On, Key_Type T2 = Key_Type::None, Key_Type T3 = Key_Type::None);
};
