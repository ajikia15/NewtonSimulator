#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <ncurses.h>
class Projectile
{
public:
    Projectile(WINDOW *gwin, int yGm, int xGm);
    void startProjectile();
    void fall();
    void collide();

private:
    int yGm, xGm, c, y, x;
    WINDOW *gwin;
};
Projectile::Projectile(WINDOW *gwin, int yGm, int xGm)
{
    this->gwin = gwin;
    this->yGm = yGm;
    this->xGm = xGm;
}
class Player
{
public:
    Player(WINDOW *win, int yc, int xc, char c, WINDOW *mwin);

    void mvleft();
    void mvright();
    void clearTrails();
    void drawControlls();
    int getmv();
    void display();
    bool gameGoing();

private:
    int x, y, ax, ay, width, height;
    char character;
    WINDOW *curwin;
    WINDOW *main;
};

Player::Player(WINDOW *win, int yc, int xc, char c, WINDOW *mwin)
{
    curwin = win;
    main = mwin;
    y = yc + 1;
    x = xc;

    getmaxyx(curwin, height, width);
    ay = height * 2;
    ax = width / 4;
    keypad(curwin, true);
    character = c;
}
void Player::drawControlls()
{
    wattron(main, COLOR_PAIR(12));
    mvwprintw(main, ay - ay / 4 - 1, ax, "  "); // up
    mvwprintw(main, ay - ay / 4, ax + 2, "  "); // right
    mvwprintw(main, ay - ay / 4 + 1, ax, "  "); // down
    mvwprintw(main, ay - ay / 4, ax - 2, "  "); // left
    mvwprintw(main, ay - ay / 4, ax * 3, "    ");
    mvwprintw(main, ay - ay / 4, ax * 3.8, "    ");
    wattroff(main, COLOR_PAIR(12));
    wrefresh(main);
}
void Player::mvleft()
{
    wattron(main, COLOR_PAIR(10));
    mvwprintw(main, ay - ay / 4, ax - 2, "  ");
    wattroff(main, COLOR_PAIR(10));
    wrefresh(main);
    x -= 1;
    if (x < 1)
        x = 1;
}

void Player::mvright()
{
    wattron(main, COLOR_PAIR(10));
    mvwprintw(main, ay - ay / 4, ax + 2, "  "); // right
    wattroff(main, COLOR_PAIR(10));
    wrefresh(main);
    x += 1;
    if (x > width - 1)
        x = width - 1;
    if (x == 10)
        gameGoing();
}
void Player::clearTrails()
{
    mvwaddch(curwin, y - 2, x, ' ');
    mvwaddch(curwin, y - 1, x, ' ');
    mvwaddch(curwin, y - 2, x + 1, ' ');
    mvwaddch(curwin, y - 2, x - 1, ' ');
    mvwaddch(curwin, y, x - 1, ' ');
    mvwaddch(curwin, y, x + 1, ' ');
}

int Player::getmv()
{
    int choice = wgetch(curwin);
    switch (choice)
    {
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_LEFT:
        drawControlls();
        clearTrails();
        mvleft();
        break;
    case KEY_RIGHT:
        drawControlls();
        clearTrails();
        mvright();
        break;
    default:
        break;
    }
    return choice;
}

void Player::display()
{
    mvwaddch(curwin, y - 2, x, 'O');
    mvwaddch(curwin, y - 1, x, character);
    mvwaddch(curwin, y - 2, x + 1, '/');
    mvwaddch(curwin, y - 2, x - 1, '\\');
    mvwaddch(curwin, y, x - 1, '/');
    mvwaddch(curwin, y, x + 1, '\\');
}
bool Player::gameGoing()
{
    if (x > 10)
    {
        return false;
    }
    return true;
}
#endif
