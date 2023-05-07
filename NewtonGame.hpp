#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include "Handheld.hpp"
#include "Projectile.hpp"
#include "Drawable.hpp"
#include "Empty.hpp"
#include "Player.hpp"
#define INTERVAL 150
#define DEBUG 0
class NewtonGame
{
private:
    std::string pName;
    Handheld handheld;
    bool game_over;
    bool pAdded;
    bool projectileOnScreen;
    Projectile *projectile;
    Player *player;

public:
    NewtonGame(const std::string &name)
    {
        pName = name;
        handheld = Handheld();
        handheld.start();
        game_over = projectileOnScreen = pAdded = false;
    }
    ~NewtonGame()
    {
        delete projectile;
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
            case KEY_LEFT:
            case 'a':
                if (player->getX() != 0)
                    player->moveL();
                handheld.cMoveLeft();
                break;
            case KEY_RIGHT:
            case 'd':
                if (player->getX() != handheld.getMaxX() - 1)

                    player->moveR();
                handheld.cMoveRight();
                break;
            default:
                break;
            }
    }
    void updateState(int startTime)
    {
        if (DEBUG)
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

        if (!getProjectileState())
        {
            handheld.setPoints(player->getPoints());
            handheld.displayPoints();
            handheld.displayHealth(player->getLives());
            int y, x;
            x = handheld.getCoordinates();
            y = -1;
            projectileAdd(y, x);
        }

        if (getProjectileState())
        {

            if (startTime % (INTERVAL - player->getPoints()) == 0) // speed of the game
            {
                if (projectile->getY() < handheld.getMaxY() - 1)
                {
                    projectile->moveD();
                    handheld.add(Empty(projectile->getPreviousY(), projectile->getX()));
                    handheld.colorToggle("red", 1);
                    handheld.add(*projectile);
                    handheld.colorToggle("red", 0);
                }
                else
                {
                    if (projectile->getX() != player->getX())
                    {
                        player->deductPoints();
                        player->deductLives();
                        if (player->getLives() == 0)
                        {
                            handheld.setState(2);
                        }
                    }
                    else
                        player->plusPoints();
                    projectileDel();
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
    bool getProjectileState()
    {
        return projectileOnScreen;
    }
    void projectileAdd(int y, int x)
    {
        projectile = new Projectile(y, x);
        projectileOnScreen = true;
    }
    void projectileDel()
    {
        handheld.add(Empty(projectile->getY(), projectile->getX()));
        projectileOnScreen = false;
        delete projectile;
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
    void gameOverScreen()
    {
        handheld.gameOverScreen();
    }
    void clearGameScreen()
    {
        handheld.clear();
    }
    std::string getName()
    {
        return pName;
    }
    int getScore()
    {
        return player->getPoints();
    }
};
