#include <iostream>
#include <ncurses.h>
#include <fstream>
#include "player.h"

class Game
{
public:
    Game();

    void drawScreen();
    void drawApple();
    void drawControlls();
    void nextArr();
    void extArr();
    void upArr();
    void downArr();
    void hOpt1();
    void hOpt2();
    void gameOverScreen();
    void showScoreboard();

private:
    int x, y, yGm, xGm, hlt;
    WINDOW *mwin;
    WINDOW *gwin;
};
Game::Game()
{
}
void drawScreen()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // border colors
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_RED, COLOR_BLACK);
}
void drawApple(WINDOW *w, int y)
{
    wattron(w, COLOR_PAIR(4));
    mvwprintw(w, y - 8, 1, "          .:");
    mvwprintw(w, y - 7, 1, "    __ :'__");
    mvwprintw(w, y - 6, 1, " .'`  `-'  ``.");
    mvwprintw(w, y - 5, 1, ":          .-'");
    mvwprintw(w, y - 4, 1, ":         :");
    mvwprintw(w, y - 3, 1, " :         `-;");
    mvwprintw(w, y - 2, 1, "  `.__.-.__.'");
    wattroff(w, COLOR_PAIR(4));
}
void drawControlls(WINDOW *w, int y, int x)
{
    // labels
    mvwprintw(w, y - y / 4 - 2, x, "UP");
    mvwprintw(w, y - y / 4, x + 5, "RIGHT");
    mvwprintw(w, y - y / 4 + 2, x - 1, "DOWN");
    mvwprintw(w, y - y / 4, x - 7, "LEFT");
    mvwprintw(w, y - y / 4 + 1, x * 3, "NEXT");
    mvwprintw(w, y - y / 4 + 1, x * 3.8, "EXIT");
    // actual controlls
    wattron(w, COLOR_PAIR(12));
    mvwprintw(w, y - y / 4 - 1, x, "  ");     // up
    mvwprintw(w, y - y / 4, x + 2, "  ");     // right
    mvwprintw(w, y - y / 4 + 1, x, "  ");     // down
    mvwprintw(w, y - y / 4, x - 2, "  ");     // left
    mvwprintw(w, y - y / 4, x * 3, "    ");   // next
    mvwprintw(w, y - y / 4, x * 3.8, "    "); // exit
    wattroff(w, COLOR_PAIR(12));
}
void nexArr(WINDOW *w, int y, int x)
{
    wattron(w, COLOR_PAIR(10));
    mvwprintw(w, y - y / 4, x * 3, "NEXT");
    wattroff(w, COLOR_PAIR(10));
}
void extArr(WINDOW *w, int y, int x)
{
    wattron(w, COLOR_PAIR(10));
    mvwprintw(w, y - y / 4, x * 3.8, "EXIT");
    wattroff(w, COLOR_PAIR(10));
}
void upArr(WINDOW *w, int y, int x)
{
    wattron(w, COLOR_PAIR(10));
    mvwprintw(w, y - y / 4 - 1, x, "  "); // up
    wattroff(w, COLOR_PAIR(10));
    wrefresh(w);
}
void downArr(WINDOW *w, int y, int x)
{
    wattron(w, COLOR_PAIR(10));
    mvwprintw(w, y - y / 4 + 1, x, "  "); // up
    wattroff(w, COLOR_PAIR(10));
    wrefresh(w);
}
void hOpt1(WINDOW *gwin, int *hlt, int y, int x)
{
    *hlt = 1;
    wattron(gwin, A_STANDOUT);
    mvwprintw(gwin, y / 2 - 1, x / 2 - 5, " Start > ");
    wattroff(gwin, A_STANDOUT);
    mvwprintw(gwin, y / 2 + 1, x / 2 - 4, " Exit   ");
}
void hOpt2(WINDOW *gwin, int *hlt, int y, int x)
{
    *hlt = 2;
    wattron(gwin, A_STANDOUT);
    mvwprintw(gwin, y / 2 + 1, x / 2 - 4, " Exit > ");
    wattroff(gwin, A_STANDOUT);
    mvwprintw(gwin, y / 2 - 1, x / 2 - 5, " Start   ");
}
void gameOverScreen(WINDOW *gwin, int y, int x)
{
    wclear(gwin);
    mvwprintw(gwin, 1, 1, "Game Over!");
}
void showScoreboard(WINDOW *gwin)
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
        // cout << i++ << " " << line << endl;
        file.close();
    }
}
using namespace std;
int main()
{
    drawScreen();

    int mh = LINES - 5; // Responsive design :)
    int mw = mh + 15;
    int selected_option = 0;

    WINDOW *mwin = newwin(mh, mw, (LINES - mh) / 2, (COLS - mw) / 2);
    wattron(mwin, COLOR_PAIR(10));
    box(mwin, 0, 0);
    wattroff(mwin, COLOR_PAIR(10));
    WINDOW *gwin = newwin(mh / 2 - 1, mw - 6, (LINES - mh) / 2 + 2, (COLS - mw) / 2 + 3);
    wbkgd(mwin, COLOR_PAIR(11));
    // WINDOW * gwin = subwin(mwin, 5, 5, 2, 2); // can't get subwins to work for some reason
    wattron(gwin, COLOR_PAIR(10));
    box(gwin, 0, 0);
    wattroff(gwin, COLOR_PAIR(10));
    refresh();
    wrefresh(mwin);

    int yGm, xGm; // game screen dimensions
    getmaxyx(gwin, yGm, xGm);
    wattron(mwin, A_BLINK);
    mvwprintw(mwin, mh - 3, mw / 2 - 7, " GAMEBOY E9999 ");
    wattroff(gwin, A_BLINK);

    drawControlls(mwin, yGm * 2, xGm / 4);

    wrefresh(mwin);

    while (1)
    {
        int opt_hlt = 0, menu_items_amount = 3, ch;
        keypad(gwin, true);
        wattron(gwin, A_BOLD);
        mvwprintw(gwin, yGm / 2 - 4, xGm / 2 - 7, "Apple Catcher");
        drawApple(gwin, yGm);
        wattroff(gwin, A_BOLD);
        hOpt1(gwin, &opt_hlt, yGm, xGm);
        while (!selected_option)
        {
            ch = wgetch(gwin);
            switch (ch)
            {
            case KEY_UP:
                drawControlls(mwin, yGm * 2, xGm / 4);
                wrefresh(mwin);
                hOpt1(gwin, &opt_hlt, yGm, xGm);
                upArr(mwin, yGm * 2, xGm / 4);
                drawControlls(mwin, yGm * 2, xGm / 4);
                break;
            case KEY_DOWN:
                drawControlls(mwin, yGm * 2, xGm / 4);
                wrefresh(mwin);
                hOpt2(gwin, &opt_hlt, yGm, xGm);
                downArr(mwin, yGm * 2, xGm / 4);
                // timeout(5000);
                // drawControlls(mwin, yGm * 2, xGm / 4);
                // wrefresh(mwin);
                break;
            case 'x':
                extArr(mwin, yGm * 2, xGm / 4);
                beep();
                endwin();
                exit(0);
            case 10:
                nexArr(mwin, yGm * 2, xGm / 4);
                beep();
                wclear(gwin);
                selected_option = opt_hlt;
                break;
            }
        }
        while (selected_option == 1)
        {
            drawControlls(mwin, yGm * 2, xGm / 4);
            wrefresh(mwin);
            wclear(gwin);
            Player *p = new Player(gwin, yGm - 2, 1, '|', mwin);
            do
                p->display();
            while (p->getmv() != 'x' && p->gameGoing());
            wclear(gwin);
            delete p;
            selected_option = -1;
            break;
        }
        while (selected_option == -1)
        {
            wattron(gwin, A_BLINK);
            mvwprintw(gwin, 1, 2, "GAME OVER!");
            wattroff(gwin, A_BLINK);
            mvwprintw(gwin, 1, xGm - 16, "Your score: 0 ");
            for (int i = 1; i < xGm - 1; i++)
                mvwprintw(gwin, 2, i, "-");

            mvwprintw(gwin, 3, 2, "Top Ranking:");
            // list the players here
            showScoreboard(gwin);
            if (wgetch(gwin) == 'x')
            {
                wclear(gwin);
                wrefresh(gwin);
                selected_option = 0;
                break;
            }
        }
        while (selected_option == 2)
        {
            beep();
            extArr(mwin, yGm * 2, xGm / 4);
            wclear(gwin);
            endwin();
            delwin(gwin);
            exit(0);
            break;
        }
    }

    wgetch(gwin); //
    endwin();

    return 0;
}

/*  Scoreboard

    DEFINITION
    Username Highscore CurrentScore

    STARTING
    set username = "" currentScore = 0 HighScore = 0

    After each game the player will have to submit a name.

    1)Check if the username is already there.
        a) if the username is already there, compare the highscore.

    2)Else add that player and add the highscore


*/