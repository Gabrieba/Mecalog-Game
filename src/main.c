#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "../include/main.h"


void errorMSG(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_RED);   // Set to bold red letters
  printf("ERROR : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}


void warningMSG(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_YELLOW);   // Set to bold yellow letters
  printf("WARNING : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}


void infoMSG(char* msg) {
  fprintf(stdout, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE);   // Set to bold blue letters
  printf("INFO : %s\n", msg);
  fprintf(stdout, "%c[%dm", 0x1B, 0);   // Reset
}




// Initialize the variables for the game
// Return ERRORVALUE if an error has occured, 0 otherwise
int initGame(tabsprite* tab) {
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    errorMSG("Erreur à l'initialisation de la librairie SDL");
    printf("%s\n", SDL_GetError());
    return ERRORVALUE;
  }
  (tab->game).surface = SDL_SetVideoMode(XDIMGAME, HDIMGAME, 32, SDL_SWSURFACE);      // Window mode configuration
  SDL_WM_SetCaption("## Mecalog Game ##", NULL);                 // Title of the SDL window
  if ((tab->game).surface == NULL) {
    errorMSG("Erreur à l'ouverture de la fenêtre SDL");
    printf("%s\n", SDL_GetError());
    return ERRORVALUE;
  }
  (tab->floor).surface =  SDL_CreateRGBSurface(SDL_HWSURFACE, XDIMGAME, HDIMGAME-298, 32, 0, 0, 0, 0);    // Create the floor
  (tab->box).surface = NULL;
  (tab->robot).surface = NULL;
  ((tab->game).position).x = 0;
  ((tab->game).position).y = 0;
  ((tab->floor).position).x = 0;
  ((tab->floor).position).y = 298;
  ((tab->box).position).x = 165;
  ((tab->box).position).y = 227;
  SDL_FillRect((tab->game).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
  SDL_FillRect((tab->floor).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 127, 127, 127));     // Fix the parameters of the floor

  return 0;
}



void updateWindow(tabsprite* tab) {
  SDL_FillRect((tab->game).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 255, 255, 255)); // General background of the SDL window
  SDL_FillRect((tab->floor).surface, NULL, SDL_MapRGB(((tab->game).surface)->format, 127, 127, 127));     // Fix the parameters of the floor
  SDL_BlitSurface((tab->game).surface, NULL, (tab->game).surface, &((tab->game).position));       // Add the background to the SDL window
  SDL_BlitSurface((tab->floor).surface, NULL, (tab->game).surface, &((tab->floor).position));      // Add the floor to the SDL window
  SDL_BlitSurface((tab->box).surface, NULL, (tab->game).surface, &((tab->box).position));        // Add the box surface to the SDL window
  SDL_SetColorKey((tab->robot).surface, SDL_SRCCOLORKEY, SDL_MapRGB(((tab->robot).surface)->format, 255, 51, 153));
  SDL_BlitSurface((tab->robot).surface, NULL, (tab->game).surface, &((tab->robot).position));
  SDL_Flip((tab->game).surface);       // Update the game window
}





// Free the variables when the game is finished
// Return nothing
void finishGame(tabsprite* tab) {
  SDL_FreeSurface((tab->box).surface);
  SDL_FreeSurface((tab->robot).surface);
  SDL_FreeSurface((tab->floor).surface);
  SDL_FreeSurface((tab->game).surface);
  SDL_Quit();
}





// Main code of the game + error handler
int main() {
  int code;
  tabsprite tab;

  code = initGame(&tab);
  if (code < 0) {
    SDL_FreeSurface((tab.box).surface);
    SDL_FreeSurface((tab.robot).surface);
    SDL_FreeSurface((tab.floor).surface);
    SDL_FreeSurface((tab.game).surface);
    puts("An error has occured");
    exit(EXIT_FAILURE);
  }
  code = welcomeGame(&tab);
  if (code < 0) {
    puts("An error has occured");
    finishGame(&tab);
    exit(EXIT_FAILURE);
  }
  code = playGame(&tab);
  if (code < 0) {
    puts("An error has occured");
    finishGame(&tab);
    exit(EXIT_FAILURE);
  }
  finishGame(&tab);
  exit(EXIT_SUCCESS);
}
