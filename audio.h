#ifndef AUDIO_H
#define AUDIO_H
#include </usr/include/SDL2/SDL.h>
#include </usr/include/SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <atomic>


#define PI2 6.28318530718

/*
 * Example Wav aus jsfxr
 */


typedef struct {
    SDL_AudioSpec spec;
    Uint8* buffer;
    Uint32 len;
}Wav8Bit;

typedef struct{
    Mix_Music * music;
    int id ;
}Mp3struct;




class Audio
{
public:
    Audio();
    ~Audio();

    void Play8BitWav(Wav8Bit wav,int audioid,bool endless = false); //Wave8Bit Structs in map !!
    bool LoadWav(std::string path, Wav8Bit &wav);

    Mix_Music * LoadBackgroundSound(std::string path);

    Mix_Music * GetMusic(int id);



    // -------------------------------------------
    // Neu mit mix
    // -------------------------------------------
    Mix_Chunk * LoadWavMixSound(std::string path);
    void PlaySound(Mix_Chunk * sound);
    void PlayMp3(Mix_Chunk *music, bool endless = true);
    void PlayBackrgoundSound(Mix_Music * music,bool endless = true,int loops=0);

    bool Mp3InitDone();
    bool InitAudioSpec(Wav8Bit &spec, int &audioid);

    void ReleaseAudioWav(Wav8Bit &wav,int audioid);
    void ReleaseAudioMP3(Mix_Music * music);


protected:
    // 8 Bit Wav:
    Wav8Bit JumpSpec; // Ist eigentlich hitHurt.wav
    // MP3:
    Mix_Music* backgroundmusic;
    Mix_Music* startup;
    Mix_Music* frogdeath;

    std::map<int,Wav8Bit> WavMap;
    std::map<int,Mp3struct> Mp3Map;

private :

    bool Mp3_Init_OK;


    int _AudioId;
    void Init();
    bool OpenMixerDevice();
    static void audiocallback (void* data,Uint8* stream,int len);
    static void on_channel_finished(int channel);
    void PlayMixChunckAsync(Mix_Chunk * chunck);
};

#endif // AUDIO_H

