#pragma once
#include <iostream>
#include <fstream>

struct Score
{
    std::string name;
    int score;
};
bool compareScores(const Score &lhs, const Score &rhs)
{
    if (lhs.score == rhs.score)
    {
        return lhs.name < rhs.name;
    }
    return lhs.score > rhs.score;
}
void updateScoreboard(Score newScore)
{
    std::ifstream scoreboardFile("scoreboard.txt");
    std::vector<Score> scores;
    bool found = false;

    if (scoreboardFile.is_open())
    {
        Score currentScore;

        while (scoreboardFile >> currentScore.name >> currentScore.score)
        {
            if (currentScore.name == newScore.name)
            {
                found = true;
                if (newScore.score > currentScore.score)
                {
                    scores.push_back(newScore);
                }
                else
                {
                    scores.push_back(currentScore);
                }
            }
            else
            {
                scores.push_back(currentScore);
            }
        }

        if (!found)
        {
            scores.push_back(newScore);
        }

        scoreboardFile.close();

        sort(scores.begin(), scores.end(), compareScores);

        std::ofstream tempFile("temp.txt");
        for (const auto &score : scores)
        {
            tempFile << score.name << " " << score.score << std::endl;
        }
        tempFile.close();

        remove("scoreboard.txt");
        rename("temp.txt", "scoreboard.txt");
    }
}
