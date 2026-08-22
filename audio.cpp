#include "audio.h"
#include <iostream>



float _Time = 0;
float _Freq = 320;//440;

Audio::Audio(){
    Init();
}

Audio::~Audio(){
    SDL_CloseAudioDevice(_AudioId);
    // SDL_FreeWAV(JumpSpec.buffer);

    // if (backgroundmusic != nullptr)
    //     Mix_FreeMusic(backgroundmusic);

    Mix_CloseAudio();
    Mix_Quit();

}

void Audio::ReleaseAudioWav(Wav8Bit &wav,int audioid){

    SDL_CloseAudioDevice(audioid);
    SDL_FreeWAV(wav.buffer);


    // Mix_FreeMusic(wav);
}

void Audio::ReleaseAudioMP3(Mix_Music * music){
    Mix_FreeMusic(music);
}

void Audio::audiocallback(void* data,Uint8* stream,int len)
{
    /*
    short * snd = reinterpret_cast<short*>(stream);
    len /= sizeof(*snd);
    for(int i = 0; i < len; i++) //Fill array with frequencies, mathy-math stuff
    {
        snd[i] = 5000 * sin(_Time);

        _Time += _Freq * PI2 / 48000.0;
        if(_Time >= PI2)
            _Time -= PI2;
    }
  */

}


Mix_Chunk* Audio::LoadWavMixSound(std::string path){
    Mix_Chunk* tmp = Mix_LoadWAV(path.c_str());

    if (tmp == nullptr) {
        std::cout << "Fehler beim laden von " << path << ".wav Datei" << std::endl;
        return nullptr;
    }
    return tmp;
}



bool Audio::LoadWav(std::string path, Wav8Bit &wav){
    if (SDL_LoadWAV(path.c_str(),&wav.spec,&wav.buffer,&wav.len) == NULL)
    {
        std::cout << "Fehler beim laden von " << path << ".wav Datei" << std::endl;
        return false;
    }
    std::cout << "Laden von " << path << " success!"<< std::endl;
    return true;
}

// bool Audio::LoadMP3(std::string path){

//     mp3 = Mix_LoadMUS(path.c_str());


//     if (! mp3) {
//         std::cout << "Fehler beim laden der Datei " << path << "Fehler : " << Mix_GetError() << std::endl;
//         return false;
//     }
//     std::cout << "Laden der Datei " << path << " success!" << std::endl;
//     return true;
// }

Mix_Music * Audio::LoadBackgroundSound(std::string path){
    return Mix_LoadMUS(path.c_str());
}

void Audio::PlayBackrgoundSound(Mix_Music *music, bool endless, int loops){

    if (endless)
        Mix_PlayMusic(music,-1);
    else
        Mix_PlayMusic(music,loops);
}

void Audio::PlayMp3(Mix_Chunk *music, bool endless){

    int channel = Mix_PlayChannel(-1, music,3);
    if (channel == -1) {
        std::cout << "Sound konnte nicht abgespielt werden! Fehler: " << Mix_GetError() << std::endl;
    }

    // 5. Warten, bis der Sound fertig abgespielt ist
    // Mix_Playing prüft, ob der Kanal noch aktiv ist
    while (Mix_Playing(channel)) {
        SDL_Delay(100);
    }

    // if (endless)
    //     Mix_PlayMusic(music,-1);
    // else
    //     Mix_PlayMusic(music,1);

}


void Audio::Play8BitWav(Wav8Bit wav, int audioid, bool endless){
    // Todo: id checken , ob vorhanden
    // Queue wav

    SDL_QueueAudio(audioid,wav.buffer,wav.len);

    // Abspielen mit unpause 0 starten

    if (endless)
        SDL_PauseAudioDevice(audioid,1);
    else
        SDL_PauseAudioDevice(audioid,0);

    while (SDL_GetQueuedAudioSize(audioid) > 0) {
        SDL_Delay(200);
    }
}
bool Audio::OpenMixerDevice(){
    // ++++++++++++++++++++++++++++++++++++
    // Todo: einstellungen als argumente !!
    // ++++++++++++++++++++++++++++++++++++
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0){
        std::cout << "Audio Gerät konnte nicht geöffnent werden ." << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}


Mix_Music * GetMusic(int id){

}


bool Audio::Mp3InitDone() {return Mp3_Init_OK;}

bool Audio::InitAudioSpec(Wav8Bit &wav,int & audioid){

    //SDL_zero(wav);

    //Open audio, if error, print

    if ((audioid = SDL_OpenAudioDevice(nullptr, 0, &wav.spec,nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE)) <= 0)
    {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        SDL_FreeWAV(wav.buffer);
        return false;
    }

    /* Start playing, "unpause" */
    SDL_PauseAudioDevice(audioid,0);
    return true;
}

void Audio::PlaySound(Mix_Chunk *sound){
    int channel = Mix_PlayChannel(-1, sound, 0);
    if (channel == -1) {
        std::cout << "Sound konnte nicht abgespielt werden! Fehler: " <<  Mix_GetError() << std::endl;
    }

    // 5. Warten, bis der Sound fertig abgespielt ist
    // Mix_Playing prüft, ob der Kanal noch aktiv ist
    while (Mix_Playing(channel)) {
        SDL_Delay(50);
    }
}

void Audio::Init(){

    // int flags = MIX_INIT_MP3 | MIX_INIT_WAVPACK;
    // if ((Mix_Init(flags) & flags) != flags) {
    //     std::cout << "Mix nicht initialisiert(.mp3 " << Mix_GetError() << std::endl;

    //     //Flag setzen
    //     Mp3_Init_OK = false;
    // }
    // else
    Mp3_Init_OK = true;

    if (OpenMixerDevice())
        std::cout << "Audio initialisiert " << std::endl;

}

