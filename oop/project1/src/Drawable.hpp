#pragma once
#include <ncurses.h>
class Drawable
{
protected:
	int y, x, oldx, oldy, points;
	chtype icon;

public:
	Drawable()
	{
		y = x = oldx = oldy = points = 0;
		icon = '*';
	}
	Drawable(int y, int x, chtype ch)
	{
		this->x = x;
		this->y = y;
		this->icon = ch;
	}
	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}

	chtype getIcon()
	{
		return icon;
	}
};
