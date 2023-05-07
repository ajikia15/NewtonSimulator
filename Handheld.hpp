#pragma once
#include <ncurses.h>
#include <fstream>
#include "Drawable.hpp"
#include <iostream>
class Handheld
{
public:
    Handheld()
    {
        game_state = 0;
        mh = LINES - 5; // Responsive design :)
        mw = mh + 15;
        mwin = newwin(mh, mw, (LINES - mh) / 2, (COLS - mw) / 2);
        gwin = newwin(mh / 2 - 1, mw - 6, (LINES - mh) / 2 + 2, (COLS - mw) / 2 + 3);
        getmaxyx(gwin, yGm, xGm);
        points = 0;
    }
    int getState()
    {
        return game_state;
    }
    void start()
    {
        noecho();
        curs_set(0);
        keypad(gwin, true);
        initColors();
        initUI();
        refresh();
        nodelay(mwin, true);
    }
    void initColors()
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);  
        init_pair(2, COLOR_BLACK, COLOR_BLUE);
        init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(4, COLOR_RED, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLUE);
        init_pair(6, COLOR_RED, COLOR_BLUE);
        init_pair(7, COLOR_WHITE, COLOR_BLUE);
    }
    void colorToggle(std::string color, bool state)
    {
        if (color == "red")
        {
            if (state)
                wattron(gwin, COLOR_PAIR(4));
            else
                wattroff(gwin, COLOR_PAIR(4));
        }
    }
    void test()
    {
        mvwprintw(gwin, 10, 10, "test");
    }
    void initUI()
    {
        drawOutline();
        drawControlls();
        drawMainMenu();
    };
    chtype getInput()
    {
        notimeout(mwin, true);
        return wgetch(mwin);
        redraw();
    }
    void add(Drawable drawable)
    {
        addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
    }
    void addAt(int y, int x, chtype ch)
    {
        mvwaddch(gwin, y, x, ch);
    }
    void exitFull()
    {
        beep();
        endwin();
        exit(0);
    }
    int getCoordinates()
    {
        return (rand() % (xGm + 1) - 1);
    }
    void gameEnter()
    {
        beep();
        wclear(gwin);
        game_state = 1;
    }
    void inMainMenu()
    {
        wclear(gwin);
        game_state = 0;
    }
    void clear()
    {
        wclear(gwin);
        wrefresh(gwin);
    }
    void setState(int t)
    {
        game_state = t;
    }
    void drawProjectile()
    {
        wattron(gwin, COLOR_PAIR(4));
        mvwprintw(gwin, yGm - 7, 0, "         /)");
        mvwprintw(gwin, yGm - 6, 0, "    __  /__");
        mvwprintw(gwin, yGm - 5, 0, " .'`  `-'  ``.");
        mvwprintw(gwin, yGm - 4, 0, ":          .-'");
        mvwprintw(gwin, yGm - 3, 0, ":         :");
        mvwprintw(gwin, yGm - 2, 0, " :         `-;");
        mvwprintw(gwin, yGm - 1, 0, "  `.__.-.__.'");
        wattroff(gwin, COLOR_PAIR(4));
    }
    void drawG()
    {
        wattron(gwin, A_STANDOUT);
        mvwprintw(gwin, 2, xGm / 2 - 7, "Apple Catcher");
        wattroff(gwin, A_STANDOUT);
        mvwprintw(gwin, 4, xGm / 2 - 12, "Use arrow keys to move");
        mvwprintw(gwin, 6, xGm / 2 - 8, "Collect projectiles");
        mvwprintw(gwin, 7, xGm / 2 - 7, "Avoid rocks!");
        wattron(gwin, A_BLINK);
        mvwprintw(gwin, yGm - 3, xGm / 2 - 3, "Press Enter to Start");
        wattroff(gwin, A_BLINK);
        drawProjectile();

        wrefresh(mwin);
        wrefresh(gwin);
    };
    void redraw()
    {
        wrefresh(gwin);
        wrefresh(mwin);
    };
    void drawMainMenu()
    {
        int opt_hlt = 0, menu_items_amount = 3, ch;
    };
    int getMaxY()
    {
        return yGm;
    }
    int getMaxX()
    {
        return xGm;
    }
    void drawOutline()
    {
        wattron(mwin, COLOR_PAIR(1));
        box(mwin, 0, 0);
        wattroff(mwin, COLOR_PAIR(1));
        wbkgd(mwin, COLOR_PAIR(2));
        wattron(mwin, A_STANDOUT);
        mvwprintw(mwin, mh - 3, mw / 2 - 9, " NEWTON SIMULATOR ");
        wattroff(mwin, A_STANDOUT);
    };
    void drawControlls()
    {
        // labels
        mvwprintw(mwin, mh / 3 + mh / 3 - 2, 10, "UP");
        mvwprintw(mwin, mh / 3 + mh / 3, 10 + 5, "RIGHT");
        mvwprintw(mwin, mh / 3 + mh / 3 + 2, 10 - 1, "DOWN");
        mvwprintw(mwin, mh / 3 + mh / 3, 10 - 7, "LEFT");
        mvwprintw(mwin, mh / 3 + mh / 3 + 1, 10 * 3, "NEXT");
        mvwprintw(mwin, mh / 3 + mh / 3 + 1, 10 * 3.8, "EXIT");
        // actual controlls
        wattron(mwin, COLOR_PAIR(3));
        mvwprintw(mwin, mh / 3 + mh / 3 - 1, 10, "  ");     // up
        mvwprintw(mwin, mh / 3 + mh / 3, 10 + 2, "  ");     // right
        mvwprintw(mwin, mh / 3 + mh / 3 + 1, 10, "  ");     // down
        mvwprintw(mwin, mh / 3 + mh / 3, 10 - 2, "  ");     // left
        mvwprintw(mwin, mh / 3 + mh / 3, 10 * 3, "    ");   // next
        mvwprintw(mwin, mh / 3 + mh / 3, 10 * 3.8, "    "); // exit
        wattroff(mwin, COLOR_PAIR(3));
    };
    void cMoveLeft()
    {
        drawControlls();
        wattron(mwin, COLOR_PAIR(4));
        mvwprintw(mwin, mh / 3 + mh / 3, 10 - 2, "  "); // left
        wattroff(mwin, COLOR_PAIR(4));
    }
    void cMoveRight()
    {
        drawControlls();
        wattron(mwin, COLOR_PAIR(4));
        mvwprintw(mwin, mh / 3 + mh / 3, 10 + 2, "  "); // right
        wattroff(mwin, COLOR_PAIR(4));
    }
    void cNext()
    {
        drawControlls();
        wattron(mwin, COLOR_PAIR(4));
        mvwprintw(mwin, mh / 3 + mh / 3, 10 * 3, "    "); // next
        wattroff(mwin, COLOR_PAIR(4));
    }
    void setPoints(int p)
    {
        points = p;
    }
    int getPoints()
    {
        return points;
    }
    void displayPoints()
    {
        mvwprintw(mwin, 1, mw - 11, "          ");
        if (getPoints() < 0)
        {
            wattron(mwin, COLOR_PAIR(6));
            mvwprintw(mwin, 1, mw - 12, "Score: %d", getPoints());
            wattroff(mwin, COLOR_PAIR(6));
        }
        else if (getPoints() >= 20)
        {
            wattron(mwin, COLOR_PAIR(5));
            mvwprintw(mwin, 1, mw - 12, "Score: %d", getPoints());
            wattroff(mwin, COLOR_PAIR(5));
        }
        else
        {
            wattron(mwin, COLOR_PAIR(7));
            mvwprintw(mwin, 1, mw - 12, "Score: %d", getPoints());
            wattroff(mwin, COLOR_PAIR(7));
        }
    }
    void displayTime(int t)
    {
        mvwprintw(mwin, 1, 2, "%d", t);
    }
    void displayHealth(int lives)
    {
        for (int j = 0; j < 5; j++)
            mvwprintw(mwin, 1, 11 + j, " ");
        wattron(mwin, COLOR_PAIR(5));
        mvwprintw(mwin, 1, 4, "Lives:");
        for (int j = 0; j < lives; j++)
            mvwprintw(mwin, 1, 11 + j, "X");
        wattroff(mwin, COLOR_PAIR(5));
    }
    void showScoreBoard()
    {
        std::ifstream file("scoreboard.txt");
        if (file.is_open())
        {
            int i = 0;
            std::string line;
            while (std::getline(file, line))
            {
                i++;
                mvwprintw(gwin, i + 4, 2, "%d) %s", i, line.c_str());
            }
            file.close();
        }
    }
    void gameOverScreen()
    {
        wclear(gwin);
        wattron(gwin, A_BLINK);
        mvwprintw(gwin, 1, 2, "GAME OVER!");
        wattroff(gwin, A_BLINK);
        mvwprintw(gwin, 1, xGm - 16, "Your score: %d", getPoints());
        for (int i = 1; i < xGm - 1; i++)
            mvwprintw(gwin, 2, i, "-");

        mvwprintw(gwin, 3, 2, "Top Ranking:");
        showScoreBoard();
    }

private:
    WINDOW *mwin, *gwin;
    int mh, mw, yGm, xGm, game_state, ch, points; // screen dimensions
};
