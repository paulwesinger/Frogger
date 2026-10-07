#ifndef HIGHSCOREPLAYER_H
#define HIGHSCOREPLAYER_H


#include <string>
#include <iostream>
#include <fstream>
using namespace  std;

namespace  SCORE {


class HighScorePlayer
{
public:
    HighScorePlayer();
    HighScorePlayer(string name,int score,int level, int rank);

    void SetPlayerName(string name);
    void SetScore(int score);
    void SetLevel(int level);
    void SetRank(int rank);

    string PlayerName();
    int Score();
    int Level();
    int Rank();


protected:
    string _PlayerName;
    int _Score;
    int _Level;
    int _Rank;
};

}  // namespace score
#endif // HIGHSCOREPLAYER_H
