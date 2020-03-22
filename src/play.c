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




// Return 1 if two sprites share the same X coordinates (regardless of Y coordinates), 0 otherwise
int conflictX(sprite anime, sprite object) {
  if ((anime.position).x > (object.position).x && (anime.position).x < (object.position).x + XBOX)
    return 1;
  if ((anime.position).x + XROBOT > (object.position).x && (anime.position).x + XROBOT < (object.position).x + XBOX)
    return 1;
  if ((anime.position).x < (object.position).x && (anime.position).x + XROBOT > (object.position).x + XBOX)
    return 1;
  return 0;
}



// Return 1 if two sprites share the same Y coordinates (regardless of X coordinates), 0 otherwise
int conflictY(sprite anime, sprite object) {
  if ((anime.position).y >= (object.position).y && (anime.position).y <= (object.position).y + HBOX)
    return 1;
  if ((anime.position).y + HROBOT >= (object.position).y && (anime.position).y + HROBOT <= (object.position).y + HBOX)
    return 1;
  if ((anime.position).y  <= (object.position).y && (anime.position).y + HROBOT >= (object.position).y + HBOX)
    return 1;
  return 0;
}




// Checks if there is a collision between hitboxes along X axis and with robot shifting to the left direction
// Update current robot position
void hitboxLeftX(tabsprite* tab, int shifting) {
  if (  (conflictY(tab->robot, tab->box) == 1) &&
        (((tab->robot).position).x - shifting <= ((tab->box).position).x + XBOX) &&
        (((tab->robot).position).x + XROBOT > ((tab->box).position).x + XBOX)
     )
    ((tab->robot).position).x = ((tab->box).position).x + XBOX;
  else
    ((tab->robot).position).x -= shifting;
}


// Checks if there is a collision between hitboxes along X axis and with robot shifting to the right direction
// Update current robot position
void hitboxRightX(tabsprite* tab, int shifting) {
  if (  (conflictY(tab->robot, tab->box) == 1) &&
        (((tab->robot).position).x + XROBOT + shifting >= ((tab->box).position).x) &&
        (((tab->robot).position).x < ((tab->box).position).x)
     )
    ((tab->robot).position).x = ((tab->box).position).x - XROBOT;
  else
    ((tab->robot).position).x += shifting;
}


// Checks if there is a collision between hitboxes along Y axis and with robot shifting to the down direction
// Update current robot position
void hitbox_DownY(tabsprite* tab, int shifting) {
  if (  (conflictX(tab->robot, tab->box) == 1) &&
        (((tab->robot).position).y + HROBOT + shifting >= ((tab->box).position).y) &&
        (((tab->robot).position).y + HROBOT + shifting <= ((tab->box).position).y + HBOX)
     )
    ((tab->robot).position).y = ((tab->box).position).y - HROBOT - 1;
  else
    ((tab->robot).position).y += shifting;
}



// Checks if there is a collision between hitboxes along Y axis and with robot shifting to the up direction
// Update current robot position
void hitbox_UpY(tabsprite* tab, int shifting) {
  if (  (conflictX(tab->robot, tab->box) == 1) &&
        (((tab->robot).position).y - shifting <= ((tab->box).position).y + HBOX) &&
        (((tab->robot).position).y - shifting >= ((tab->box).position).y)
      )
    ((tab->robot).position).y = ((tab->box).position).y + HBOX;
  else
    ((tab->robot).position).y -= shifting;
}




// Allows the player to move upwards if it is possible
void jumpDownAnimation(tabsprite* tab, char** inputKey) {
  SDL_Event event;
  int code; int i = 0; int continuer = 1;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 41, 70};    // To make the jump more harmonic (reverse square function)
  while (continuer) {
    tab->robot.position.y ++;
    if ( conflictY(tab->robot, tab->floor) || (conflictY(tab->robot, tab->box) && conflictX(tab->robot, tab->box)) ) {
      continuer = 0;
      tab->robot.position.y --;
      break;
    }
    tab->robot.position.y --;
    if (i < 24)
      i++;
    hitbox_DownY(tab, 4);
    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      hitboxLeftX(tab, 4);
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      hitboxRightX(tab, 4);
    }
    updateWindow(tab);
    SDL_Delay(time[24-i]);
  }
}





// Allows the player to move downwards if it is possible, until a hard surface is encountered
void jumpUpAnimation(tabsprite* tab, char** inputKey) {
  SDL_Event event;
  int i, code;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 41, 70};    // To make the jump more harmonic (reverse square function)
  for (i = 0; i < 25; i++) {
    hitbox_UpY(tab, 4);
    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
        hitboxLeftX(tab, 4);
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
        hitboxRightX(tab, 4);
    }
    updateWindow(tab);
    SDL_Delay(time[i]);
  }
}




// Principal player control code
// Return ERRORVALUE if an error has occured, 0 otherwise
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

  SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));      // Add the background to the SDL window
  SDL_BlitSurface((tab->floor).surface, NULL, (tab->game).surface, &((tab->floor).position));      // Add the floor to the SDL window

  (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");            // Load the robot surface oriented to the right
  ((tab->robot).position).x = 100;                                                  // Initial position of the robot surface
  ((tab->robot).position).y = 200;
  SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &(tab->robot.position));    // Add the robot surface to the SDL window
  (tab->box).surface = SDL_LoadBMP("img/box.bmp");                                      // Load the robot surface

  SDL_BlitSurface((tab->box).surface, NULL, (tab->game).surface, &((tab->box).position));       // Add the robot surface to the SDL window

  SDL_Flip((tab->game).surface);       // Update the game window

  SDL_EnableKeyRepeat(10, 10);      // Enable a new event every 10 ms if a key remains pressed
  while(continuer) {
    inputEvent(&inputKey);
    if (inputKey[SDLK_ESCAPE] == 1)
      continuer = 0;
    if (inputKey[SDLK_SPACE] == 1) {
      jumpUpAnimation(tab, &inputKey);
      jumpDownAnimation(tab, &inputKey);
    }
    if (inputKey[SDLK_LEFT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
        hitboxLeftX(tab, 2);
        jumpDownAnimation(tab, &inputKey);
    }
    if (inputKey[SDLK_RIGHT] == 1) {
      (tab->robot).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
        hitboxRightX(tab, 2);
        jumpDownAnimation(tab, &inputKey);
    }
    updateWindow(tab);

  }
  free(inputKey);
  return 0;
}
