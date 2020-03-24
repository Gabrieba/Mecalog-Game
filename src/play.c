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
  if ((anime.position).x > (object.position).x && (anime.position).x < (object.position).x + object.dimx)
    return 1;
  if ((anime.position).x + anime.dimx > (object.position).x &&
      (anime.position).x + anime.dimx < (object.position).x + object.dimx)
    return 1;
  if ((anime.position).x <= (object.position).x &&
      (anime.position).x + anime.dimx >= (object.position).x + object.dimx)
    return 1;
  return 0;
}



// Return 1 if two sprites share the same Y coordinates (regardless of X coordinates), 0 otherwise
int conflictY(sprite anime, sprite object) {
  if ((anime.position).y >= (object.position).y && (anime.position).y <= (object.position).y + object.dimy)
    return 1;
  if ((anime.position).y + anime.dimy >= (object.position).y &&
      (anime.position).y + anime.dimy <= (object.position).y + object.dimy)
    return 1;
  if ((anime.position).y  <= (object.position).y &&
      (anime.position).y + anime.dimy >= (object.position).y + object.dimy)
    return 1;
  return 0;
}



// Compute new y position for all blocks
void gravityBlocks(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int shifting) {
  int i, j, bit; int motionless = 0; int k = 0;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 32, 32};    // To make the jump more harmonic (reverse square function)
  while (motionless == 0) {
    motionless = 1;
    for (i = 0; i < tabBlocs->number; i++) {
      bit = 0;
      for (j = 0; j < tabBlocs->number; j++) {

        if (j != i && conflictX((tabBlocs->tab)[i], (tabBlocs->tab)[j]) &&
          (tabBlocs->tab)[i].position.y + (tabBlocs->tab)[i].dimy + shifting >= (tabBlocs->tab)[j].position.y &&
          (tabBlocs->tab)[i].position.y < (tabBlocs->tab)[j].position.y
        ) {                     // A block j directly under the block i
          (tabBlocs->tab)[i].position.y = (tabBlocs->tab)[j].position.y - (tabBlocs->tab)[i].dimy - 1;
          bit = 1;
        }
      }

      if (bit != 1) {
        for (j = 1; j < tabScenery->number; j++) {      // Conflict between block i and scenaristic element j
          if (j != 2 && conflictX((tabBlocs->tab)[i], (tabScenery->tab)[j]) &&
            (tabBlocs->tab)[i].position.y + (tabBlocs->tab)[i].dimy + shifting >= (tabScenery->tab)[j].position.y &&
            (tabBlocs->tab)[i].position.y < (tabScenery->tab)[j].position.y
          ) {                     // Scenery element directly under the block
            (tabBlocs->tab)[i].position.y = (tabScenery->tab)[j].position.y - (tabBlocs->tab)[i].dimy - 1;
            bit = 1;
          }
        }
      }
      if (bit == 0) {
        (tabBlocs->tab)[i].position.y += shifting;
        motionless = 0;
      }
    }
//    SDL_Delay(time[24-k]);
    if (k < 24)
      k++;
  }
}




// Checks if there is a collision between hitboxes along X axis and with robot shifting to the left direction
// Update current robot position and blocks position
void hitboxLeftX(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int shifting) {
  int i, j;
  for (i = 0; i < tabBlocs->number; i++) {
    if (  (conflictY((tabScenery->tab)[2], (tabBlocs->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.x - shifting <= (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx) &&
          ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx > (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx)
       ) {
         for (j = 0; j < tabBlocs->number; j++) {
           if ( j != i && conflictY((tabBlocs->tab)[j], (tabScenery->tab)[2]) &&
              ((tabScenery->tab)[2].position.x - shifting <= (tabBlocs->tab)[j].position.x + (tabBlocs->tab)[j].dimx) &&
              ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx > (tabBlocs->tab)[j].position.x + (tabBlocs->tab)[j].dimx))
           {                                // If the robot is colliding with two blocs at the same time (blocs i and j)
             (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx;
             return;
           }
           if (j != i && conflictX((tabBlocs->tab)[i], (tabBlocs->tab)[j]) && conflictY((tabBlocs->tab)[i], (tabBlocs->tab)[j])) {
             (tabBlocs->tab)[i].position.x = (tabBlocs->tab)[j].position.x + (tabBlocs->tab)[j].dimx;
             (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx;
             gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
             return;
           }
         }
         for (j = 1; j < tabScenery->number; j++) {
           if (j != 2 && conflictY((tabScenery->tab)[2], (tabScenery->tab)[j]) &&
              ((tabScenery->tab)[2].position.x - shifting <= (tabScenery->tab)[j].position.x + (tabScenery->tab)[j].dimx) &&
              ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx > (tabScenery->tab)[j].position.x + (tabScenery->tab)[j].dimx)
            ) {
              (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx;
              return;
            }
           if (j != 2 && conflictX((tabBlocs->tab)[i], (tabScenery->tab)[j]) && conflictY((tabBlocs->tab)[i], (tabScenery->tab)[j])) {
             (tabBlocs->tab)[i].position.x = (tabScenery->tab)[j].position.x + (tabScenery->tab)[j].dimx + 2;
             (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x + (tabBlocs->tab)[i].dimx;
             gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
             return;
           }
         }
         (tabScenery->tab)[2].position.x -= 3;
         (tabBlocs->tab)[i].position.x -= 3;
         gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
         return;
       }
  }
  for (i = 0; i < tabScenery->number; i++) {
    if ( i != 0 && i != 2 && (conflictY((tabScenery->tab)[2], (tabScenery->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.x - shifting <= (tabScenery->tab)[i].position.x + (tabScenery->tab)[i].dimx) &&
          ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx > (tabScenery->tab)[i].position.x + (tabScenery->tab)[i].dimx)
       ) {
         (tabScenery->tab)[2].position.x = (tabScenery->tab)[i].position.x + (tabScenery->tab)[i].dimx;
         return;
       }
  }
  (tabScenery->tab)[2].position.x -= shifting;
  return;
}




// Checks if there is a collision between hitboxes along X axis and with robot shifting to the right direction
// Update current robot position and blocks position
void hitboxRightX(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int shifting) {
  int i, j;
  for (i = 0; i < tabBlocs->number; i++) {
    if (  (conflictY((tabScenery->tab)[2], (tabBlocs->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx + shifting >= (tabBlocs->tab)[i].position.x) &&
          ((tabScenery->tab)[2].position.x < (tabBlocs->tab)[i].position.x)
       ) {
         for (j = 0; j < tabBlocs->number; j++) {
           if ( j != i && conflictY((tabScenery->tab)[2], (tabBlocs->tab)[j]) &&
                 ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx + shifting >= (tabBlocs->tab)[j].position.x) &&
                 ((tabScenery->tab)[2].position.x < (tabBlocs->tab)[j].position.x)
              ) {         // If the robot is colliding with two blocs at the same time (blocs i and j)
                (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x - (tabScenery->tab)[2].dimx;
                return;
              }
           if (j != i && conflictX((tabBlocs->tab)[i], (tabBlocs->tab)[j]) && conflictY((tabBlocs->tab)[i], (tabBlocs->tab)[j]) ) {
             (tabBlocs->tab)[i].position.x = (tabBlocs->tab)[j].position.x - (tabBlocs->tab)[i].dimx;
             (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x - (tabScenery->tab)[2].dimx;
             gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
             return;
           }
         }
         for (j = 1; j < tabScenery->number; j++) {
           if (j != 2 && conflictY((tabScenery->tab)[2], (tabScenery->tab)[j]) &&
              ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx + shifting >= (tabScenery->tab)[j].position.x) &&
              ((tabScenery->tab)[2].position.x < (tabScenery->tab)[j].position.x)
            ) {       // if the robot is colliding with a bloc and a scenaristic element at the same time
              (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x - (tabScenery->tab)[2].dimx;
              return;
            }
           if (j != 2 && conflictX((tabBlocs->tab)[i], (tabScenery->tab)[j]) && conflictY((tabBlocs->tab)[i], (tabScenery->tab)[j])) {
             (tabBlocs->tab)[i].position.x = (tabScenery->tab)[j].position.x - (tabBlocs->tab)[j].dimx - 2;
             (tabScenery->tab)[2].position.x = (tabBlocs->tab)[i].position.x - (tabScenery->tab)[2].dimx;
             gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
             return;
           }
         }
         (tabScenery->tab)[2].position.x += 3;
         (tabBlocs->tab)[i].position.x += 3;
         gravityBlocks(tabBlocs, tabScenery, tabButtons, 4);
         return;
       }
  }
  for (i = 0; i < tabScenery->number; i++) {
    if ( i != 0 && i != 2 && (conflictY((tabScenery->tab)[2], (tabScenery->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.x + (tabScenery->tab)[2].dimx + shifting >= (tabScenery->tab)[i].position.x) &&
          ((tabScenery->tab)[2].position.x < (tabScenery->tab)[i].position.x)
       ) {
         (tabScenery->tab)[2].position.x = (tabScenery->tab)[i].position.x - (tabScenery->tab)[2].dimx;
         return;
       }
  }
  (tabScenery->tab)[2].position.x += shifting;
  return;
}




// Checks if there is a collision between hitboxes along Y axis and with robot shifting to the down direction
// Update current robot position
void hitbox_DownY(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int shifting) {
  int i;
  if ((tabScenery->tab)[2].position.y + (tabScenery->tab)[2].dimy + shifting >= HDIMGAME) {
    (tabScenery->tab)[2].position.y = HDIMGAME + (tabScenery->tab)[2].dimy;
    return;
  }
  for (i = 0; i < tabBlocs->number; i++) {
    if (  (conflictX((tabScenery->tab)[2], (tabBlocs->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.y + (tabScenery->tab)[2].dimy + shifting >= (tabBlocs->tab)[i].position.y) &&
          ((tabScenery->tab)[2].position.y + (tabScenery->tab)[2].dimy + shifting <= (tabBlocs->tab)[i].position.y + (tabBlocs->tab)[i].dimy)
       ) {
         (tabScenery->tab)[2].position.y = (tabBlocs->tab)[i].position.y - (tabScenery->tab)[2].dimy - 1;
         return;
       }
  }
  for (i = 1; i < tabScenery->number; i++) {
    if ( i != 2 && (conflictX((tabScenery->tab)[2], (tabScenery->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.y + (tabScenery->tab)[2].dimy + shifting >= (tabScenery->tab)[i].position.y) &&
          ((tabScenery->tab)[2].position.y + (tabScenery->tab)[2].dimy + shifting <= (tabScenery->tab)[i].position.y + (tabScenery->tab)[i].dimy)
       ) {
         (tabScenery->tab)[2].position.y = (tabScenery->tab)[i].position.y - (tabScenery->tab)[2].dimy - 1;
         return;
       }
  }
  (tabScenery->tab)[2].position.y += shifting;
  return;
}



// Checks if there is a collision between hitboxes along Y axis and with robot shifting to the up direction
// Update current robot position
void hitbox_UpY(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int shifting) {
  int i;
  if ((tabScenery->tab)[2].position.y - shifting <= 0) {
    (tabScenery->tab)[2].position.y = 0;
    return;
  }
  for (i = 0; i < tabBlocs->number; i++) {
    if (  (conflictX((tabScenery->tab)[2], (tabBlocs->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.y - shifting <= (tabBlocs->tab)[i].position.y + (tabBlocs->tab)[i].dimy) &&
          ((tabScenery->tab)[2].position.y - shifting >= (tabBlocs->tab)[i].position.y)
       ) {
         (tabScenery->tab)[2].position.y = (tabBlocs->tab)[i].position.y + (tabBlocs->tab)[i].dimy;
         return;
       }
  }
  for (i = 1; i < tabScenery->number; i++) {
    if ( i != 2 && (conflictX((tabScenery->tab)[2], (tabScenery->tab)[i]) == 1) &&
          ((tabScenery->tab)[2].position.y - shifting <= (tabScenery->tab)[i].position.y + (tabScenery->tab)[i].dimy) &&
          ((tabScenery->tab)[2].position.y - shifting >= (tabScenery->tab)[i].position.y)
       ) {
         (tabScenery->tab)[2].position.y = (tabScenery->tab)[i].position.y + (tabScenery->tab)[i].dimy;
         return;
       }
  }
  (tabScenery->tab)[2].position.y -= shifting;
  return;
}




// Allows the player to move downwards if it is possible
void jumpDownAnimation(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, char** inputKey) {
  SDL_Event event;
  int code, j; int i = 0; int previousy = -10;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 41, 70};    // To make the jump more harmonic (reverse square function)
  while (previousy != ((tabScenery->tab)[2]).position.y) {
    previousy = ((tabScenery->tab)[2]).position.y;
    hitbox_DownY(tabBlocs, tabScenery, tabButtons, 4);

    if (i < 24)
      i++;

    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      hitboxLeftX(tabBlocs, tabScenery, tabButtons, 4);
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      hitboxRightX(tabBlocs, tabScenery, tabButtons, 4);
    }
    updateWindow(tabBlocs, tabScenery, tabButtons);
    SDL_Delay(time[24-i]);
  }
}





// Allows the player to move downwards if it is possible, until a hard surface is encountered
void jumpUpAnimation(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, char** inputKey) {
  SDL_Event event;
  int i, code;
  int time[25] = {10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 16, 16, 17, 18, 20, 21, 24, 27, 32, 41, 70};    // To make the jump more harmonic (reverse square function)
  for (i = 0; i < 25; i++) {
    hitbox_UpY(tabBlocs, tabScenery, tabButtons, 4);
    inputEvent(inputKey);
    if ((*inputKey)[SDLK_ESCAPE] == 1)
      return;
    if ((*inputKey)[SDLK_LEFT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      hitboxLeftX(tabBlocs, tabScenery, tabButtons, 4);
    }
    if ((*inputKey)[SDLK_RIGHT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      hitboxRightX(tabBlocs, tabScenery, tabButtons, 4);
    }
    updateWindow(tabBlocs, tabScenery, tabButtons);
    SDL_Delay(time[i]);
  }
}




// Principal player control code
// Return ERRORVALUE if an error has occured, 0 otherwise
int playGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons) {
  SDL_Event event;
  char* inputKey;
  int code;
  inputKey = calloc(SDLK_LAST, sizeof(*inputKey));
  if (inputKey == NULL) {
    errorMSG("Error while allocating memory to the key array");
    return ERRORVALUE;
  }
  int continuer = 1;

  ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_right.bmp");            // Load the robot surface oriented to the right
  updateWindow(tabBlocs, tabScenery, tabButtons);

  SDL_EnableKeyRepeat(10, 10);      // Enable a new event every 10 ms if a key remains pressed
  while(continuer) {
    inputEvent(&inputKey);
    if (inputKey[SDLK_ESCAPE] == 1)
      continuer = 0;
    if (inputKey[SDLK_SPACE] == 1) {
      jumpUpAnimation(tabBlocs, tabScenery, tabButtons, &inputKey);
      jumpDownAnimation(tabBlocs, tabScenery, tabButtons, &inputKey);
    }
    if (inputKey[SDLK_LEFT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_left.bmp");   // Load the robot surface oriented to the left
      hitboxLeftX(tabBlocs, tabScenery, tabButtons, 6);
      jumpDownAnimation(tabBlocs, tabScenery, tabButtons, &inputKey);
    }
    if (inputKey[SDLK_RIGHT] == 1) {
      ((tabScenery->tab)[2]).surface = SDL_LoadBMP("img/robot_right.bmp");   // Load the robot surface oriented to the right
      hitboxRightX(tabBlocs, tabScenery, tabButtons, 6);
      jumpDownAnimation(tabBlocs, tabScenery, tabButtons, &inputKey);
    }
    updateWindow(tabBlocs, tabScenery, tabButtons);

  }
  free(inputKey);
  return 0;
}
