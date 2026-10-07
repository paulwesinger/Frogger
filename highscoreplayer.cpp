#include "highscoreplayer.h"

SCORE::HighScorePlayer::HighScorePlayer() {

    _PlayerName = "UNKNOW PLAYER";
    _Score      = 0;
    _Level      = 1;
    _Rank       = 1000;
}
SCORE::HighScorePlayer::HighScorePlayer(string name, int score, int level, int rank){
    _PlayerName = name;
    _Score      = score;
    _Level      = level;
    _Rank       = rank;
}

// -------------------------------------------------

void SCORE::HighScorePlayer::SetPlayerName(string name){
    _PlayerName = name;
}
string SCORE::HighScorePlayer::PlayerName(){
    return _PlayerName;
}

//---------------------------------------------------

void SCORE::HighScorePlayer::SetLevel(int level){
    _Level = level;
}
int SCORE::HighScorePlayer::Level(){
    return _Level;
}

// ---------------------------------------------------

void SCORE::HighScorePlayer::SetRank(int rank){
    _Rank = rank;
}
int SCORE::HighScorePlayer::Rank(){
    return _Rank;
}

// ---------------------------------------------------


void SCORE::HighScorePlayer::SetScore(int score){
    _Score = score;
}
int SCORE::HighScorePlayer::Score(){
    return _Score;
}
