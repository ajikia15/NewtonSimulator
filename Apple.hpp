#pragma once
#include <ncurses.h>

class Apple : public Drawable
{
public:
    Apple(int y, int x)
    {
        this->y = y;
        this->x = x;
        this->icon = 'O';
    }
    void updatePreviousY(int y)
    {
        oldy = y;
    }
    int getPreviousY()
    {
        return oldy;
    }
    void moveD()
    {
        updatePreviousY(y);
        y++;
    }
};
