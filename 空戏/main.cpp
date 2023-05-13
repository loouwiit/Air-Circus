#include <iostream>
#include <SFML/Graphics.hpp>
#include "Fighter.hpp"
#include "Camera.hpp"
#include "Substance.hpp"
#include "functions.hpp"

namespace Time
{
	sf::Time now_Time = sf::milliseconds(0);
	sf::Time delta_Time = sf::milliseconds(16);
	sf::Time sleep_Time = sf::milliseconds(1000 / FPS);
}

sf::RenderWindow window;
Camera camrea;
Fighter players[Player_Number];
Buoy buoy[4];

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
	camrea.set_Size(250 * Meter, 150 * Meter);
	camrea.set_Center(0.f, 0.f);
	window.setView(camrea.get_View());

	players[0].set_Position(-10 * Meter, 0 * Meter);
	players[0].set_Color(sf::Color::Red);
	players[1].set_Position(10 * Meter, 0 * Meter);
	players[1].set_Color(sf::Color::Blue);

	buoy[0].set_Position(-100 * Meter, -50 * Meter);
	buoy[1].set_Position(-100 * Meter, +50 * Meter);
	buoy[2].set_Position(+100 * Meter, -50 * Meter);
	buoy[3].set_Position(+100 * Meter, +50 * Meter);

	//[debug]
	players[0].set_Velocity(0, 800.0f);
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
}

void draw()
{
	window.clear(sf::Color::White);

	for (char i = 0; i < Player_Number; i++) window.draw(players[i]);
	for (char i = 0; i < 4; i++) window.draw(buoy[i]);

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
