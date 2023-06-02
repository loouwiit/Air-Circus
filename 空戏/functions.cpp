#include "functions.hpp"

namespace SinCos
{
	float(*sin)(float) = fsin;
	float(*cos)(float) = fcos;
	float(*atan2)(float, float) = satan2; //atan2ÐÔÄÜÌáÉýºÃÏñ²»´ó£¬»¹ÊÇÓÃ±ê×¼¿â°É
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
	//cout << "ÊäÈëÈÎÒâ×Ö·û¼ÌÐø¡­¡­";	std::cin >> pause;
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
	//for (unsigned short i = 0; i < 50000; i++) delta += abs((::sin(x) + ::cos(x)) - (fsin(x) + fcos(x))) / 50000;
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

	//cout << "ÊäÈëÈÎÒâ×Ö·û¼ÌÐø¡­¡­"; std::cin.ignore();

	//static sf::Vector2f force_old{};
	//players[0].froce(force_old);
	//players[0].froce(players[0].get_Position() * -1.0f, Time::delta_Time.asMilliseconds() / 1000.f);
	//force_old = players[0].get_Position() * 1.0f;

	//float x = 0;
	//float y = 0;
	//float s = 0;
	//float f = 0;
	//sf::Clock clock;
	//clock.restart();
	//for (int i = 0; i < 1000000; i++)
	//{
	//	x = rand(); y = rand();
	//	f += SinCos::fatan2(y, x);
	//}
	//std::cout << "[debug]f: " << clock.restart().asMilliseconds() << "ms = " << f << endl;

	//clock.restart();
	//for (int i = 0; i < 1000000; i++)
	//{
	//	x = rand(); y = rand();
	//	s += SinCos::satan2(y, x);
	//}
	//std::cout << "[debug]s: " << clock.restart().asMilliseconds() << "ms = " << s << endl;
	//std::cin.get();
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

float SinCos::fatan2(float y, float x)
{
	float ax = std::abs(x), ay = std::abs(y);
	float a = std::min(ax, ay) / (std::max(ax, ay) + (float)DBL_EPSILON);
	float s = a * a;
	float r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
	if (ay > ax) r = 1.57079637f - r;
	if (x < 0) r = 3.14159274f - r;
	if (y < 0) r = -r;
	return r;
}

void SinCos::Change_Function(Func f, bool is_Fast)
{
	using namespace SinCos;
	switch (f)
	{
	case SinCos::Func::Sin:
		sin = is_Fast ? fsin : ssin;
		break;
	case SinCos::Func::Cos:
		cos = is_Fast ? fcos : scos;
		break;
	case SinCos::Func::Tan2:
		atan2 = is_Fast ? fatan2 : satan2;
		break;
	default:
		break;
	}
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
	case Event::Resized:
	{
		camera.set_Proportion((float)event.size.height / (float)event.size.width);
		break;
	}
	}
}

void Event::event_Mouse(Event::MouseMoveEvent& mouse)
{
	//cout << "Event::event_Mouse:mouse move to " << mouse.x << ',' << mouse.y << endl;
	//cout << "[debug]Event::event_Moouse:" << mouse.x << ',' << mouse.y << ':' << atan2f(mouse.y, mouse.x) << endl;

	//players[0].set_Position(mouse.x , mouse.y);
	//players[0].set_Velocity(0, 0);
}

void Event::event_Keyboard(Event::KeyEvent& key, bool is_Press)
{
	using Time::now_Time;
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
		players[0].set_Key(Fighter::Key::W, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::S:
	{
		//players[0].set_Velocity({ 0,0 });
		players[0].set_Key(Fighter::Key::S, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::A:
	{
		//players[0].rotate(-0.005f * Time::delta_Time.asMilliseconds());
		players[0].set_Key(Fighter::Key::A, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::D:
	{
		//players[0].rotate(+0.005f * Time::delta_Time.asMilliseconds());
		players[0].set_Key(Fighter::Key::D, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::Q:
	{
		players[0].set_Key(Fighter::Key::Q, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::E:
	{
		players[0].set_Key(Fighter::Key::E, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::I:
	{
		players[1].set_Key(Fighter::Key::W, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::K:
	{
		players[1].set_Key(Fighter::Key::S, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::J:
	{
		players[1].set_Key(Fighter::Key::A, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::L:
	{
		players[1].set_Key(Fighter::Key::D, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::U:
	{
		players[1].set_Key(Fighter::Key::Q, is_Press, now_Time.asMilliseconds());
		break;
	}
	case Key::O:
	{
		players[1].set_Key(Fighter::Key::E, is_Press, now_Time.asMilliseconds());
		break;
	}
#ifdef _DEBUG
	case Key::H:
	{
		players[0].set_Position(-10 * Meter, 0 * Meter);
		players[1].set_Position(10 * Meter, 0 * Meter);
		for (char i = 0; i < Player_Number; i++) players[i].set_Rotation(-PIf2);
		break;
	}
	case Key::G:
	{
		if (is_Press) for (char i = 0; i < Player_Number; i++) players[i].set_Velocity(0, 0);
		break;
	}
	case Key::C:
	{
		system("cls");
		cout << "[debug]Çå³ý¿ØÖÆÌ¨" << endl;
		break;
	}
	case Key::F1:
	{
		cout << "[debug]Åö×²²âÊÔ1" << endl;
		players[0].set_Position(0, 0);
		players[1].set_Position(500,500);
		players[0].set_Velocity(100, 100);
		players[1].set_Velocity(-100, -100);
		break;
	}
	case Key::F2:
	{
		cout << "[debug]Åö×²²âÊÔ2" << endl;
		players[1].set_Position(0, 0);
		players[0].set_Position(500,500);
		players[1].set_Velocity(100, 100);
		players[0].set_Velocity(-100, -100);
		break;
	}
	case Key::F3:
	{
		cout << "[debug]Åö×²²âÊÔ3" << endl;
		players[0].set_Position(0, 500);
		players[1].set_Position(500,0);
		players[0].set_Velocity(100, -100);
		players[1].set_Velocity(-100, 100);
		break;
	}
	case Key::F4:
	{
		cout << "[debug]Åö×²²âÊÔ4" << endl;
		players[1].set_Position(0, 500);
		players[0].set_Position(500,0);
		players[1].set_Velocity(100, -100);
		players[0].set_Velocity(-100, 100);
		break;
	}
	case Key::F5:
	{
		cout << "[debug]Åö×²²âÊÔ5" << endl;
		players[0].set_Position(0, 500);
		players[1].set_Position(0,0);
		players[0].set_Velocity(0, -100);
		players[1].set_Velocity(0, 100);
		break;
	}
	case Key::F6:
	{
		cout << "[debug]Åö×²²âÊÔ6" << endl;
		players[1].set_Position(0, 0);
		players[0].set_Position(0,500);
		players[1].set_Velocity(0, 100);
		players[0].set_Velocity(0, -100);
		break;
	}
	case Key::F7:
	{
		cout << "[debug]Åö×²²âÊÔ7" << endl;
		players[0].set_Position(500, 0);
		players[1].set_Position(0,0);
		players[0].set_Velocity(-100, 0);
		players[1].set_Velocity(100, 0);
		break;
	}
	case Key::F8:
	{
		cout << "[debug]Åö×²²âÊÔ8" << endl;
		players[0].set_Position(-500, 0);
		players[1].set_Position(0,0);
		players[0].set_Velocity(100, 0);
		players[1].set_Velocity(-100, 0);
		break;
	}
	case Key::F9:
	{
		if (is_Press) return;
		cout << "[debug]Åö×²²âÊÔ9" << endl;
		players[0].set_Position((rand() - RAND_MAX / 2) * 3000.f / RAND_MAX, (rand() - RAND_MAX / 2) * 3000.f / RAND_MAX);
		players[1].set_Position(0,0);
		players[0].set_Velocity(players[0].get_Position().x * -1.0f, players[0].get_Position().y * -1.0f);
		players[1].set_Velocity(players[0].get_Position().x * +1.0f, players[0].get_Position().y * +1.0f);
		break;
	}
#endif
	}
}


void Compute::compute_Player()
{
	using Time::delta_Time;
	using Time::now_Time;

	for (unsigned i = 0; i < Player_Number; i++)
		players[i].move(Time::delta_Time.asMilliseconds() / 1000.f, now_Time.asMilliseconds());
	for (unsigned i = 0; i < Player_Number; i++)
		for (unsigned j = 0; j < Buoy_Number; j++)
		{
			players[i].collide(buoy[j], delta_Time.asMilliseconds() / 1000.f, now_Time.asMilliseconds());
		}
	for (unsigned i = 0; i < Player_Number; i++)
		for (unsigned j = i + 1; j < Player_Number; j++)
		{
			players[i].collide(players[j], delta_Time.asMilliseconds() / 1000.f, now_Time.asMilliseconds());
		}
}

void Compute::compute_Camera()
{
	bool is_normal = true;

	for (char i = 0; i < Player_Number; i++) for (char j = i + 1; j < Player_Number; j++)
	{
		if (players[i].distance(players[j]) < Near_Distance)
		{
			is_normal = false;
			break;
			break;
		}
	}

	if (is_normal)
	{
		camera.set_Is_Full(true);
		camera.set_Position(sf::Vector2f(-240 / 2 * Meter, -140 / 2 * Meter), sf::Vector2f(240 / 2 * Meter, 140 / 2 * Meter));
	}
	else
	{
		using std::max;
		using std::min;

		sf::Vector2f far;
		sf::Vector2f near;

		camera.set_Is_Full(false);
		far.x = players[0].get_Position().x;
		far.y = players[0].get_Position().y;
		near.x = players[0].get_Position().x;
		near.y = players[0].get_Position().y;
		for (char i = 1; i < Player_Number; i++)
		{
			far.x = max(far.x, players[i].get_Position().x);
			far.y = max(far.y, players[i].get_Position().y);
			near.x = min(near.x, players[i].get_Position().x);
			near.y = min(near.y, players[i].get_Position().y);
		}

		//size = far - near + sf::Vector2f(1000, 1000);
		//size.x /= 16.f;
		//size.y /= 9.f;
		//if (size.x > size.y)
		//{
		//	size.y = size.x * 9;
		//	size.x = size.x * 16;
		//}
		//else
		//{
		//	size.x = size.y * 16;
		//	size.y = size.y * 9;
		//}
		//
		//center = (far + near) * 0.5f; //ÖÐµã

		camera.set_Position(near, far);
	}

	//camera.set_Size(size);
	//camera.set_Center(center);

	camera.compute();
	return;
}
