#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Collideable.hpp"

namespace SinCos
{
	extern float(*sin)(float);
	extern float(*cos)(float);
}

class Fighter : public Collideable, public sf::Drawable
{
public:
	enum class Key
	{
		W = 0, S, A, D, Q, E,
		Forward = W, Back, Left, Right, Turn_Left, Turn_Right
	};

	Fighter();

	void set_Color(unsigned int Color);
	void set_Color(sf::Color Color);

	float get_Rotation();
	void set_Rotation(float rotation);
	void rotate(float rotation);
	float get_Sin_Rotate();
	float get_Cos_Rotate();

	void set_Key(unsigned i, bool flag);
	bool get_Key(unsigned i);
	void set_Key(Key i, bool flag);
	bool get_Key(Key i);
	
	sf::Vector2f get_Position();
	sf::Vector2f get_Velocity();

	void set_Position(float x, float y);
	void set_Velocity(sf::Vector2f velocity);
	void set_Velocity(float x, float y);
	void change_Velocity(sf::Vector2f delta_Velocity, float time = 0);
	void froce(sf::Vector2f acceleration, float time = 0);
	void froce(float x, float y, float time = 0);
	void move(float delta_Time);

	static sf::Texture& get_Default_Texture();

	sf::FloatRect get_Collision_Box();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	static float(*&sin)(float);
	static float(*&cos)(float);

	constexpr static char Key_Number = 6;

	static sf::Texture Default_Texture;
	sf::Sprite self_Sprite{};

	bool self_Key[Key_Number] = { false };
	bool self_Key_old[Key_Number] = { false };

	float self_Rotation = 0;
	float self_Rotation_SinCos[2] = { 0.0f,1.0f };

	//sf::Vector2f self_Acceleration{};
	sf::Vector2f self_Velocity{};
	sf::Vector2f self_Velocity_old{};
	sf::Vector2f self_Position{};
};
