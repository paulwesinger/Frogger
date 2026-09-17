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

enum GAMELEVEL{
    Level_1 = 1,
    Level_2,
    Level_3,
    Level_4,
    Level_5    // usw.. todo: Levelnamen für jeden level
};

struct sFrogDestination{
    // borders
    int left;
    int top;
    int right;
    int bottom;
    sFrogDestination(){ left=0; top = 0; right=0;bottom=0;}
    sFrogDestination(int _left, int _top, int _right, int bottom) :  left(_left), top(_top), right(_right), bottom(bottom)  {}
};

struct sFrogArrived{
    sPoint position;
    bool arrived;
    bool haveTodie;

    sFrogArrived(){arrived = false; haveTodie = false;  position = sPoint(0,0);}
    sFrogArrived(sPoint _p, bool _arrived,bool _hastodie) : arrived(_arrived),position(_p), haveTodie(_hastodie){}
};

const int MAX_TILE_X = 20;
const int MAX_TILE_Y = 15;

const int AUDIO_Channel_Background  = 0;
const int AUDIO_Channel_StartUp     = 5;
const int AUDIO_Channel_Hop         = 2;
const int AUDIO_Channel_Death       = 3;
const int AUDIO_Channel_Plunck      = 4;
const int AUDIO_CHANNEL_EXTRA       = 5;

const int FLOATOBJECTS_PER_ROW_5            = 3;
const int FLOATOBJECTS_PER_ROW_4            = 6;
const int FLOATOBJECTS_PER_ROW_3            = 3;
const int FLOATOBJECTS_PER_ROW_2            = 4;
const int FLOATOBJECTS_PER_ROW_1            = 3;

const int TURTLES_ROW_2                     = 9;
const int TURTLES_ROW_4                     = 8;

// Vehicles
const int VEHICLES_PER_ROW_7                = 5;
const int VEHICLES_PER_ROW_8                = 6;
const int VEHICLES_PER_ROW_9                = 4;
const int VEHICLES_PER_ROW_10               = 5;
const int VEHICLES_PER_ROW_11               = 4;


// Ziel Bereiche:
const int FROG_DESTINATIONS                 = 5;
const int WALLS                             = 10;

const int TIMELEVEL_1 = 120;
const int TIMELEVEL_2 = 100;
const int TIMELEVEL_3 = 90;
const int TIMELEVEL_4 = 80;
const int TIMELEVEL_5 = 70;

const int TURTLE_DIVING_TILE                = 5;



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

    void _ResetTimeCounter(GAMELEVEL level);
    void _ResetScore();
    string _Score2String();
    void ResetGame();

    CLOCK::GameClock clock;
    Logger log;

    GAMELEVEL _GameLevel;

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

    sFrogArrived FrogArrivedDestinatons [FROG_DESTINATIONS] ;

    ENGINE::Sprite * FrogZiel[FROG_DESTINATIONS];
    ENGINE::Sprite * Walls[WALLS];

    // ----------------------
    // Holz, crocs und biber:
    // ----------------------
    ENGINE::Sprite* Baum_Row5[FLOATOBJECTS_PER_ROW_5];
    ENGINE::Sprite* Baum_Row3[FLOATOBJECTS_PER_ROW_3];    
    ENGINE::Sprite* Baum_Row1[FLOATOBJECTS_PER_ROW_1];

    // ----------------------
    // Schildkröten:
    // ----------------------
    ENGINE::Sprite* turtlesRow2[TURTLES_ROW_2];
    ENGINE::Sprite* turtlesRow4[TURTLES_ROW_4];

    // ----------------------
    // Fahrzeuge:
    // ----------------------
    ENGINE::Sprite* Vehicle_Row7[VEHICLES_PER_ROW_7];
    // Der 2.Teil des lkw...
     ENGINE::Sprite* Vehicle_Row7_2[VEHICLES_PER_ROW_7];

    ENGINE::Sprite* Vehicle_Row8[VEHICLES_PER_ROW_8];
    ENGINE::Sprite* Vehicle_Row9[VEHICLES_PER_ROW_9];
    ENGINE::Sprite* Vehicle_Row10[VEHICLES_PER_ROW_10];
    ENGINE::Sprite* Vehicle_Row11[VEHICLES_PER_ROW_11];


    // Der Frog ist im ziel
    ENGINE::Sprite* FrogArrived[VEHICLES_PER_ROW_7];

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
    Mix_Chunk* sound_Extra;

    // Sounds .mp3
    Mix_Music* sound_Background;
    //Mix_Music* sound_StartUp;

    uint64_t _Elapsed;

private:

    // ----------------------------------------
    // X-Steps für cars, Bäume usw..
    // ----------------------------------------
    int Step_Trees_1;
    int Step_Trees_3;
    int Step_Trees_5;

    int _StepXFrog;
    int _StepYFrog;

    int _StepXTurtlesRow2;
    int _StepXTurtlesRow4;

    int _StepVehicleRow11;
    int _StepVehicleRow10;
    int _StepVehicleRow9;
    int _StepVehicleRow8;
    int _StepVehicleRow7;


    int _gameScore;
    int _gameHighScore;
    int Step_Snake;

    int indexFrogArrived = 0;

    bool _Pause;
    bool _DontRunAgain;

    GAMESTATE _Oldstate;  // zum restaurieren nach _Pause

    static void SoundHandler();
    void ChangeGameState(GAMESTATE state);
    int FrogInRow();// Die aktuelle "Zeile" vom Frosch...
    void GetNewState();
    void InitTreeRows();
    void ReleaseTrees();
    void InitTextMap();
    void InitVehicles();
    void ReleaseVehicles();

    void RenderBackgroundSprites();
    void RenderSplashScreen();
    void RenderWood();
    void RenderFrog();
    void RenderScore();
    void RenderTurtles();
    void RenderVehicles();
    void RenderArrivedFrogs();
    int GetFloatingStep();


    void StartDieAnimation(int starttile, int endtile); // Der Frosch darf nich leiden..

    int _FrogCount;
    string keyboardtext;

    int _TileX;
    int _TileY;

    int _EndTileX;
    int _EndTileY;

    bool _AnimateReverse = false;

    int SnakeX;

    //TEst Countdown
    uint64_t countelapse = 0;
    int _TimeCounter = 100;
    std::string stCounter = "100";

};

#endif // ENGINE_H

