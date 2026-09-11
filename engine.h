#ifndef ENGINE_H
#define ENGINE_H

#include "glframework.h"
#include <base2dobject.h>
#include <base.h>
#include <logger.h>
#include <SDL.h>
#include "audio.h"
#include "sprite.h"
#include "costumtext.h"



enum GAMESTATE{
    Starting =0,        // Startupsound, spielstart
    StartUpFinished,    // Splash screen weg, Endlos  sound staten
    Run,
    RemoveFrog,
    FloatingRight,
    FloatingLeft,
    TimeOut,
    Plunk,              // ins wasser plumpsen
    Die,                // Autsch.... neuer Frosch
    Arrived,            // Im ziel
    GameOver,
    Paused              // Zum testen, später wieder löschen
};

const int MAX_TILE_X = 20;
const int MAX_TILE_Y = 15;

const int AUDIO_Channel_Background  = 0;
const int AUDIO_Channel_StartUp     = 5;
const int AUDIO_Channel_Hop         = 2;
const int AUDIO_Channel_Death       = 3;
const int AUDIO_Channel_Plunck      = 4;

const int TREES_PER_ROW             = 3;


static GAMESTATE GameState;

class TestEngine : public ENGINE::GLFrameWork
{
public:
    TestEngine(int resx,int resy);
    ~TestEngine();
    void Run() override;
    bool AddTextDisplay(int x, int y,int id, string name) override;
    bool AddTextDisplayWithBackground(int x, int y,int id,string name) override;

    void HandleMessage() override;

    bool UserUpdate(KEYBOARDSTATE state);
    bool InitUserObjects();
    void StartUp();

    bool LoadSurface(std::string path);


protected:
    CLOCK::GameClock clock;
    Logger log;
    ENGINE::Base* Frogger;
    std::vector<ENGINE::Base*> Holz;
    Audio * audio;

    // +++++++++++++++++++++++++++++++
    // unser hauptakteur: der Frosch
    // +++++++++++++++++++++++++++++++
    ENGINE::Sprite * frog;

    // -------------------------------
    // Enemies
    // -------------------------------
    ENGINE::Sprite * snake;
    ENGINE::Sprite * frogdeath;

    // +++++++++++++++++++++++++++++++
    // cars and snakes als tiles 64x64
    // 20 Stk pro x-achse
    // +++++++++++++++++++++++++++++++
    ENGINE::Sprite * StreetBlocksBottom[20];
    ENGINE::Sprite * StreetBlocksMiddle[20];

    ENGINE::Sprite * FrogZiel[5];

    // ----------------------
    // Holz, crocs und biber:
    // ----------------------
    ENGINE::Sprite* Baum_Row1[TREES_PER_ROW];

    // --------------------------
    // Splash Screen
    // --------------------------
    ENGINE::BaseObject2D * _SplashScreen;

    COSTUMTEXT::TextBase * _HighScore;
    COSTUMTEXT::TextBase * _Score;
    COSTUMTEXT::TextBase * _Time;

    Mix_Chunk* sound_Startup;
    Mix_Chunk* sound_Hop;
    Mix_Chunk* sound_FrogDeath;
    Mix_Chunk* sound_Plunk;         // Ins Wasser geplumpst..

    // Sounds .mp3
    Mix_Music* sound_Background;
    //Mix_Music* sound_StartUp;

    uint64_t _Elapsed;

private:

    // ----------------------------------------
    // X-Steps für cars, Bäume usw..
    // ----------------------------------------
    int Step_Trees;
    int Step_Snake;

    bool _Pause;
    GAMESTATE _Oldstate;  // zum restaurieren nach _Pause

    //static GAMESTATE _GameState;
    static void SoundHandler();
    void ChangeGameState(GAMESTATE state);
    int FrogInRow();// Die aktuelle "Zeile" vom Frosch...
    void GetNewState();

    void RenderBackgroundSprites();
    void RenderSplashScreen();
    void RenderWood();
    void RenderFrog();
    void RenderScore();

    void StartDieAnimation(int starttile, int endtile); // Der Frosch darf nich leiden..

    int _FrogCount;
    string keyboardtext;

    int _StepX;
    int _StepY;

    int _TileX;
    int _TileY;

    int _EndTileX;
    int _EndTileY;

    int SnakeX;
};

#endif // ENGINE_H

