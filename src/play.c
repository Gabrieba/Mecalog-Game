#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "../include/main.h"


// Pause the graphic SDL display
// Wait the user request to quit the SDL window
void pauseSDL(void) {
  int etat = 1;
  SDL_Event event;
  while (etat) {
    SDL_WaitEvent(&event);
    switch(event.type) {
      case SDL_QUIT :
        etat = 0;
    }
  }
}



// Lists all entries from the keyboard
// Store it in the array 'keytab' (1 if key pressed, 0 otherwise)
void inputEvent(char** keytab) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        return;
      case SDL_KEYDOWN:
        (*keytab)[event.key.keysym.sym] = 1;
        break;
      case SDL_KEYUP:
        (*keytab)[event.key.keysym.sym] = 0;
        break;
      default:
        break;
    }
  }
}





void jumpAnimation(tabsprite* tab, char** inputKey) {
  SDL_Event event;
  int i, code;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 41, 70};    // To make the jump more harmonic (reverse square function)

  for (i = 0; i < 25; i++) {
    ((tab->robot).position).y -= 4;
    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      ((tab->robot).position).x -= 4;
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      ((tab->robot).position).x += 4;
    }

    SDL_FillRect((tab->game).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
    ((tab->game).position).x = 0;
    ((tab->game).position).y = 0;
    SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));       // Add the background to the SDL window
    ((tab->box).position).x = 165;
    ((tab->box).position).y = 227;
    SDL_BlitSurface((tab->box).surface, NULL, (tab->game).surface, &((tab->box).position));        // Add the box surface to the SDL window
    SDL_SetColorKey((tab->robot).surface, SDL_SRCCOLORKEY, SDL_MapRGB(((tab->robot).surface)->format, 255, 51, 153));
    SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &((tab->robot).position));
    SDL_Flip((tab->game).surface);                                        // Update the game window
    SDL_Delay(time[i]);
  }

  for (i = 0; i < 25; i++) {
    ((tab->robot).position).y += 4;
    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      ((tab->robot).position).x -= 4;
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      ((tab->robot).position).x += 4;
    }

    SDL_FillRect(tab->game.surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
    ((tab->game).position).x = 0;
    ((tab->game).position).y = 0;
    SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));       // Add the background to the SDL window
    ((tab->box).position).x = 165;
    ((tab->box).position).y = 227;
    SDL_BlitSurface(tab->box.surface, NULL, (tab->game).surface, &((tab->box).position));        // Add the box surface to the SDL window
    SDL_SetColorKey((tab->robot).surface, SDL_SRCCOLORKEY, SDL_MapRGB(((tab->robot).surface)->format, 255, 51, 153));
    SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &((tab->robot).position));
    SDL_Flip((tab->game).surface);                                        // Update the game window
    SDL_Delay(time[24-i]);
  }
}






int playGame(tabsprite* tab) {
  SDL_Event event;
  char* inputKey;
  int code;
  inputKey = calloc(SDLK_LAST, sizeof(*inputKey));
  if (inputKey == NULL) {
    errorMSG("Error while allocating memory to the key array");
    return ERRORVALUE;
  }
  int continuer = 1;

  SDL_FillRect((tab->game).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
  ((tab->game).position).x = 0;
  ((tab->game).position).y = 0;
  SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));      // Add the background to the SDL window

  (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");            // Load the robot surface oriented to the right
  ((tab->robot).position).x = 100;                                                  // Initial position of the robot surface
  ((tab->robot).position).y = 200;
  SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &(tab->robot.position));    // Add the robot surface to the SDL window
  (tab->box).surface = SDL_LoadBMP("img/box.bmp");                                      // Load the robot surface
  ((tab->box).position).x = 165;                                                  // Initial position of the robot surface
  ((tab->box).position).y = 227;
  SDL_BlitSurface((tab->box).surface, NULL, (tab->game).surface, &((tab->box).position));       // Add the robot surface to the SDL window

  SDL_Flip((tab->game).surface);       // Update the game window

  SDL_EnableKeyRepeat(10, 10);      // Enable a new event every 10 ms if a key remains pressed
  while(continuer) {
    inputEvent(&inputKey);
    if (inputKey[SDLK_ESCAPE] == 1)
      continuer = 0;
    if (inputKey[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      ((tab->robot).position).x -= 2;
    }
    if (inputKey[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      ((tab->robot).position).x += 2;
    }
    if (inputKey[SDLK_SPACE] == 1) {
      jumpAnimation(tab, &inputKey);
    }
    SDL_FillRect((tab->game).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
    ((tab->game).position).x = 0;
    ((tab->game).position).y = 0;
    SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));       // Add the background to the SDL window
    ((tab->box).position).x = 165;
    ((tab->box).position).y = 227;
    SDL_BlitSurface((tab->box).surface, NULL, (tab->game).surface, &((tab->box).position));        // Add the box surface to the SDL window
    SDL_SetColorKey((tab->robot).surface, SDL_SRCCOLORKEY, SDL_MapRGB(((tab->robot).surface)->format, 255, 51, 153));
    SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &((tab->robot).position));
    SDL_Flip((tab->game).surface);       // Update the game window
  }
  free(inputKey);
  return 0;
}
