#pragma once
#include <ncurses.h>

class Apple : public Drawable
{
public:
    Apple(int y, int x /*, int type*/)
    {
        this->y = y;
        this->x = x;
        // if (type)
        this->icon = 'O';
        // if (type == 2)
        // {
        //     this->icon ='+'
        // }
    }
    void updatePreviousY(int y)
    {
        oldy = y;
    }
    int getPreviousY()
    {
        return oldy;
    }

    void moveU()
    {
        y--;
    }
    void moveD(int max, int pY, int pX)
    {
        updatePreviousY(y);
        y++;
    }
};
