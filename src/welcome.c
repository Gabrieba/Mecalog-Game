#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>



#include "../include/main.h"

static Uint32 audio_len;
static Uint32 audio_pos;
SDL_AudioSpec want, have;
Uint8 *audioBuffer;

void MyAudioCallBack(void *udata, Uint8 *stream, int len)
{


    /* Only play if we have data left */
    // if ( audio_len == 0 )
    //     return;
    // puts("OK");
    // /* Mix as much data as possible */
    // len = ( len > audio_len ? audio_len : len );
    // SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    // audio_pos += len;
    // audio_len -= len;
    // Remise à zéro du tampon de sortie
    // On ne lit que s'il reste des données à jouer

    if ( audio_len == 0 )
        return;

    // Remise à zéro du tampon de sortie
    memset(stream, 0, len);

    // Lecture du buffer audio
    printf("audio_pos=%d\n", audio_pos);
    if (audio_pos < want.size) {
        if (audio_pos+len > want.size)
            len = want.size = audio_pos;
        puts("OK");
        SDL_MixAudio(stream, audioBuffer + audio_pos, len, SDL_MIX_MAXVOLUME);
        audio_pos += len;
    }

    // Décrémentation de ce qu'il reste à lire
    audio_len -= len;
}



// Display the welcoming page before the game starts
// Return ERRORVALUE if an error has occured, 0 otherwise
int welcomeGame(tabsprite* tabBlocs, tabsprite* tabScenery) {

  Uint32 audioBufferLen;

  SDL_memset(&want, 0, sizeof(want));

  want.freq = 44100; // 4 100 Hz, 48 000 Hz, 96 000 Hz, 192 000 Hz (standard)
  want.format = AUDIO_S16SYS;
  want.channels = 1; // mono
  want.samples = 4096; // Oublier pas que ce sa doit être en puissance de deux 2^n
  want.callback = MyAudioCallBack;

  if (SDL_OpenAudio(&want, &have) < 0)
    printf("Failed to open audio: %s", SDL_GetError());

  if(!SDL_LoadWAV("sound/Wooden_explosion.wav", &have, &audioBuffer, &audioBufferLen)) {
      printf("Erreur lors du chargement du fichier WAV.\n");
      printf("%s\n", SDL_GetError());
      return ERRORVALUE;
  }
  audio_len = want.size;
  SDL_PauseAudio(0); /* start audio playing. */

  // while ( audio_len > 0 )
  //   SDL_Delay(100);
  SDL_Delay(5000);
  SDL_FreeWAV(audioBuffer);
  SDL_CloseAudio();



  return 0;
}
