#include <ncurses.h>
#include <chrono>
#include <unistd.h>
#include <iostream>
#include "NewtonGame.hpp"
#include "Handheld.hpp"
#include "Drawable.hpp"
#include "Apple.hpp"
#include "Handheld.hpp"
int main()
{
    bool exit = false;
    initscr();
    NewtonGame game;
    std::string name;
    game.drawG();
    std::chrono::steady_clock::time_point start_time;
    while (!exit)
    {
        std::cin >> name;
        chtype inp = getch();
        switch (inp)
        {
        case 'x':
            game.exit();
            break;
        case 10:
            game.gameEnter();
            break;
        default:
            break;
        }
        if (game.getState() == 1)
        {
            start_time = std::chrono::steady_clock::now();
            do
            {
                auto current_time = std::chrono::steady_clock::now();
                auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>((current_time - start_time)).count();

                game.processInput();
                game.updateState(elapsed_time);
                game.redraw();
                usleep(800);
            } while (game.getState() == 1);
        }
        while (game.getState() == 2)
        {
            // gameOver();
        }
        game.drawG();
    }
    getch();
    endwin();

    return 0;
}