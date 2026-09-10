#include "engine.h"
#include <iostream>
#include <thread>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include <imageloader.h>


const uint64_t Frogger_TIME = 100;
const int Frogger_FRAMES = 5;
const uint64_t Frogger_END_DELAY = 100;

const int STEP_X = 64;
const int STEP_Y = 64;



TestEngine::TestEngine(int resx,int resy)
    :GLFrameWork(resx,resy)
{
    cout << "Ok" << endl;

    _Pause = false;
}

TestEngine::~TestEngine(){


    // if (! Holz.empty()) {
    //     for (ENGINE::Base *elem:Holz)
    //     {
    //         delete elem;
    //     }
    //     Holz.clear();
    // }

    delete frog;
    delete snake;
    delete frogdeath;

    for(int i=0; i< MAX_TILE_X; i++)
        delete StreetBlocksBottom[i];

    for(int i=0; i< MAX_TILE_X; i++)
        delete StreetBlocksMiddle[i];

     for (int i =0; i<5; i++)
        delete FrogZiel[i];

     for (int i =0; i<5; i++)
         delete Baum_Row1[i];


    // wav freigeben
    Mix_FreeChunk(sound_Hop);
    Mix_FreeChunk(sound_Startup);
    Mix_FreeChunk(sound_FrogDeath);

    // Mixer_Music freigeben
    Mix_FreeMusic(sound_Background);
}

bool TestEngine::LoadSurface(string path){

    ENGINE::ImageLoader imgloader;
    SDL_Surface* surf = imgloader.getSurface(path,"TestEngine::LoadImage");

    std::cout << "W x H von surf: " << surf->w  << " " << surf ->h << std::endl;

    if (surf != nullptr)
        return true;
    return false;
}

bool TestEngine::UserUpdate(KEYBOARDSTATE state){


    bool ret = false;
    switch(state.BtnStatePress){

    case  BTN_PRESS_UP_KEY:{
        keyboardtext = "Up Key";

        _StepX = 0; _StepY = -STEP_Y;
        _TileX = 1;
        _TileY = 0;
        _EndTileX = 0; _EndTileY = 0;

        if (frog->AnimationDone()) {
            frog->EndAnimation(_EndTileX,_EndTileY,Frogger_END_DELAY,_Elapsed);

            if (frog->EndAnimationDone())
                frog->StartAnimation(_TileX,_TileY,Frogger_TIME,Frogger_FRAMES,_StepX,_StepY);


        }
        break;
    }

    case BTN_PRESS_DOWN_KEY:
        keyboardtext = "Down Key"; 

        _StepX = 0; _StepY = STEP_Y;
        _TileX = 5;
        _TileY = 0;
        _EndTileX = 4; _EndTileY = 0;

        if (frog->AnimationDone()) {
            frog->EndAnimation(_EndTileX,_EndTileY,Frogger_END_DELAY,_Elapsed);

            if (frog->EndAnimationDone())
                frog->StartAnimation(_TileX,_TileY,Frogger_TIME,Frogger_FRAMES,_StepX,_StepY);

            cout << "Animation started" << endl;
        }
        break;

    case BTN_PRESS_LEFT_KEY:
        keyboardtext = "Left Key";  

        _StepX = -STEP_X; _StepY = 0;
        _TileX = 3;
        _TileY = 0;
        _EndTileX = 2; _EndTileY = 0;

        if (frog->AnimationDone()) {
            frog->EndAnimation(_EndTileX,_EndTileY,Frogger_END_DELAY,_Elapsed);

            if (frog->EndAnimationDone())
                frog->StartAnimation(_TileX,_TileY,Frogger_TIME,Frogger_FRAMES,_StepX,_StepY);            
        }
        break;

    case BTN_PRESS_RIGHT_KEY:
        keyboardtext = "Right Key";

        _StepX = STEP_X; _StepY = 0;
        _TileX = 7;
        _TileY = 0;
        _EndTileX = 6; _EndTileY = 0;

        if (frog->AnimationDone()) {
            frog->EndAnimation(_EndTileX,_EndTileY,Frogger_END_DELAY,_Elapsed);

            if (frog->EndAnimationDone())
                frog->StartAnimation(_TileX,_TileY,Frogger_TIME,Frogger_FRAMES,_StepX,_StepY);

            cout << "Animation started" << endl;
        }
        break;

    case BTN_PRESS_SPACE_KEY:
        keyboardtext = "Space Key";
        break;
    default:
        keyboardtext = "No Key";
        _StepX = 0;
        _StepY = 0;
        break;
    }

    return ret;
}

void TestEngine::HandleMessage(){
    GLFrameWork::HandleMessage();

    switch(KeyboardState.BtnStateUP){
        case BTN_UP_DOWN_KEY:
        case BTN_UP_UP_KEY:
        case BTN_UP_LEFT_KEY:
        case BTN_UP_RIGHT_KEY:
            frog->EndAnimation(_TileX,_TileY,Frogger_END_DELAY,_Elapsed);
            audio->PlaySound(sound_Hop,AUDIO_Channel_Hop);
            cout << "Animation ends" << endl;
            break;


        case BTN_UP_SPACE_KEY:
        if (_Pause) {
            GameState = _Oldstate; // State wieder herstellen

        }
        else{
            _Oldstate = GameState;
            GameState = GAMESTATE::Paused;
        }
        _Pause = ! _Pause;  // Toggle the Pause state
        break;
    }
}

int TestEngine::FrogInRow(){
    // anm.: 34 ist die obere Begrenzung des Bewegungsbereichs, 64 die Höhe des FroschSprite,
    //    ergibt eine ganzzahl zwischen 0 und 11
    return (frog->PosY() - 34) / 64;
}

void TestEngine::RenderBackgroundSprites(){
    int x = 0;
    for (int i = 0; i< MAX_TILE_X; i++){
        StreetBlocksBottom[i]->setPos(x,802);
        StreetBlocksBottom[i]->RenderFromAsset(8,0);

        StreetBlocksMiddle[i]->setPos(x,418);
        StreetBlocksMiddle[i]->RenderFromAsset(8,0);
        x += 64;
    }

    x=0;
    for(int i=0; i< 4; i++){
        FrogZiel[i]->setPos(x,0);
        FrogZiel[i]->RenderFromAsset(0,0);
        x +=270;
    }



    FrogZiel[4]->setPos(1152,0);
    FrogZiel[4]->RenderFromAsset(0,0);
}

void TestEngine::RenderWood(){

    bool tmp;
    for (int i =0; i < 5;i++){
        // Erstmal alles rendern
        Baum_Row1[i]->MoveSprite(0,0,128,64,100,Step_Trees,0,_Elapsed,tmp);
    }



}
void TestEngine::RenderFrog(){
    if ( ! frog->AnimationDone() ){
        // Warten auf Animationsende
        frog->MoveSprite(_StepX,_StepY,Frogger_TIME,Frogger_FRAMES,_Elapsed,_TileX,_TileY);
    }
    else{

        GetNewState();

        if (GameState == GAMESTATE::FloatingRight)    {
            // checken, ob von links nach rechts oder umgekehrt,
            // zum testen von links nach rechts...
            sPoint p = frog->Pos();            
            p.x += Step_Trees;
            frog->SetPosition(p.x,p.y);
        }
        else
            if (GameState == GAMESTATE::FloatingLeft) {
                sPoint p = frog->Pos();
                p.x -= Step_Trees;
                frog->SetPosition(p.x,p.y);
            }


        // if (frog->PosY() < 350)
        //     GameState = GAMESTATE::Plunk;

        frog->RenderFromAsset(_EndTileX,_EndTileY);
    }
}

void TestEngine::GetNewState(){

    int frogrow = FrogInRow();

    switch(frogrow)
    {
    case 0:
        GameState = GAMESTATE::Arrived;

        break;
    case 1:
        GameState = GAMESTATE::FloatingRight;
        break ;
    case 2:
        GameState = GAMESTATE::FloatingLeft;
        break;
    case 3:
        GameState = GAMESTATE::FloatingRight;
        break;
    case 4:
        GameState = GAMESTATE::FloatingLeft;

        break;

    case 5:{
            bool plunk = true;
            for (int i =0; i< 3; i++) {
                if (Baum_Row1[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingRight;
                    plunk = false;
                }
            }
            if (plunk)
                GameState = GAMESTATE::Plunk;
            break;
        }
    case 6:
        // Back on the street:
        GameState = GAMESTATE::Run;
        break;
    case 7:


        break;
    case 8: break;
    case 9:
        break;

    case 10:
        break;
    case 11:
        break;
    default:
        break;

    }
}


void TestEngine::Run(){
    glEnable(GL_DEPTH_TEST);
    //LoadSurface("/home/paul/workspace/GLFrameWork/images/standard/errorAlpha.png");
    clock.Start();


    GameState = GAMESTATE::Starting;

    while (! _Quit) {

        StartUp();

        while (_FrogCount > 0  && ! _Quit){

            _Elapsed = clock.Elapsed(); //CLOCK::GameClock::Elapsed();
            //   cout << "Elapsed: " << elapsed  << endl;
            HandleMessage();

            KEYBOARDSTATE state =  KeyState();
            UserUpdate(state);// Nur mal testen

            //        std::this_thread::sleep_for(std::chrono::milliseconds(50));
            // cout << "Key pressed " << keyboardtext <<  endl;
            glDepthFunc(GL_LEQUAL);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor( _ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

            // +++++++++++++++++++++++++++++++++++
            // Rendering 3D
            // +++++++++++++++++++++++++++++++++++
            Restore3D();

            // +++++++++++++++++++++++++++++++++++
            // Rendering 2D
            // +++++++++++++++++++++++++++++++++++
            Prepare2D();

            static string displayout;
            displayout = clock.FPSasString(_Elapsed,displayout);

            // _Displays.at(0)->SetText(displayout,0);
            // _Displays.at(1)->SetText(keyboardtext,0);

            // for(ENGINE::RenderText* elems:_Displays)
            //     elems->Draw();

            // --------------------------------------
            // Hintergrund und mauern immer rendern..
            // --------------------------------------

            // Das ganze mal mit den states:


            switch (GameState){

                case GAMESTATE::Paused:
                    frog->RenderFromAsset(_EndTileX,_EndTileY);
                    Baum_Row1[0]->RenderFromAsset(0,0);
                break;

                case GAMESTATE::Starting:

                    RenderSplashScreen();

                    if (audio->PlaySoundFinished(AUDIO_Channel_StartUp))
                        GameState = GAMESTATE::StartUpFinished;
                    break;

                case GAMESTATE::StartUpFinished:
                    RenderBackgroundSprites();                  
                    audio->PlayBackrgoundSound(sound_Background,1);
                    frog->SetPosition(608,802);

                    GameState= GAMESTATE::Run;
                    break;
                case GAMESTATE::Run:

                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                 //   GetNewState();

                    if (! snake->IsColliding(frog->Pos(),frog->Size())) {

                        bool tmp;
                        snake->MoveSprite(0,2,100,128,64,Step_Snake,0,_Elapsed,tmp);
                    }
                    else
                    {
                        GameState  = GAMESTATE::Die;
                        frogdeath->StartAnimation(0,6);
                        frogdeath->SetPosition(frog->PosX(),frog->PosY());

                        frogdeath->setRenderSprite(true);
                        frog->setRenderSprite(false);

                        audio->HaltMusic();
                        audio->ChannelToListen(AUDIO_Channel_Death);
                        audio->PlaySound(sound_FrogDeath,AUDIO_Channel_Death);
                    }

                    break;
                case GAMESTATE::FloatingRight:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
               //     GetNewState();
                    break;

                case GAMESTATE::FloatingLeft:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                //    GetNewState();
                    break;

                case GAMESTATE::TimeOut:
                    RenderBackgroundSprites();
                    break;
                case GAMESTATE::Plunk:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                    audio->ChannelToListen(AUDIO_Channel_Plunck);
                    audio->PlaySound(sound_Plunk,AUDIO_Channel_Plunck);

                    if (audio->PlaySoundFinished(AUDIO_Channel_Plunck)){
                        GameState = GAMESTATE::RemoveFrog;
                        //frogdeath->StartAnimation(0,6);
                        //frogdeath->SetPosition(frog->PosX(),frog->PosY());
                    }
                    break;
                case GAMESTATE::Die:

                    RenderBackgroundSprites();
                    RenderWood();
                    // Frog death
                    bool animdone;
                    frogdeath->MoveSprite(0,6,64,64,200,0,0,_Elapsed,animdone);

                    if (frogdeath->AnimationDone()){
                        GameState = GAMESTATE::RemoveFrog;
                        frogdeath->setRenderSprite(false);
                        frog->setRenderSprite(true);
                        snake->SetPosition(_ResX,802);
                    }

                    break;

                case GAMESTATE::RemoveFrog:
                    RenderBackgroundSprites();
                    RenderWood();
                    _FrogCount --;
                    GameState = GAMESTATE::StartUpFinished;
                    cout << "Frösche " << _FrogCount << endl;

                    if (_FrogCount == 0)
                        GameState = GAMESTATE::GameOver;
                    break;
                case GAMESTATE::Arrived:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                    break;

                case GAMESTATE::GameOver:
                    audio->HaltMusic();

                    // Abspann anzeigen
                    // Score


                    RenderSplashScreen();
                    SDL_Delay(4000);


                    break;
            }

            if (SnakeX > _ResX)
                SnakeX = 0;
            SwapWindow();
        }
    }
}

void TestEngine::RenderSplashScreen(){    
    _SplashScreen -> Render();
}

bool TestEngine::InitUserObjects(){

    bool ret = true;

    // ----------------------------------------------------------------------
    // Step init, bei jedem höheren level erhöhen, erhöht die geschwindigkeit
    // ----------------------------------------------------------------------
    Step_Trees = 4;
    Step_Snake = -4;  // Right to Left...



    if (AddTextDisplayWithBackground(100,100,0,"FPS Display with background")){

        _Displays[_Displays.size() -1]->SetText("Neues Display");
    }
    else{
        // Keine Textanzeige, aber wir können weitermachen -> ret bleibt true
        log.logwarn("Noch keine Displays erstellt","TestEngine");
    }


    if (AddTextDisplayWithBackground(100,200,1,"Keyboard Display with background")){

        _Displays[_Displays.size() -1]->SetText("Neues keyboard display");
    }
    else{
        log.logwarn("Noch keine Displays erstellt","TestEngine");
    }

    int x = 0;
    // for (int i =0; i<5; i++){
    //     ENGINE::Base* obj = new ENGINE::BaseObject2D(_ResX,_ResY,"/home/paul/workspace/GLFrameWork/images/Holztexture/Wood.png",_Shader);

    //     obj->setPos(x,400);
    //     obj->setSize(150,40);
    //     x+=200;
    //     Holz.push_back(obj);
    // }

    frog = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/images/retrogames/frogger/Froggs8x4.png",_Shader);
    // Für Auflösung 1280x960 Für 64 pixel tiles
    //frog->SetPosition(608,802);
    frog->InitTextureMap(8,4);
    frog->SetMoveArea(0,32,1280,876);


    // ---------------------------------------------
    // Snake
    // ---------------------------------------------
    snake = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Snakes3x1_128_64.png",_Shader);
    snake->InitTextureMap(3,1);
    snake->SetPosition(_ResX,802);

    snake ->StartAnimation(0,0,3000,3,5,0);
    SnakeX = 0;

    // ---------------------------------------------
    // frog - death
    // ---------------------------------------------
    frogdeath = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/frogdeath7x1_64x64.png",_Shader);
    frogdeath->InitTextureMap(7,1);

    // Die untere Strasse Rendern:
    // instancen für sprites in einer schleife generieren.
    for(int i = 0; i < MAX_TILE_X; i++){
        StreetBlocksBottom[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/CarsAndSnakes64x64.png",_Shader);
        StreetBlocksBottom[i]->InitTextureMap(9,4);
    }
    // instancen in einem array of sprites neu anlegen

    // Die obere Strasse Rendern:
    // instancen für sprites in einer schleife generieren.
    for(int i = 0; i < MAX_TILE_X; i++){
        StreetBlocksMiddle[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/CarsAndSnakes64x64.png",_Shader);
        StreetBlocksMiddle[i]->InitTextureMap(9,4);
    }

    // Frog Destination:
    x = 0;
    for(int i=0; i< 5; i++){
        FrogZiel[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/FrogZiel.png",_Shader);
        FrogZiel[i]->InitTextureMap(1,1);

        x +=150;
    }

    // -------------------------
    // Bäume Row1
    // -------------------------

    x = 0;
    for (int i =0; i < 5;i++){
        Baum_Row1[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum192x64.png",_Shader);
        Baum_Row1[i]->InitTextureMap(1,1);
        Baum_Row1[i]->SetPosition(x,354);

        Baum_Row1[i]->StartAnimation(0,0);

        x+= 280;
    }




    // Default settings at start
    _TileX = 0;
    _TileY = 0;
    _EndTileX = 0; _EndTileY = 0;

    // _SplashScreen
    _SplashScreen = new ENGINE::BaseObject2D(_ResX,_ResY,"/home/paul/workspace/Frogger/images/frogsplash2.png",_Shader);
    _SplashScreen->setPos(200,200);


    audio = new Audio;

    audio->AddHandlder(SoundHandler,AUDIO_Channel_StartUp);

    // Background sound laden
    // audio->LoadMP3("/home/paul/workspace/sounds/retrogames/frogger/frogger.mp3",Sound_Background);

    // ------------------------
    // Wav's
    // ------------------------
    sound_Hop =  audio->LoadWavMixSound("/home/paul/workspace/sounds/retrogames/frogger/pickupCoin.wav");
    sound_Startup = audio->LoadWavMixSound("/home/paul/workspace/sounds/retrogames/frogger/downloaded/sound-frogger-coin-in/sound-frogger-coin-in.wav");
    sound_FrogDeath = audio->LoadWavMixSound("/home/paul/workspace/Frogger/sounds/frogDeath.wav");
    sound_Plunk = audio->LoadWavMixSound("/home/paul/workspace/Frogger/sounds/plunk.wav");

    // --------------
    // Change Volume:
    // --------------
    audio->ChunkVolume(sound_Hop,80);
    audio->ChunkVolume(sound_Startup,100);
    audio->ChunkVolume(sound_FrogDeath,80);
    audio->ChunkVolume(sound_Plunk,80);

    // -----------------------
    // Mp3's
    // -----------------------
    // hintergrundsound laden, etwas nervig aber witzig...!!
    sound_Background = audio->LoadBackgroundSound("/home/paul/workspace/sounds/retrogames/frogger/AudacityModiyfied/EndlessBackground.mp3");
    audio->MusicVolume(32);


    return ret;
}

void TestEngine::SoundHandler(){
    //Change Gamestate after startup sound
    //GameState = GAMESTATE::StartUpFinished;
}

void TestEngine::StartUp(){
    audio->ChannelToListen(AUDIO_Channel_StartUp);
    audio->PlaySound(sound_Startup,AUDIO_Channel_StartUp);
    _FrogCount = 3;
    GameState = GAMESTATE::Starting;

    // Splash screen usw anzeigen
}

bool TestEngine::AddTextDisplayWithBackground(int x, int y,int id,string name) {

    if (GLFrameWork::AddTextDisplayWithBackground(x,y,id,name)) {
        log.loginfo("Display with Background erstellt","AddTextDisplayWithBackground");
        return true;
    }
    return false;
}

bool TestEngine::AddTextDisplay(int x, int y, int id, string name){

    if (GLFrameWork::AddTextDisplay(x,y,id,name)) {
        log.loginfo("Display erstellt","TestEngine");
        return true;
    }

    return false;
}

