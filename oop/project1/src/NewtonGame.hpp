#pragma once
#include <random>
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
            // speed of the game

            // if (startTime % (INTERVAL - player->getPoints() * (player->getPoints() > 5 && player->getPoints() < 10 ? 2 : 1)) == 0)
            if (startTime % (INTERVAL - (player->getPoints() * 2)) == 0)

            {
                if (projectile->getY() < handheld.getMaxY() - 1)
                {
                    projectile->moveD();
                    handheld.add(Empty(projectile->getPreviousY(), projectile->getX()));
                    handheld.colorToggle(projectile->getType(), 1);
                    handheld.add(*projectile);
                    handheld.colorToggle(projectile->getType(), 0);
                }
                else
                {
                    int type = projectile->getType();
                    if (projectile->getX() == player->getX())
                    {
                        if (!type)
                            player->plusPoints();
                        else if (type == 2)
                            player->healUp();
                        else if (type)
                            player->kill();
                    }
                    else
                    {
                        if (!type)
                        {
                            player->deductPoints();
                            player->deductLives();
                        }
                    }
                    if (player->getLives() == 0)
                    {
                        handheld.clearStatusBar();
                        handheld.setState(2);
                    }
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
    int getRandomType()
    {
        // int type = rand() % 10;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);
        int type = dis(gen);
        if (type < 7)
        {
            type = 0;
        }
        else if (type < 8)
        {
            type = 1;
        }
        else
        {
            type = 2;
        }
        return type;
    }

    void projectileAdd(int y, int x)
    {
        int type = getRandomType();
        projectile = new Projectile(y, x, type);
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
        handheld.redraw();
    }
    void gameGuideScreen()
    {
        handheld.clear();
        handheld.gameGuide();
        handheld.redraw();
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
