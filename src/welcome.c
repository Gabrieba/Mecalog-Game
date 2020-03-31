#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fmodex/fmod.h>



#include "../include/main.h"


// Display the welcoming page before the game starts
// Return ERRORVALUE if an error has occured, 0 otherwise
int welcomeGame(tabsprite* tabBlocs, tabsprite* tabScenery) {
  FMOD_SYSTEM* system;
  FMOD_SOUND *sound;
  FMOD_RESULT code;
  FMOD_System_Create(&system);
  FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);

  code = FMOD_System_CreateSound(system, "sound/Wooden_explosion.mp3", FMOD_CREATESAMPLE, 0, &sound);
  if (code != FMOD_OK) {
    errorMSG("Impossible de lire le son");
    return -1;
  }

  FMOD_System_PlaySound(system, sound, NULL, 0, NULL);


  FMOD_Sound_Release(sound);
  FMOD_System_Close(system);
  FMOD_System_Release(system);
  return 0;
}
