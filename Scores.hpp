#include <iostream>
#include <fstream>

using namespace std;

struct Score
{
    string name;
    int score;
};

void updateScoreboard(Score newScore)
{
    ifstream scoreboardFile("scoreboard.txt");
    ofstream tempFile("temp.txt");
    bool found = false;

    if (scoreboardFile.is_open() && tempFile.is_open())
    {
        Score currentScore;

        while (scoreboardFile >> currentScore.name >> currentScore.score)
        {
            if (currentScore.name == newScore.name)
            {
                found = true;
                if (newScore.score > currentScore.score)
                {
                    tempFile << newScore.name << " " << newScore.score << endl;
                }
                else
                {
                    tempFile << currentScore.name << " " << currentScore.score << endl;
                }
            }
            else
            {
                tempFile << currentScore.name << " " << currentScore.score << endl;
            }
        }

        if (!found)
        {
            tempFile << newScore.name << " " << newScore.score << endl;
        }

        scoreboardFile.close();
        tempFile.close();

        remove("scoreboard.txt");
        rename("temp.txt", "scoreboard.txt");
    }
}

// int main()
// {
//     Score newScore;
//     newScore.name = game.getName();
//     newScore.score = game.getScore();
//     updateScoreboard(newScore);

//     return 0;
// }
