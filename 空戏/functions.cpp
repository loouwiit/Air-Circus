#include "functions.hpp"

namespace SinCos
{
	float fsin(float x);
	float fcos(float x);
	float(*sin)(float) = fsin;
	float(*cos)(float) = fcos;
}

#ifdef _DEBUG
void debug()
{
	//sf::Clock clock;
	//sf::Time time[2];
	//float all[2] = { 0,0 };
	//float delta = 0.0f;
	//char pause;
	//
	//using SinCos::fsin;
	//using SinCos::fcos;

	//for (float i = -PIf; i < PIf; i += 0.001f) fsin_d(i);
	//cout << "输入任意字符继续……";	std::cin >> pause;
	//for (float i = -PIf; i < PIf; i += 0.001f) fcos_d(i);
	//
	//clock.restart();
	//for (float i = -TAUf; i < TAUf; i += 0.00001f) all[0] += ::sin(i) + ::cos(i);
	//time[0] = clock.getElapsedTime();
	//
	//clock.restart();
	//for (float i = -TAUf; i < TAUf; i += 0.00001f) all[1] += fsin(i) + fcos(i);
	//time[1] = clock.getElapsedTime();
	//
	//srand(rand());
	//float x = (rand() - 16000) / 1000.0f;
	//for (unsigned short i = 0; i < 50000; i++) delta += ((::sin(x) + ::cos(x)) - (fsin(x) + fcos(x))) / 50000;
	//	cout << "delta:" << delta << endl;
	//
	//for (unsigned short i = 0; i < 1000; i++)
	//{
	//	float x = (rand() - 10000) / 1000.0f;
	//	float a = x;
	//
	//	printf("x:%f\t|", x);
	//
	//	while (x > PIf) x -= TAUf;
	//	while (x < -PIf) x += TAUf;
	//	printf("x:%f\t|", x);
	//
	//	printf("d:%d\t|", (int)(a / TAUf));
	//
	//	if (a > PIf) a -= (int)((a + PIf) / TAUf) * TAUf;
	//	if (a < -PIf) a += (int)((a - PIf) / -TAUf) * TAUf;
	//	printf("a:%f\t|", a);
	//
	//	printf("delta:%f\n", a - x);
	//}
	//
	//cout << "c:" << all[0] << endl
	//	<< "time:" << time[0].asMilliseconds() << "ms" << endl
	//	<< "f:" << all[1] << endl
	//	<< "time:" << time[1].asMilliseconds() << "ms" << endl;

	//cout << "输入任意字符继续……"; std::cin.ignore();

	//static sf::Vector2f force_old{};
	//players[0].froce(force_old);
	//players[0].froce(players[0].get_Position() * -1.0f, Time::delta_Time.asMilliseconds() / 1000.f);
	//force_old = players[0].get_Position() * 1.0f;
}

//float fsin_d(float x)
//{
//	using SinCos::fsin;
//	using SinCos::fcos;
//
//	while (x > PIf) x -= PIf;
//	while (x < -PIf) x += PIf;
//	cout << x << '|' << sin(x) << '|' << fsin(x) << '|' << sin(x) - fsin(x) << endl;
//	return fsin(x);
//}
//
//float fcos_d(float x)
//{
//	using SinCos::fsin;
//	using SinCos::fcos;
//
//	while (x > PIf) x -= PIf;
//	while (x < -PIf) x += PIf;
//	cout << x << '|' << cos(x) << '|' << fcos(x) << '|' << cos(x) - fcos(x) << endl;
//	return fcos(x);
//}
#endif //_DEBUG

float SinCos::fsin(float x)
{
	//while (x > PIf) x -= TAUf;
	//while (x < -PIf) x += TAUf;
	if (x > PIf) x -= (int)((x + PIf) / TAUf) * TAUf;
	if (x < -PIf) x += (int)((x - PIf) / -TAUf) * TAUf;
	return 0.987862f * x - 0.155271f * x * x * x + 0.00564312f * x * x * x * x * x;
}

float SinCos::fcos(float x)
{
	return fsin(x + PIf2);
}

void SinCos::using_Stand()
{
	using namespace SinCos;
	sin = ssin;
	cos = scos;
}

void SinCos::using_Fast()
{
	using namespace SinCos;
	sin = fsin;
	cos = fcos;
}

void Event::event(Event& event)
{
	switch (event.type)
	{
	case Event::Closed:
	{
		cout << "Event:event:window close" << endl;
		window.close();
		break;
	}
	case Event::MouseMoved:
	{
		sf::Vector2f position = window.mapPixelToCoords({ event.mouseMove.x,event.mouseMove.y });
		event.mouseMove.x = (int)position.x;
		event.mouseMove.y = (int)position.y;
		event_Mouse(event.mouseMove);
		break;
	}
	case Event::KeyPressed:
	{
		event_Keyboard(event.key, true);
		break;
	}
	case Event::KeyReleased:
	{
		event_Keyboard(event.key, false);
		break;
	}
	}
}

void Event::event_Mouse(Event::MouseMoveEvent& mouse)
{
	//cout << "Event::event_Mouse:mouse move to " << mouse.x << ',' << mouse.y << endl;
}

void Event::event_Keyboard(Event::KeyEvent& key, bool is_Press)
{
	using Key = sf::Keyboard;
	//cout << "Event::event_Keyboard:key " << key.code << ' ' << (is_Press ? "press" : "free") << endl;
	switch (key.code)
	{
	case Key::Escape:
	{
		sf::Event close{};
		close.type = sf::Event::Closed;
		event(close);
		break;
	}
	case Key::W:
	{
		//players[0].set_Velocity(500.f * players[0].get_Cos_Rotate(), 500.f * players[0].get_Sin_Rotate());
		//players[0].froce(100.f * players[0].get_Cos_Rotate(), 100.f * players[0].get_Sin_Rotate());
		players[0].set_Key(Fighter::Key::W, is_Press);
		break;
	}
	case Key::S:
	{
		//players[0].set_Velocity({ 0,0 });
		players[0].set_Key(Fighter::Key::S, is_Press);
		break;
	}
	case Key::A:
	{
		//players[0].rotate(-0.005f * Time::delta_Time.asMilliseconds());
		players[0].set_Key(Fighter::Key::A, is_Press);
		break;
	}
	case Key::D:
	{
		//players[0].rotate(+0.005f * Time::delta_Time.asMilliseconds());
		players[0].set_Key(Fighter::Key::D, is_Press);
		break;
	}
	case Key::Q:
	{
		players[0].set_Key(Fighter::Key::Q, is_Press);
		break;
	}
	case Key::E:
	{
		players[0].set_Key(Fighter::Key::E, is_Press);
		break;
	}
	case Key::I:
	{
		players[1].set_Key(Fighter::Key::W, is_Press);
		break;
	}
	case Key::K:
	{
		players[1].set_Key(Fighter::Key::S, is_Press);
		break;
	}
	case Key::J:
	{
		players[1].set_Key(Fighter::Key::A, is_Press);
		break;
	}
	case Key::L:
	{
		players[1].set_Key(Fighter::Key::D, is_Press);
		break;
	}
	case Key::U:
	{
		players[1].set_Key(Fighter::Key::Q, is_Press);
		break;
	}
	case Key::O:
	{
		players[1].set_Key(Fighter::Key::E, is_Press);
		break;
	}
	case Key::H: //[debug]
	{
		players[0].set_Position(-10 * Meter, 0 * Meter);
		players[1].set_Position(10 * Meter, 0 * Meter);
		break;
	}
	case Key::G:
	{
		if (is_Press) for (char i = 0; i < Player_Number; i++) players[i].set_Velocity(0, 0);
		break;
	}
	}
}


void Compute::compute_Player()
{
	for (unsigned i = 0; i < Player_Number; i++)
		players[i].move(Time::delta_Time.asMilliseconds() / 1000.f);
	for (unsigned i = 0; i < Player_Number; i++)
	{
		for (unsigned j = i + 1; j < Player_Number; j++)
		{
			players[i].collide(players[j]);
		}
		for (unsigned j = 0; j < Buoy_Number; j++)
		{
			players[i].collide(buoy[j]);
		}
	}
}
