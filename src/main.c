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
  (tab->box).surface = NULL;
  (tab->robot).surface = NULL;
  return 0;
}




// Free the variables when the game is finished
// Return nothing
void finishGame(tabsprite* tab) {
  SDL_FreeSurface((tab->box).surface);
  SDL_FreeSurface((tab->robot).surface);
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
