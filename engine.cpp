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

    for(int i=0; i< MAX_TILE_X; i++)
        delete StreetBlocksBottom[i];

    for(int i=0; i< MAX_TILE_X; i++)
        delete StreetBlocksMiddle[i];

     for (int i =0; i<5; i++)
        delete FrogZiel[i];

    // wav freigeben
    Mix_FreeChunk(sound_Hop);
    Mix_FreeChunk(sound_Startup);
    Mix_FreeChunk(sound_FrogDeath);

    // Mixer_Music freigeben
    Mix_FreeMusic(sound_Background);
    //  Mix_FreeMusic(sound_StartUp);
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
        //audio->PlaySound(sound_Hop);



        _StepX = 0; _StepY = -STEP_Y;
        _TileX = 1;
        _TileY = 0;
        _EndTileX = 0; _EndTileY = 0;

        if (frog->AnimationDone()) {
            frog->EndAnimation(_EndTileX,_EndTileY,Frogger_END_DELAY,_Elapsed);

            if (frog->EndAnimationDone())
                frog->StartAnimation(_TileX,_TileY,Frogger_TIME,Frogger_FRAMES,_StepX,_StepY);

            cout << "Animation started" << endl;
        }
        break;
    }

    case BTN_PRESS_DOWN_KEY:
        keyboardtext = "Down Key";
       // audio->PlaySound(sound_Hop);

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
       // audio->PlaySound(sound_Hop);

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
        //audio->PlaySound(sound_Hop);

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
            audio->PlaySound(sound_Hop);
            cout << "Animation ends" << endl;
            break;
        }
}

void TestEngine::Run(){
    glEnable(GL_DEPTH_TEST);


    LoadSurface("/home/paul/workspace/GLFrameWork/images/standard/errorAlpha.png");

    clock.Start();

    while (! _Quit) {

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

        int x = 0;
        for (int i = 0; i< MAX_TILE_X; i++){
            StreetBlocksBottom[i]->setPos(x,802);
            StreetBlocksBottom[i]->RenderFromAsset(8,0);

            StreetBlocksMiddle[i]->setPos(x,348);
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

        if ( ! frog->AnimationDone() ){

            frog->MoveSprite(_StepX,_StepY,Frogger_TIME,Frogger_FRAMES,_Elapsed,_TileX,_TileY);
        }
        else{

            frog->RenderFromAsset(_EndTileX,_EndTileY);
        }


        if (! snake->IsColliding(frog->Pos(),frog->Size())) {

            snake->MoveSprite(0,2,100,128,64,-4,0,_Elapsed);

        }
        else{
            cout << "Colliding !!!!!!!!!!!!!!" << std::endl;
            cout << "Size Frog " << frog->Size().w << "  " << frog ->Size().h << std::endl;
            // Frog death
            audio->PlaySound(sound_FrogDeath);

            //SDL_Delay(1500);
        }
        // std::cout << "Move sprite" << std::endl;
        // if ( snake->AnimationDone()){
        //     std::cout << "Animation Done" << std::endl;
        //     snake->EndAnimation(2,0,200,_Elapsed);

        //     if (snake->EndAnimationDone()) {
        //          std::cout << "En Animation done" << std::endl;
        //         snake->StartAnimation(0,0,900,3,16,0);
        //         snake->SetPosition(SnakeX,802);
        //         SnakeX += 5;
        //     }
        // }

        if (SnakeX > _ResX)
            SnakeX = 0;
        SwapWindow();
    }
}

bool TestEngine::InitUserObjects(){

    bool ret = true;

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
    frog->SetPosition(608,802);
    frog->InitTextureMap(8,4);
    frog->SetMoveArea(0,32,1280,876);


    // ---------------------------------------------
    // Snake
    // ---------------------------------------------
    snake = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Snakes3x1_128_64.png",_Shader);
    snake->InitTextureMap(3,1);
    snake->SetPosition(0,802);

    snake ->StartAnimation(0,0,3000,3,5,0);
    SnakeX = 0;

    // Die untere Strasse Rendern:
    // instancen für sprites in einer schleife generieren.
    for(int i = 0; i < MAX_TILE_X; i++){
        StreetBlocksBottom[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/images/retrogames/frogger/CarsAndSnakes64x64.png",_Shader);
        StreetBlocksBottom[i]->InitTextureMap(9,4);
    }
    // instancen in einem array of sprites neu anlegen

    // Die obere Strasse Rendern:
    // instancen für sprites in einer schleife generieren.
    for(int i = 0; i < MAX_TILE_X; i++){
        StreetBlocksMiddle[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/images/retrogames/frogger/CarsAndSnakes64x64.png",_Shader);
        StreetBlocksMiddle[i]->InitTextureMap(9,4);
    }

    // Frog Destination:
    x = 0;
    for(int i=0; i< 5; i++){
        FrogZiel[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/FrogZiel.png",_Shader);
        FrogZiel[i]->InitTextureMap(1,1);

        x +=150;
    }

    // instancen in einem array of sprites neu anlegen

    // Default settings at start
    _TileX = 0;
    _TileY = 0;
    _EndTileX = 0; _EndTileY = 0;
    audio = new Audio;

    // Background sound laden
    // audio->LoadMP3("/home/paul/workspace/sounds/retrogames/frogger/frogger.mp3",Sound_Background);

    // ------------------------
    // Wav's
    // ------------------------
    sound_Hop =  audio->LoadWavMixSound("/home/paul/workspace/sounds/retrogames/frogger/pickupCoin.wav");
    sound_Startup = audio->LoadWavMixSound("/home/paul/workspace/sounds/retrogames/frogger/downloaded/sound-frogger-coin-in/sound-frogger-coin-in.wav");
    sound_FrogDeath = audio->LoadWavMixSound("/home/paul/workspace/Frogger/sounds/frogDeath.wav");
    // -----------------------
    // Mp3's
    // -----------------------
    // hintergrundsound laden, etwas nervig aber witzig...!!
    sound_Background = audio->LoadBackgroundSound("/home/paul/workspace/sounds/retrogames/frogger/AudacityModiyfied/EndlessBackground.mp3");



    return ret;
}

void TestEngine::StartBackgroundSound(){
    audio->PlayBackrgoundSound(sound_Background);
}

void TestEngine::StartUp(){
    audio->PlaySound(sound_Startup);
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

