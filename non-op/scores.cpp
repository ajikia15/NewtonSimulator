#include <iostream>
#include <fstream>

using namespace std;

struct Score
{
    string name;
    int score;
};

bool updateScoreboard(Score newScore)
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

        return true;
    }
    else
    {
        cerr << "Error opening files" << endl;
        return false;
    }
}

int main()
{
    Score newScore;
    cout << "Enter name: ";
    cin >> newScore.name;
    cout << "Enter score: ";
    cin >> newScore.score;

    if (updateScoreboard(newScore))
    {
        cout << "Scoreboard updated" << endl;
    }
    else
    {
        cout << "Error updating scoreboard" << endl;
    }
    return 0;
}
