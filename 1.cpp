#include <ncurses.h>
#include <chrono>
#include <unistd.h>
#include <iostream>
#include <cctype>
#include <fstream>
#include "NewtonGame.hpp"
#include "Handheld.hpp"
#include "Drawable.hpp"
#include "Projectile.hpp"
#include "Handheld.hpp"
// #include "Scores.hpp"

#define UNDEFINED 0
#define INGAME 1
#define GAMEOVER 2
#define GUIDE 3

bool isValidUsername(const std::string &name)
{
    for (char c : name)
    {
        if (!std::isalnum(c))
        { // check if the character is not alphanumeric
            return false;
        }
    }
    return true;
}
std::string getUsernameFromUser()
{
    std::cout << "Make sure this window is maximized!" << std::endl;
    std::cout << "Give your username" << std::endl;
    std::string name;
    char c;

    while ((c = std::cin.get()) != '\n')
    {
        if (std::isalnum(c))
        {
            name.push_back(c);
        }
        else
        {
            std::cout << "Invalid username. Use only letters and numbers." << std::endl;
            name.clear();
            while ((c = std::cin.get()) != '\n')
            {
            }
            break;
        }
    }

    while (name.empty())
    {
        std::cout << "Enter a valid username: ";
        while ((c = std::cin.get()) != '\n')
            if (std::isalnum(c))
            {
                name.push_back(c);
            }
            else
            {
                std::cout << "Invalid username. Use only letters and numbers." << std::endl;
                name.clear();
                while ((c = std::cin.get()) != '\n')
                    ;
                break;
            }
    }

    return name;
}
int main()
{
    std::string name = getUsernameFromUser();
    bool exit = false;
    initscr();
    NewtonGame game(name);
    game.drawG();
    std::chrono::steady_clock::time_point start_time;
    while (!exit)
    {
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
            game.clearGameScreen();
            game.gameOverScreen();
            game.redraw();
            napms(400);
            game.clearGameScreen();
            exit = true;
            game.setState(0);
        }
        if (game.getState() != 0)
            game.drawG();
    }
    game.exit();
    getch();

    return 0;
}