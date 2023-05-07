#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <chrono>
#include "Handheld.hpp"
#include "Apple.hpp"
#include "Drawable.hpp"
#include "Empty.hpp"
#include "Player.hpp"
#define INTERVAL 150
class NewtonGame
{
public:
    NewtonGame()
    {
        handheld = Handheld();
        handheld.start();
        game_over = appleOnScreen = pAdded = false;
    }
    ~NewtonGame()
    {
        delete apple;
        delete player;
    }
    void processInput()
    {
        chtype input = handheld.getInput();
        // process this crap
        if (input != ERR)
            switch (input)
            {
            case 'x':
                handheld.inMainMenu();
                break;
            case 'a':
                player->moveL();
                handheld.cMoveLeft();
                break;
            case 'd':
                player->moveR(); // !
                handheld.cMoveRight();
                break;
            default:
                break;
            }
    }
    void updateState(int startTime)
    {
        handheld.displayTime(startTime);
        int y, x;
        if (!getPlayerState())
        {
            playerAdd();
        }
        else
        {
            handheld.add(Empty(handheld.getMaxY() - 1, player->getPreviousX()));
            handheld.add(*player);
        }

        if (!getAppleState())
        {
            handheld.displayPoints(player->getPoints());
            int y, x;
            x = handheld.getCoordinates();
            y = 0;
            appleAdd(y, x);
        }

        if (getAppleState())
        {

            if (startTime % (INTERVAL - player->getPoints()) == 0) // speed of the game
            {
                if (apple->getY() < handheld.getMaxY() - 1)
                {
                    apple->moveD();
                    handheld.add(Empty(apple->getPreviousY(), apple->getX()));
                    handheld.colorToggle("red", 1);
                    handheld.add(*apple);
                    handheld.colorToggle("red", 0);
                }
                else
                {
                    if (apple->getX() != player->getX())
                        player->deductPoints();
                    else
                        player->plusPoints();
                    appleDel();
                }
            }
        }
    }
    bool getPlayerState()
    {
        return pAdded;
    }
    void playerAdd()
    {
        player = new Player(handheld.getMaxY() - 1, 3);
        pAdded = true;
    }
    void playerDel()
    {
        pAdded = false;
    }

    int getState()
    {
        return handheld.getState();
    }
    void setState(int t)
    {
        handheld.setState(t);
    }
    void gameEnter()
    {
        handheld.gameEnter();
    }
    bool getAppleState()
    {
        return appleOnScreen;
    }
    void appleAdd(int y, int x)
    {
        apple = new Apple(y, x);
        appleOnScreen = true;
    }
    void appleDel()
    {
        handheld.add(Empty(apple->getY(), apple->getX()));
        appleOnScreen = false;
        delete apple;
    }
    void redraw()
    {
        handheld.redraw();
    }
    void drawG()
    {
        handheld.drawG();
    }
    bool isOver()
    {
        return game_over;
    }
    void exit()
    {
        handheld.exitFull();
    }

private:
    Handheld handheld;
    bool game_over;
    bool pAdded;
    bool appleOnScreen;
    Apple *apple;
    Player *player;
};
