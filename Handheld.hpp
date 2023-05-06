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
        init_pair(1, COLOR_BLUE, COLOR_BLACK); // border colors
        init_pair(2, COLOR_BLACK, COLOR_BLUE);
        init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(4, COLOR_RED, COLOR_BLACK);
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
        // Screen Screen(mwin, mh, mw);
    };
    chtype getInput()
    {
        // wtimeout(mwin, 150);
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
        return rand() % xGm;
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
    void setState(int t)
    {
        game_state = t;
    }
    void drawApple()
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
        mvwprintw(gwin, 6, xGm / 2 - 8, "Collect apples");
        mvwprintw(gwin, 7, xGm / 2 - 7, "Avoid rocks!");
        wattron(gwin, A_BLINK);
        mvwprintw(gwin, yGm - 3, xGm / 2 - 3, "Press Enter to Start");
        wattroff(gwin, A_BLINK);
        drawApple();

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
    void displayPoints(int p)
    {
        mvwprintw(mwin, 1, mw / 2 - 5, "             ");
        mvwprintw(mwin, 1, mw / 2 - 5, "Score: %d", p);
        // mvwprintw(mwin, 1, 10, "%d", p);
    }
    void displayTime(int t)
    {
        mvwprintw(mwin, 1, 2, "%d", t);
    }
private:
    WINDOW *mwin, *gwin;
    int mh, mw, yGm, xGm, game_state, ch; // screen dimensions
};
