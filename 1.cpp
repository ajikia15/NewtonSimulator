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
#include "Scores.hpp"

#define UNDEFINED 0
#define INGAME game.getState() == 1
#define GAMEOVER game.getState() == 2
#define GUIDE game.getState() == 3

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
void initScoreBoard(std::string n, int s)
{
    Score newScore;
    newScore.name = n;
    newScore.score = s;
    updateScoreboard(newScore);
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
        case 'q':
            game.setState(3);
        default:
            break;
        }
        if (INGAME)
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
            } while (INGAME);
        }
        while (GAMEOVER)
        {
            initScoreBoard(game.getName(), game.getScore());
            game.gameOverScreen();
            exit = true;
            napms(5000);
        }
        while (GUIDE)
        {

            game.gameGuideScreen();
            if (chtype c = getch())
            {
                game.clearGameScreen();
                break;
            }
        }
        if (game.getState() != 0)
            game.drawG();
    }
    game.exit();
    getch();

    return 0;
}