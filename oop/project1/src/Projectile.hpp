#pragma once
#include <ncurses.h>

class Projectile : public Drawable
{
public:
    Projectile(int y, int x, int type)
    {
        this->y = y;
        this->x = x;
        this->type = type;

        switch (type)
        {
        case 0:
            icon = 'O'; // apple
            break;
        case 1:
            icon = '*'; // rock
            break;
        case 2:
            icon = '+'; // health kit
            break;
        default:
            icon = 'O'; // default to apple
            break;
        }
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

    int getType()
    {
        return type;
    }

private:
    int type;
};