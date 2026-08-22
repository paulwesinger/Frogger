#include "engine.h"
#include <iostream>
#include <thread>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include <imageloader.h>


const uint64_t Frogger_TIME = 150;



TestEngine::TestEngine(int resx,int resy)
    :GLFrameWork(resx,resy)
{
    cout << "Ok" << endl;
}

TestEngine::~TestEngine(){


    if (! Holz.empty()) {
        for (ENGINE::Base *elem:Holz)
        {
            delete elem;
        }
        Holz.clear();
    }

    delete frog;
    delete Frogger;

    // wav freigeben
    Mix_FreeChunk(sound_Hop);
    Mix_FreeChunk(sound_Startup);

    // Mixer_Music freigeben
    Mix_FreeMusic(sound_Background);
    //  Mix_FreeMusic(sound_StartUp);
}

// void TestEngine::GetKey(){
//     // switch (KeyboardState.BtnStatePress){
//     // case BTN_PRESS_LEFT_KEY:keyboardtext = "Left pressed"; cout << "Left pressed " << endl; break;
//     // case BTN_PRESS_RIGHT_KEY:keyboardtext = "Right pressed"; cout << "Right pressed " << endl; break;
//     // case BTN_PRESS_UP_KEY:keyboardtext = "up pressed"; break;
//     // case BTN_PRESS_DOWN_KEY:keyboardtext = "Down pressed"; break;
//     // case BTN_PRESS_SPACE_KEY: keyboardtext = "Space pressed"; break;
//     // case BTN_PRESS_NO: keyboardtext = "Nix"; break;
//     // default:
//     //     keyboardtext = "default nix";
//     // }
// }

bool TestEngine::LoadSurface(string path){

    ENGINE::ImageLoader imgloader;
    SDL_Surface* surf = imgloader.getSurface(path,"TestEngine::LoadImage");

    std::cout << "W x H von surf: " << surf->w  << " " << surf ->h << std::endl;

    if (surf != nullptr)
        return true;
    return false;
}

void TestEngine::UserUpdate(KEYBOARDSTATE state){

    switch(state.BtnStatePress){

    case  BTN_PRESS_UP_KEY:{
        keyboardtext = "Up Key";
        audio->PlaySound(sound_Hop);

        _StepX = 0; _StepY = -32;
        _TileX = 1;
        _TileY = 0;
        _EndTileX = 0; _EndTileY = 0;
        frog->StartAnimation(_TileX,_TileY,Frogger_TIME,1,_StepX,_StepY);
        //frog->MoveSprite(_StepX,_StepY,Frogger_TIME,2,_Elapsed,_TileX,_TileY);
        break;
    }

    case BTN_PRESS_DOWN_KEY:
        keyboardtext = "Down Key";
        audio->PlaySound(sound_Hop);

        _StepX = 0; _StepY = 32;
        _TileX = 5;
        _TileY = 0;
        _EndTileX = 4; _EndTileY = 0;
        frog->StartAnimation(_TileX,_TileY,Frogger_TIME,1,_StepX,_StepY);
        //frog->MoveSprite(_StepX,_StepY,Frogger_TIME,2,_Elapsed,_TileX,_TileY);
        break;

    case BTN_PRESS_LEFT_KEY:
        keyboardtext = "Left Key";
        audio->PlaySound(sound_Hop);

        _StepX = -32; _StepY = 0;
        _TileX = 3;
        _TileY = 0;
        _EndTileX = 2; _EndTileY = 0;
        frog->StartAnimation(_TileX,_TileY,Frogger_TIME,1,_StepX,_StepY);
        // frog->MoveSprite(_StepX,_StepY,Frogger_TIME,2,_Elapsed,_TileX,_TileY);
        break;

    case BTN_PRESS_RIGHT_KEY:
        keyboardtext = "Right Key";
        audio->PlaySound(sound_Hop);

        _StepX = 32; _StepY = 0;
        _TileX = 7;
        _TileY = 0;
        _EndTileX = 6; _EndTileY = 0;
        frog->StartAnimation(_TileX,_TileY,Frogger_TIME,1,_StepX,_StepY);
        //frog->MoveSprite(_StepX,_StepY,Frogger_TIME,2,_Elapsed,_TileX,_TileY);
        break;

    case BTN_PRESS_SPACE_KEY:
        keyboardtext = "Space Key";
        break;


    case BTN_UP_UP_KEY:
    case BTN_UP_DOWN_KEY:
    case BTN_UP_LEFT_KEY:
    case BTN_UP_RIGHT_KEY:
        frog->EndAnimation(_TileX,_TileY);
        break;
    default:
        keyboardtext = "No Key";
        _StepX = 0;
        _StepY = 0;
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
        UserUpdate(state); // Nur mal testen


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


        // for (ENGINE::Base* elem: Holz)
        //      elem->Render();

        //  Frogger->Render();
        // frog->Render();

        //frog->RenderFromAsset(0,1);
        if ( ! frog->AnimationDone() )

        frog->MoveSprite(_StepX,_StepY,Frogger_TIME,1,_Elapsed,_TileX,_TileY);


        // frog->Animate(elapsed,
        //             _StepX,_StepY,
        //             _TileX,
        //             _TileX+1,
        //             _TileY);
        else{

            //    frog->EndAnimation(_EndTileX,_EndTileY);
            frog->RenderFromAsset(_EndTileX,_EndTileY);
        }


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
    for (int i =0; i<5; i++){
        ENGINE::Base* obj = new ENGINE::BaseObject2D(_ResX,_ResY,"/home/paul/workspace/GLFrameWork/images/Holztexture/Wood.png",_Shader);

        obj->setPos(x,400);
        obj->setSize(150,40);
        x+=200;
        Holz.push_back(obj);
    }

    Frogger = new ENGINE::BaseObject2D(_ResX,_ResY,"/home/paul/workspace/images/retrogames/frogger/Sprites640x560.png",_Shader);
    Frogger->setPos(300,400);

    //CarsAndSnakes32x32.png
    frog = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/images/retrogames/frogger/Froggs8x4.png",_Shader); //graphics-game-sprites640x560.png",_Shader);x
    frog->setPos(500,800);
    frog->InitTextureMap(8,4);
    //frog->SetTimeToAnimate(1500);
    //frog->SetCountSequences(1);

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

    // -----------------------
    // Mp3's
    // -----------------------
    // hintergrundsound laden, etwas nervig aber witzig...!!
    sound_Background = audio->LoadBackgroundSound("/home/paul/workspace/sounds/retrogames/frogger/AudacityModiyfied/EndlessBackground.mp3");    return ret;
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

