#pragma once
#include <SFML/Graphics.hpp>

class Text : public sf::Text
{
public:
	Text();
	Text(Text&) : Text() {}
	Text(Text&&) : Text() {}
	~Text();

private:
	static sf::Font* font;
};

class Score : public Text
{
public:
	Score();

	void set_Score(int score);
	void add(int delta);
	int get_Score();

private:
	int self_Score = 0;
	char self_Buffer[4] = "000";
};