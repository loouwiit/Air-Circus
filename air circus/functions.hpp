#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "fighter.hpp"
#include "camera.hpp"

using std::cout;
constexpr char endl = '\n';
constexpr unsigned short FPS = 60;
constexpr unsigned char Player_Number = 2;
constexpr unsigned char Buoy_Number = 4;
constexpr float PIf = (float)(3.14159265358979323846);
constexpr double PI = 3.14159265358979323846;
constexpr float PIf2 = (float)(3.14159265358979323846 / 2.0);
constexpr double PI2 = 3.14159265358979323846 / 2.0;
constexpr float TAUf = (float)(3.14159265358979323846 * 2.0);
constexpr double TAU = 3.14159265358979323846 * 2;
constexpr char Meter = 100;
constexpr unsigned Near_Distance = 50 * Meter;
constexpr short Position_Point_X_Number = 24;
constexpr short Position_Point_Y_Number = 14;
constexpr unsigned Max_Player_Speed = 10000;

#ifdef _DEBUG
constexpr wchar_t Window_Name[] = L"空戏_debug";
#else
constexpr wchar_t Window_Name[] = L"空戏";
#endif

extern sf::RenderWindow window;
extern Fighter players[Player_Number];
extern Camera camera;
extern Buoy buoy[Buoy_Number];

namespace Time
{
	extern sf::Time now_Time;
	extern sf::Time delta_Time;
	extern sf::Time sleep_Time;
}

#ifdef  _DEBUG
void debug();
//float fsin_d(float x);
//float fcos_d(float x);
#endif //_DEBUG

namespace SinCos
{
	float fsin(float x);
	float fcos(float x);
	float fatan2(float y, float x);
	constexpr float (*ssin)(float) = ::sinf;
	constexpr float (*scos)(float) = ::cosf;
	constexpr float (*satan2)(float, float) = ::atan2f;
	extern float(*sin)(float);
	extern float(*cos)(float);
	extern float(*atan2)(float, float);

	enum class Func :signed char
	{
		Sin,Cos,Tan2
	};

	void Change_Function(Func f, bool is_Fast);
}

namespace Event
{
	using sf::Event;
	void event(Event& event);
	void event_Mouse(Event::MouseMoveEvent& mouse);
	void event_Keyboard(Event::KeyEvent& key, bool is_Press);
}

namespace Compute
{
	void compute_Player();
	void compute_Buoy();
	void compute_Camera();
}
