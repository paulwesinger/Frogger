#ifndef ENGINE_H
#define ENGINE_H

#include "glframework.h"
#include <base2dobject.h>
#include <base.h>
#include <logger.h>
#include <SDL.h>
#include <random>

#include "audio.h"
#include "sprite.h"
#include "costumtext.h"
#include "levelconfig.h"
#include "highscoreplayer.h"




enum GAMESTATE{
    Starting =0,        // Startupsound, spielstart
    StartUpFinished,    // Splash screen weg, Endlos  sound staten
    InsertCoins,        // 1 oder 2 € == 3 oder 7 Froggis
    Loop,
    Run,
    RemoveFrog,
    FloatingRight,
    FloatingLeft,
    TimeOut,
    Plunk,              // ins wasser plumpsen
    Die,                // Autsch.... neuer Frosch
    Arrived,            // Im ziel
    GameOver,
    StageCleared,
    LevelUp,            // Nächster Level
    ShowTheBest,        // BestenListe anzeigen
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
const int AUDIO_Channel_StartUp     = 1;
const int AUDIO_Channel_Hop         = 2;
const int AUDIO_Channel_Death       = 3;
const int AUDIO_Channel_Plunck      = 4;
const int AUDIO_CHANNEL_EXTRA       = 5;
const int AUDIO_CHANNEL_GameOver    = 6;
const int AUDIO_CHANNEL_RaceCar     = 7;

const int AUDIO_CHANNEL_FrogHomed   = 8;
const int AUDIO_CHANNEL_StageCleared= 9;
const int AUDIO_CHANNEL_HurryUp     = 10;

const int FLOATOBJECTS_PER_ROW_5            = 3;
const int FLOATOBJECTS_PER_ROW_4            = 6;
const int FLOATOBJECTS_PER_ROW_3            = 4;
const int FLOATOBJECTS_PER_ROW_2            = 4;
const int FLOATOBJECTS_PER_ROW_1            = 3;

const int TURTLES_ROW_2                     = 9;
const int TURTLES_ROW_4                     = 8;

// Vehicles
const int VEHICLES_PER_ROW_7                = 5;
const int VEHICLES_PER_ROW_8                = 6;
const int VEHICLES_PER_ROW_9                = 2;
const int VEHICLES_PER_ROW_10               = 5;
const int VEHICLES_PER_ROW_11               = 4;


// Ziel Bereiche:
const int FROG_DESTINATIONS                 = 5;
const int WALLS                             = 10;

const int TIMELEVEL_1 = 60;
const int TIMELEVEL_2 = 50;
const int TIMELEVEL_3 = 40;
const int TIMELEVEL_4 = 35;
const int TIMELEVEL_5 = 30;
const int TIMELEVEL_6 = 25;

const int TURTLE_DIVING_TILE                = 5;
const int TIME_RUNS_OUT                     = 10;
const std::array<std::string,6> LEVEL_NAMES = {
    {"FUNKY BABY FROG"   ,
    "CRAZY OCHSENFROSCH",
    "BOUNCING PETER"    ,
    "HERO OF THE DAY"   ,
    "OUTSTANDING FROGGI",
     "RAMBO "}
};


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

    CONFIG::LevelConfig _LevelConfig;

    void _ResetTimeCounter(GAMELEVEL level);
    void _ResetScore();
    string _Score2String();
    void ResetGame();
    // "Insert Coin, continue,...usw"
    void RenderGameOverScreen();
    void RenderNextLevelScreen(int level);

    void UpdateTime();
    void RenderAvailableFrogs();
    int RemoveFrogFromList();

    int RandomGenerator();

    // --------------------------------------------
    // Für den nächsten Level wieder alle freigeben
    // --------------------------------------------
    void ClearArrivedFrogs();

    //static Uint32 _TimerCallback(Uint32,void*);

   // static FP _TimerCallback();
    // ID's für TimerSnapshots:
    SDL_TimerID TimerID_Snake;
    SDL_TimerID TimerID_Croc;

    sPoint _SnakePos;

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
    // Croco
    ENGINE::Sprite* Croco_Row_3;

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

    std::vector<ENGINE::Sprite*> _AvailableFrogs; // 3 bzw 5 Frogis, deshalb kein Array


    // Der Frog ist im ziel
    ENGINE::Sprite* FrogArrived[VEHICLES_PER_ROW_7];

    // --------------------------
    // Splash Screen
    // --------------------------
    ENGINE::BaseObject2D * _StartScreen;
    ENGINE::BaseObject2D * _GameOverScreen;
    ENGINE::BaseObject2D * _LevelBackground;
    ENGINE::BaseObject2D * _BestPlayersBG;

    COSTUMTEXT::TextBase * _HighScore;
    COSTUMTEXT::TextBase * _Score;
    COSTUMTEXT::TextBase * _Time;
    COSTUMTEXT::TextBase * _GameOver;
    COSTUMTEXT::TextBase * _LevelText;
    COSTUMTEXT::TextBase * _BestFive;

    // ---------------------------
    // GameOver screen
    // ---------------------------
    COSTUMTEXT::TextBase * _InsertCoin_1EU;   // 3 Froggis
    COSTUMTEXT::TextBase * _InsertCoin_2EU;   // 5 Froggis

    Mix_Chunk* sound_Startup;
    Mix_Chunk* sound_Hop;
    Mix_Chunk* sound_FrogDeath;
    Mix_Chunk* sound_Plunk;         // Ins Wasser geplumpst..
    Mix_Chunk* sound_Extra;
    Mix_Chunk* sound_GameOver;
    Mix_Chunk* sound_Racer;
    Mix_Chunk* sound_FrogHomed;
    Mix_Chunk* sound_StageCleared;
    Mix_Chunk* sound_HurryUp;

    // Music für hintergrund
    Mix_Music* sound_MainTheme;
    Mix_Music* sound_Start;


    uint64_t _Elapsed;
    uint64_t _TimerGameOver;

private:

    // ----------------------------------------
    // X-Steps für cars, Bäume usw..
    // ----------------------------------------
    int _Step_Trees_1;
    int _Step_Trees_3;
    int _Step_Trees_5;

    int _StepXTurtlesRow2;
    int _StepXTurtlesRow4;

    int _StepVehicleRow11;
    int _StepVehicleRow10;
    int _StepVehicleRow9;
    int _StepVehicleRow8;
    int _StepVehicleRow7;

    int _StepXFrog;
    int _StepYFrog;

    int _Step_Snake;

    int _gameScore;
    int _gameHighScore;


    int _IndexFrogArrived = 0;

    bool _Pause;
    bool _DontRunAgain;
    bool _StartNewGame;
    bool _TimesIsOut = false;
    bool _Randomrunning = false;

    GAMESTATE _Oldstate;  // zum restaurieren nach _Pause

    int _FrogCount;
    string keyboardtext;

    int _TileX;
    int _TileY;

    int _EndTileX;
    int _EndTileY;

    bool _AnimateReverse = false;
    bool _UpdateTime = true;

    uint64_t countelapse = 0;
    int _TimeCounter = 0;
    std::string stCounter = "60";

    std::vector<SCORE::HighScorePlayer*> _BestPlayers;
    SCORE::HighScorePlayer* _BestPlayer;


    static void SoundHandler();
    void ChangeGameState(GAMESTATE state);
    int FrogInRow();// Die aktuelle "Zeile" vom Frosch...
    void GetNewState();
    void InitTreeRows();
    void ReleaseTrees();
    void InitTextMap();
    void InitVehicles();
    void InitCroco();
    void InitAudio();
    void InitGameOverScreen();
    void InitLevels();

    // den Score des 5. platzes ermitteln,
    // ob der aktuelle spieler überhaupt in die bestenliste aufgenommen wird
    int FifthScore_ofThe_Best();
    void AddPlayerToHighScoreList(SCORE::HighScorePlayer * player);

    void ReleaseVehicles();

    void RenderBackgroundSprites();
    void RenderStartScreen();
    void RenderGameoverScreen();
    void RenderBestPlayers();
    void RenderWood();
    void RenderFrog();
    void RenderScore();
    void RenderHighScore();
    void RenderTime();
    void RenderTurtles();
    void RenderVehicles();
    void RenderArrivedFrogs();
    void RenderCrocos();
    int GetFloatingStep();
    void StartDieAnimation(int starttile, int endtile); // Der Frosch darf nich leiden..
};

#endif // ENGINE_H

