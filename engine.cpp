#include "engine.h"
#include <iostream>
#include <thread>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include <imageloader.h>


const uint64_t Frogger_TIME = 100;
const int Frogger_FRAMES = 2; //5;
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

    ReleaseTrees();

    delete _Score;
    delete _HighScore;
    delete _Time;


    // wav freigeben
    Mix_FreeChunk(sound_Hop);
    Mix_FreeChunk(sound_Startup);
    Mix_FreeChunk(sound_FrogDeath);

    // Mixer_Music freigeben
    Mix_FreeMusic(sound_Background);
}

void TestEngine::ReleaseTrees(){
    for(int i = 0; i < FLOATOBJECTS_PER_ROW_1; i++)
        delete Baum_Row1[i];

    for(int i = 0; i < FLOATOBJECTS_PER_ROW_2; i++)
        delete Baum_Row2[i];

    for(int i = 0; i < FLOATOBJECTS_PER_ROW_3; i++)
        delete Baum_Row3[i];

    for(int i = 0; i < FLOATOBJECTS_PER_ROW_4; i++)
        delete Baum_Row4[i];

    for(int i = 0; i < FLOATOBJECTS_PER_ROW_5; i++)
        delete Baum_Row5[i];
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
    for (int i =0; i < FLOATOBJECTS_PER_ROW_1;i++){
        // Erstmal alles rendern
        Baum_Row1[i]->MoveSprite(0,0,128,64,100,Step_Trees_1,0,_Elapsed,tmp);
    }

    for (int i =0; i < FLOATOBJECTS_PER_ROW_2;i++){
        // Erstmal alles rendern
        Baum_Row2[i]->MoveSprite(0,0,128,64,100,-Step_Trees_2,0,_Elapsed,tmp);
    }

    for (int i =0; i < FLOATOBJECTS_PER_ROW_3;i++){
        // Erstmal alles rendern
        Baum_Row3[i]->MoveSprite(0,0,128,64,100,Step_Trees_3,0,_Elapsed,tmp);
    }

    for (int i =0; i < FLOATOBJECTS_PER_ROW_4;i++){
        // Erstmal alles rendern
        Baum_Row4[i]->MoveSprite(0,0,128,64,100,-Step_Trees_4,0,_Elapsed,tmp);
    }

    for (int i =0; i < FLOATOBJECTS_PER_ROW_5;i++){
        // Erstmal alles rendern
        Baum_Row5[i]->MoveSprite(0,0,128,64,100,Step_Trees_5,0,_Elapsed,tmp);
    }
}

int TestEngine::GetFloatingStep(){
  int row = FrogInRow();

    // default:
    int ret = Step_Trees_1;
    switch (row)
    {
        case 1: ret = Step_Trees_1; break;
        case 2: ret = Step_Trees_2; break;
        case 3: ret = Step_Trees_3; break;
        case 4: ret = Step_Trees_4; break;
        case 5: ret = Step_Trees_5; break;
        default:
            ret = Step_Trees_1; break;
    }
        return ret;
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
            p.x += GetFloatingStep();
            frog->SetPosition(p.x,p.y);
        }
        else
            if (GameState == GAMESTATE::FloatingLeft) {
                sPoint p = frog->Pos();
                p.x -= GetFloatingStep();
                frog->SetPosition(p.x,p.y);
            }
        frog->RenderFromAsset(_EndTileX,_EndTileY);
    }
}

void TestEngine::RenderScore(){
    //_Score->Render();
    //_HighScore->Render();
    _Time->RenderText("TIME",sPoint(_ResX/2,_ResY -70));
}

void TestEngine::GetNewState(){

    int frogrow = FrogInRow();
    bool plunk = true;
    switch(frogrow)
    {
    case 0:
        GameState = GAMESTATE::Arrived;

        break;
    case 1:
    {
        sPoint p = frog->Pos();
        if (frog->PosX() > _ResX )
        {
            GameState = GAMESTATE::Die;
            // Wir setzen den Frosch aber in den Bildbereich..
            frog->setPos(_ResX - frog->SpriteSize().w ,p.y);
            StartDieAnimation(4,6);
        }
        else
        {
            for (int i =0; i< FLOATOBJECTS_PER_ROW_1; i++) {
                if (Baum_Row1[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingRight;
                    plunk = false;
                }
            }
            if (plunk) {
                GameState = GAMESTATE::Plunk;
                break;
            }
            else
            GameState = GAMESTATE::FloatingRight;
        }
    }
        break;

    case 2:
    {
        sPoint p = frog->Pos();
        if (frog->PosX()- frog->SpriteSize().w <  -frog->SpriteSize().w){
            GameState = GAMESTATE::Die;
            // Wir setzen den Frosch aber in den Bildbereich..
            frog->setPos(0,p.y);
            StartDieAnimation(4,6);
        }
        else{

            for (int i =0; i< FLOATOBJECTS_PER_ROW_2; i++) {
                if (Baum_Row2[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingLeft;
                    plunk = false;
                }
            }
            if (plunk) {
                GameState = GAMESTATE::Plunk;
                break;
            }
            else
                GameState = GAMESTATE::FloatingLeft;
        }
    }
        break;
    case 3:
    {
        sPoint p = frog->Pos();
        if (frog->PosX() > _ResX )
        {
            GameState = GAMESTATE::Die;
            // Wir setzen den Frosch aber in den Bildbereich..
            frog->setPos(_ResX - frog->SpriteSize().w ,p.y);
            StartDieAnimation(4,6);
        }
        else
        {
            for (int i =0; i< FLOATOBJECTS_PER_ROW_3; i++) {
                if (Baum_Row3[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingRight;
                    plunk = false;
                }
            }
            if (plunk) {
                GameState = GAMESTATE::Plunk;
                break;
            }
            else
                GameState = GAMESTATE::FloatingRight;
        }
    }
        break;
    case 4:
    {
        sPoint p = frog->Pos();
        if (frog->PosX()- frog->SpriteSize().w <  -frog->SpriteSize().w){
            GameState = GAMESTATE::Die;
            // Wir setzen den Frosch aber in den Bildbereich..
            frog->setPos(0,p.y);
            StartDieAnimation(4,6);
        }
        else
        {
            for (int i =0; i< FLOATOBJECTS_PER_ROW_4; i++) {
                if (Baum_Row4[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingLeft;
                    plunk = false;
                }
            }
            if (plunk) {
                GameState = GAMESTATE::Plunk;
                break;
            }
            else
                GameState = GAMESTATE::FloatingLeft;
        }
    }
        break;

    case 5:
    {

        // ----------------------------------------------
        // Erstmal checken, ob wir den Bereich verlassen:
        // ----------------------------------------------
        sPoint p = frog->Pos();
        if (frog->PosX() > _ResX )
        {
            GameState = GAMESTATE::Die;
            // Wir setzen den Frosch aber in den Bildbereich..
            frog->setPos(_ResX - frog->SpriteSize().w ,p.y);
            StartDieAnimation(4,6);
        }
        else
        {
            for (int i =0; i< FLOATOBJECTS_PER_ROW_5; i++) {
                if (Baum_Row5[i]->IsColliding(frog->Pos(),frog->SpriteSize()) ) {
                    GameState = GAMESTATE::FloatingRight;
                    plunk = false;
                }
            }
            if (plunk) {
                GameState = GAMESTATE::Plunk;
                break;
            }
            else
                GameState = GAMESTATE::FloatingRight;
        }
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

    case 12:


        break;
    default:
        break;

    }
}

void TestEngine::StartDieAnimation(int starttile, int endtile){

    if (starttile < 0) starttile = 0;
    if (endtile > 6) endtile = 6;

    frogdeath->StartAnimation(starttile,endtile);
    frogdeath->SetPosition(frog->PosX(),frog->PosY());

    frogdeath->setRenderSprite(true);
    frog->setRenderSprite(false);

    audio->HaltMusic();
    audio->ChannelToListen(AUDIO_Channel_Death);
    audio->PlaySound(sound_FrogDeath,AUDIO_Channel_Death);
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
                    Baum_Row5[0]->RenderFromAsset(0,0);


                    cout << "Frogpos.x  " << frog->PosX() <<  endl;
                    cout << "Frogpos.y  " << frog->PosY() <<  endl;

                    cout << "Snake .x  " << snake->PosX() <<  endl;
                    cout << "Snake .y  " << snake->PosY() <<  endl;

                break;

                case GAMESTATE::Starting:

                    RenderSplashScreen();

                    if (audio->PlaySoundFinished(AUDIO_Channel_StartUp))
                        GameState = GAMESTATE::StartUpFinished;
                    break;

                case GAMESTATE::StartUpFinished:
                    RenderBackgroundSprites();
                    RenderScore();
                    audio->PlayBackrgoundSound(sound_Background,1);
                    frog->SetPosition(608,802);

                    GameState= GAMESTATE::Run;
                    break;
                case GAMESTATE::Run:

                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                    RenderScore();
                 //   GetNewState();

                    if (! snake->IsColliding(frog->Pos(),frog->Size())) {

                        bool tmp;
                        snake->MoveSprite(0,2,100,128,64,Step_Snake,0,_Elapsed,tmp);
                    }
                    else
                    {
                        GameState  = GAMESTATE::Die;
                        StartDieAnimation(0,6);
                    }

                    break;
                case GAMESTATE::FloatingRight:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                    RenderScore();
               //     GetNewState();
                    break;

                case GAMESTATE::FloatingLeft:
                    RenderBackgroundSprites();
                    RenderWood();
                    RenderFrog();
                    RenderScore();
                //    GetNewState();
                    break;

                case GAMESTATE::TimeOut:
                    RenderBackgroundSprites();
                    RenderScore();
                    break;
                case GAMESTATE::Plunk:
                    RenderBackgroundSprites();
                    RenderWood();
                 //   RenderFrog();
                    RenderScore();
                    audio->ChannelToListen(AUDIO_Channel_Plunck);
                    audio->PlaySound(sound_Plunk,AUDIO_Channel_Plunck);

                    if (audio->PlaySoundFinished(AUDIO_Channel_Plunck)){

                        GameState = GAMESTATE::Die;
                        StartDieAnimation(4,6);

                    }
                    break;
                case GAMESTATE::Die:

                    RenderBackgroundSprites();
                    RenderWood();
                    RenderScore();
                    // Frog death
                    bool animdone;
                    frogdeath->MoveSprite(0,6,64,64,150,0,0,_Elapsed,animdone);

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
                    RenderScore();
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
                    RenderScore();

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

void TestEngine::InitTextMap(){

    // Zeile 0
     _Score->AddCharToMap('0',0,0);
     _Score->AddCharToMap('1',1,0);
     _Score->AddCharToMap('2',2,0);
     _Score->AddCharToMap('3',3,0);
     _Score->AddCharToMap('4',4,0);
     _Score->AddCharToMap('5',5,0);

     _Score->AddCharToMap('6',6,0);
     _Score->AddCharToMap('7',7,0);
     _Score->AddCharToMap('8',8,0);
     _Score->AddCharToMap('9',9,0);
     _Score->AddCharToMap('A',10,0);
     _Score->AddCharToMap('B',11,0);

     _Score->AddCharToMap('C',12,0);
     _Score->AddCharToMap('D',13,0);
     _Score->AddCharToMap('E',14,0);
     _Score->AddCharToMap('F',15,0);
     _Score->AddCharToMap('G',16,0);
     _Score->AddCharToMap('H',17,0);
     _Score->AddCharToMap('I',18,0);


     // Zeile 1
     _Score->AddCharToMap('J',0,1);
     _Score->AddCharToMap('K',1,1);
     _Score->AddCharToMap('L',2,1);
     _Score->AddCharToMap('M',3,1);
     _Score->AddCharToMap('N',4,1);
     _Score->AddCharToMap('O',5,1);

     _Score->AddCharToMap('P',6,1);
     _Score->AddCharToMap('Q',7,1);
     _Score->AddCharToMap('R',8,1);
     _Score->AddCharToMap('S',9,1);
     _Score->AddCharToMap('T',10,1);
     _Score->AddCharToMap('U',11,1);

     _Score->AddCharToMap('V',12,1);
     _Score->AddCharToMap('W',13,1);
     _Score->AddCharToMap('X',14,1);
     _Score->AddCharToMap('Y',15,1);
     _Score->AddCharToMap('Z',16,1);
     _Score->AddCharToMap('-',17,1);
     _Score->AddCharToMap('c',18,1); // copright

     _HighScore->AddMapToMap(_Score->GetCharacters());
     _Time->AddMapToMap(_Score->GetCharacters());
}

bool TestEngine::InitUserObjects(){

    bool ret = true;

    // ----------------------------------------------------------------------
    // Step init, bei jedem höheren level erhöhen, erhöht die geschwindigkeit
    // ----------------------------------------------------------------------
    Step_Trees_1 = 3;
    Step_Trees_2 = 2;
    Step_Trees_3 = 2;
    Step_Trees_4 = 4;
    Step_Trees_5 = 1;


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
    snake = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Snakes3x1_128_68.png",_Shader);
    snake->InitTextureMap(3,1);
    snake->SetPosition(_ResX,800);
    snake->setInstanceName("<SNAKE>");

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

    InitTreeRows();

    // Default settings at start
    _TileX = 0;
    _TileY = 0;
    _EndTileX = 0; _EndTileY = 0;

    // _SplashScreen
    _SplashScreen = new ENGINE::BaseObject2D(_ResX,_ResY,"/home/paul/workspace/Frogger/images/frogsplash2.png",_Shader);
    _SplashScreen->setPos(200,200);

    // ------------------------------------------
    // Score, Highscore, Time
    // ------------------------------------------

    _Score = new COSTUMTEXT::TextBase(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Text32x32_19_2.png",_Shader);
    _Score->InitTextureMap(19,2);
    _Score->setPos(_ResX / 2 -300,_ResY-70);


    _HighScore = new COSTUMTEXT::TextBase(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Text32x32_19_2.png",_Shader);
    _HighScore->InitTextureMap(19,2);
    _HighScore->setPos(_ResX - 200,_ResY-70);

    _Time = new COSTUMTEXT::TextBase(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Text32x32_19_2.png",_Shader);
    _Time->InitTextureMap(19,2);
    _Time->setPos(_ResX / 2 -300,0);

    _Time->AddText("TIME");



    InitTextMap();

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

void TestEngine::InitTreeRows(){
    // -------------------------
    // Bäume Row1 - Row5
    // -------------------------
    int x = 0;
    for (int i =0; i < FLOATOBJECTS_PER_ROW_5;i++){
        Baum_Row5[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum320x68.png",_Shader);
        Baum_Row5[i]->InitTextureMap(1,1);
        Baum_Row5[i]->SetPosition(x,352);
        Baum_Row5[i]->StartAnimation(0,0);
        x+= 450;
    }

    x = 0;
    for (int i =0; i < FLOATOBJECTS_PER_ROW_4;i++){
        Baum_Row4[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum120x68.png",_Shader);
        Baum_Row4[i]->InitTextureMap(1,1);
        Baum_Row4[i]->SetPosition(x,290);
        Baum_Row4[i]->StartAnimation(0,0);
        x+= 250;
    }

    x = 0;
    for (int i =0; i < FLOATOBJECTS_PER_ROW_3;i++){
        Baum_Row3[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum320x68.png",_Shader);
        Baum_Row3[i]->InitTextureMap(1,1);
        Baum_Row3[i]->SetPosition(x,226);
        Baum_Row3[i]->StartAnimation(0,0);
        x+= 480;
    }

    x = 0;
    for (int i =0; i < FLOATOBJECTS_PER_ROW_2;i++){
        Baum_Row2[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum120x68.png",_Shader);
        Baum_Row2[i]->InitTextureMap(1,1);
        Baum_Row2[i]->SetPosition(x,162);
        Baum_Row2[i]->StartAnimation(0,0);
        x+= 400;
    }

    x = 0;
    for (int i =0; i < FLOATOBJECTS_PER_ROW_1;i++){
        Baum_Row1[i] = new ENGINE::Sprite(_ResX,_ResY,"/home/paul/workspace/Frogger/images/Baum120x68.png",_Shader);
        Baum_Row1[i]->InitTextureMap(1,1);
        Baum_Row1[i]->SetPosition(x,98);
        Baum_Row1[i]->StartAnimation(0,0);
        x+= 400;
    }
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

