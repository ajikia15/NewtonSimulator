#pragma once
#include <ncurses.h>
#include "Drawable.hpp"

class Player : public Drawable
{
private:
	int points;
	int lives;

public:
	Player()
	{
		points = this->x = this->y = this->oldx = this->oldy = 0;
		lives = 5;
		this->icon = '#';
	}
	Player(int y, int x)
	{
		this->oldx = this->oldy = 0;
		lives = 5;
		this->x = x;
		this->y = y;
		this->icon = '#';
	}
	int getPoints()
	{
		return points;
	}
	void plusPoints()
	{
		points++;
	}
	void deductPoints()
	{
		points--;
	}
	int getLives()
	{
		return lives;
	}
	void deductLives()
	{
		lives--;
	}
	void healUp()
	{
		lives++;
	}
	void kill()
	{
		lives = 0;
	}
	void moveR()
	{
		updatePreviousX(x);
		x++;
	}
	void moveL()
	{
		updatePreviousX(x);
		x--;
	}
	void updatePreviousX(int x)
	{
		oldx = x;
	}
	int getPreviousX()
	{
		return oldx;
	}
};