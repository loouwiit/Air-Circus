#include "score.hpp"

sf::Font* Text::font = nullptr;

Text::Text()
{
	if (font == nullptr)
	{
		font = new sf::Font;
		font->loadFromFile("font.otf");
	}

	setFont(*font);
}

Text::~Text()
{
	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}
}

Score::Score()
{
	setCharacterSize(25);
	setString(self_Buffer);
	sf::FloatRect bound = getGlobalBounds();
	setOrigin(bound.width / 2.0f, bound.height / 2.0f);
	setPosition(20, 30);
	setRotation(90.0f);
	sf::Color color = sf::Color(0xFFFFFF66);
	setFillColor(color);
	setOutlineColor(sf::Color::Black);
}

void Score::set_Score(int score)
{
	self_Score = score;
	if (self_Score <= 999)
	{
		sprintf(self_Buffer, "%03d", self_Score);
		setString(self_Buffer);
	}
	else
	{
		setString("999");
	}
}

void Score::add(int delta)
{
	self_Score += delta;
	if (self_Score <= 999)
	{
		sprintf(self_Buffer, "%03d", self_Score);
		setString(self_Buffer);
	}
	else
	{
		setString("999");
	}
}

int Score::get_Score()
{
	return self_Score;
}
