#include <iostream>
#include <SFML/Graphics.hpp>
#include "Fighter.hpp"
#include "Camera.hpp"
#include "Collideable.hpp"
#include "functions.hpp"

namespace Time
{
	sf::Time now_Time = sf::milliseconds(0);
	sf::Time delta_Time = sf::milliseconds(16);
	sf::Time sleep_Time = sf::milliseconds(1000 / FPS);
}

sf::RenderWindow window;
Camera camera;
Fighter players[Player_Number];
Buoy buoy[Buoy_Number];
Point position_Point[Position_Point_X_Number * Position_Point_Y_Number];
sf::RectangleShape map_Side;
Boom boom;

int main(int argc, char* argv[]);
void init();
void event();
void compute();
void draw();
void sleep();
void ened();

int main(int argc, char* argv[])
{
	init();
	while (window.isOpen())
	{
		event();
		compute();
		draw();
		sleep();
	}
	ened();
	return 0;
}

void init()
{
	cout << "init::init" << endl;

	srand((int)::time(nullptr));

	window.create(sf::VideoMode(1500, 900), Window_Name);
	//camera.set_Size(250 * Meter, 150 * Meter);
	//camera.set_Center(0.f, 0.f);
	camera.set_Position(sf::Vector2f(-240 / 2 * Meter, -140 / 2 * Meter), sf::Vector2f(240 / 2 * Meter, 140 / 2 * Meter));
	camera.set_Target(&window);

	Collideable::set_Boom(&boom);

	players[0].set_Position(-10 * Meter, 0 * Meter);
	players[0].set_Color(sf::Color::Red);
	players[1].set_Position(10 * Meter, 0 * Meter);
	players[1].set_Color(sf::Color::Blue);

	Buoy::set_Camera(&camera);
	Buoy::set_Color(sf::Color::Green, sf::Color::Yellow);

	buoy[0].set_Position(-100 * Meter, -50 * Meter);
	buoy[1].set_Position(+100 * Meter, -50 * Meter);
	buoy[2].set_Position(+100 * Meter, +50 * Meter);
	buoy[3].set_Position(-100 * Meter, +50 * Meter);

	buoy[3].set_Next_Buoy(&buoy[0]);
	for (char i = 0; i < 3; i++) buoy[i].set_Next_Buoy(&buoy[i + 1]);

	buoy[0].set_Active(true);
	for (char i = 1; i < 4; i++) buoy[i].set_Active(false);

	for (int i = 0; i < Position_Point_Y_Number; i++) for (int j = 0; j < Position_Point_X_Number; j++)
	{
		position_Point[i * Position_Point_X_Number + j].set_Color(sf::Color::Black);
		position_Point[i * Position_Point_X_Number + j].set_Position(((float)(j - Position_Point_X_Number / 2) * 10 * Meter), (float)((i - Position_Point_Y_Number / 2) * 10 * Meter));
	}

	map_Side.setFillColor(sf::Color::Transparent);
	map_Side.setOutlineColor(sf::Color::Black);
	map_Side.setOutlineThickness(20);
	map_Side.setOrigin(120 * Meter, 70 * Meter);
	map_Side.setPosition(0, 0);
	map_Side.setSize(sf::Vector2f(240 * Meter, 140 * Meter));
}

void event()
{
	static sf::Event event;

	while (window.pollEvent(event))
	{
		Event::event(event);
	}
}

void compute()
{
#ifdef _DEBUG
	debug();
#endif

	using namespace Compute;

	compute_Player();
	compute_Camera();
	boom.compute(Time::now_Time.asMilliseconds());
}

void draw()
{
	window.clear(sf::Color::White);

	if (!camera.is_Full()) for (unsigned short i = 0; i < Position_Point_X_Number * Position_Point_Y_Number; i++) window.draw(position_Point[i]);
	else window.draw(map_Side);
	for (char i = 0; i < 4; i++) window.draw(buoy[i]);
	for (char i = 0; i < Player_Number; i++) window.draw(players[i]);
	window.draw(boom);

	window.display();
}

void sleep()
{
	using namespace Time;
	static sf::Clock clock;
	//cout << '[' << clock.getElapsedTime().asMilliseconds() << "ms]sleep:fream end time use:" << (clock.getElapsedTime() - now_Time).asMilliseconds() << "ms" << endl;

	sf::sleep(now_Time + sleep_Time - clock.getElapsedTime());
	delta_Time = clock.getElapsedTime() - now_Time;
	now_Time += delta_Time;

	//-----------------------------------------------
	//|			|			|			|			|
	//Now		Now			Now			Now			Now
	//		|Sub|		|Sub|		|Sub|		|Sub|
	//---delta------delta-------delta-------delta----
	//经实践验证，双变量足以应对可变时间
}

void ened()
{}
