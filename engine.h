#ifndef ENGINE_H
#define ENGINE_H

#include "glframework.h"
#include <base2dobject.h>
#include <base.h>
#include <logger.h>
#include <SDL.h>
#include "audio.h"
#include "sprite.h"




enum GAMESTATE{
    Starting =0,        // Startupsound, spielstart
    StartUpFinished,    // Splash screen weg, Endlos  sound staten
    Run,
    RemoveFrog,
    Floating,
    TimeOut,
    Plunk,              // ins wasser plumpsen
    Die,                // Autsch.... neuer Frosch
    Arrived,             // Im ziel
    GameOver
};

const int MAX_TILE_X = 20;
const int MAX_TILE_Y = 15;

const int AUDIO_Channel_Background  = 0;
const int AUDIO_Channel_StartUp     = 1;
const int AUDIO_Channel_Hop         = 2;
const int AUDIO_Channel_Death       = 3;
const int AUDIO_Channel_Plunck      = 4;


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
    ENGINE::Sprite * FrogSplash;

    ENGINE::BaseObject2D * window;



    // sounds .wav
    // Wav8Bit Wav_Startup;
    // Wav8Bit Wav_Hop;
    // Wav8Bit Wav_Plunk;
    // Wav8Bit Wav_Squash;


    Mix_Chunk* sound_Startup;
    Mix_Chunk* sound_Hop;
    Mix_Chunk* sound_FrogDeath;
    Mix_Chunk* sound_Plunk;         // Ins Wasser geplumpst..

    // Sounds .mp3
    Mix_Music* sound_Background;
    //Mix_Music* sound_StartUp;

    uint64_t _Elapsed;

private:

    //static GAMESTATE _GameState;
    static void SoundHandler();
    void ChangeGameState(GAMESTATE state);

    void RenderBackgroundSprites();
    void RenderSplashScreen();

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

