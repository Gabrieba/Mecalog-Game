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



// Load Data from .txt file
// Return ERRORVALUE if an error has occured, 0 otherwise
int loadData(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons, int level) {
  FILE* fp;
  char folder[25];
  char buffer[50];
  int number, positionx, positiony, dimx, dimy, i;
  if (system("ls | grep Level1") != 0 || system("ls | grep Level2") != 0 || system("ls | grep Level3") != 0) {
    errorMSG("At least a level folder missing");
    return ERRORVALUE;
  }
  if (level == 1)
    strcpy(folder, "Level1/");
  else if (level == 2)
    strcpy(folder, "Level2/");
  else if (level == 3)
    strcpy(folder, "Level3/");
  else {
    errorMSG("The requested level doesn't exist");
    return ERRORVALUE;
  }

  strcat(folder, "blocs");
  fp = fopen(folder, "r");
  if (fp == NULL) {
    errorMSG("Error when opening the data file for bloc sprites");
    return ERRORVALUE;
  }
  fgets(buffer, 50, fp);    // To read the first line
  fscanf(fp, "%d", &number);      // Number of sprites written in the file
  fgets(buffer, 2, fp);     // To read the carriage return
  tabBlocs->number = number;
  for (i = 0; i < number; i++) {
    if (fscanf(fp, "%d %d %d %d", &positionx, &positiony, &dimx, &dimy) != 4) {   // Read the data
      errorMSG("Not enough bloc sprites in data files");
      return ERRORVALUE;
    }
    fgets(buffer, 50, fp);     // To read the name of the sprite and the carriage return
    (tabBlocs->tab)[i].position.x = positionx;
    (tabBlocs->tab)[i].position.y = positiony;
    (tabBlocs->tab)[i].dimx = dimx;
    (tabBlocs->tab)[i].dimy = dimy;
  }
  fclose(fp);
  folder[7] = '\0';     // To delete the filename from the char buffer

  strcat(folder, "buttons");
  fp = fopen(folder, "r");
  if (fp == NULL) {
    errorMSG("Error when opening the data file for buttons sprites");
    return ERRORVALUE;
  }
  fgets(buffer, 50, fp);    // To read the first line
  fscanf(fp, "%d", &number);      // Number of sprites written in the file
  fgets(buffer, 2, fp);     // To read the carriage return
  tabButtons->number = number;
  for (i = 0; i < number; i++) {
    if (fscanf(fp, "%d %d %d %d", &positionx, &positiony, &dimx, &dimy) != 4) {   // Read the data
      errorMSG("Not enough button sprites in data files");
      return ERRORVALUE;
    }
    fgets(buffer, 50, fp);     // To read the name of the sprite and the carriage return
    (tabButtons->tab)[i].position.x = positionx;
    (tabButtons->tab)[i].position.y = positiony;
    (tabButtons->tab)[i].dimx = dimx;
    (tabButtons->tab)[i].dimy = dimy;
  }
  fclose(fp);
  folder[7] = '\0';     // To delete the filename from the char buffer

  strcat(folder, "scenery");
  fp = fopen(folder, "r");
  if (fp == NULL) {
    errorMSG("Error when opening the data file for scenery sprites");
    return ERRORVALUE;
  }
  fgets(buffer, 50, fp);    // To read the first line
  fscanf(fp, "%d", &number);      // Number of sprites written in the file
  fgets(buffer, 2, fp);     // To read the carriage return
  tabScenery->number = number;
  for (i = 0; i < number; i++) {
    if (fscanf(fp, "%d %d %d %d", &positionx, &positiony, &dimx, &dimy) != 4) {   // Read the data
      errorMSG("Not enough scenery sprites in data files");
      return ERRORVALUE;
    }
    fgets(buffer, 50, fp);     // To read the name of the sprite and the carriage return
    (tabScenery->tab)[i].position.x = positionx;
    (tabScenery->tab)[i].position.y = positiony;
    (tabScenery->tab)[i].dimx = dimx;
    (tabScenery->tab)[i].dimy = dimy;
  }
  fclose(fp);
  return 0;
}


// Initialize the variables for the game
// Return ERRORVALUE if an error has occured, 0 otherwise
int initGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons) {
  int i, code;
  for (i = 0; i < tabScenery->number; i++)
    ((tabScenery->tab)[i]).surface = NULL;
  for (i = 0; i < tabBlocs->number; i++)
    tabBlocs->tab[i].surface = NULL;
  for (i = 0; i < tabButtons->number; i++)
    tabButtons->tab[i].surface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    errorMSG("Error when initializing the SDL librairy");
    printf("%s\n", SDL_GetError());
    return ERRORVALUE;
  }
  ((tabScenery->tab)[0]).surface = SDL_SetVideoMode(XDIMGAME, HDIMGAME, 32, SDL_SWSURFACE);      // Window mode configuration
  SDL_WM_SetCaption("## Mecalog Game ##", NULL);                 // Title of the SDL window
  if (((tabScenery->tab)[0]).surface == NULL) {
    errorMSG("Error when opening the SDL window");
    printf("%s\n", SDL_GetError());
    return ERRORVALUE;
  }

  ((tabScenery->tab)[1]).surface =  SDL_CreateRGBSurface(SDL_HWSURFACE, ((tabScenery->tab)[1]).dimx, ((tabScenery->tab)[1]).dimy, 32, 0, 0, 0, 0);    // Create the floor
  ((tabScenery->tab)[3]).surface =  SDL_CreateRGBSurface(SDL_HWSURFACE, ((tabScenery->tab)[3]).dimx, ((tabScenery->tab)[3]).dimy, 32, 0, 0, 0, 0);    // Test
  ((tabScenery->tab)[4]).surface =  SDL_CreateRGBSurface(SDL_HWSURFACE, ((tabScenery->tab)[4]).dimx, ((tabScenery->tab)[4]).dimy, 32, 0, 0, 0, 0);    // Test

  return 0;
}




// Update the SDL window display with new parameters
void updateWindow(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons) {
  int i;
  SDL_FillRect(((tabScenery->tab)[0]).surface, NULL, SDL_MapRGB(((tabScenery->tab)[0]).surface->format, 255, 255, 255)); // General background of the SDL window
  SDL_FillRect(((tabScenery->tab)[1]).surface, NULL, SDL_MapRGB(((tabScenery->tab)[1]).surface->format, 127, 127, 127));     // Fix the parameters of the floor
  SDL_FillRect(((tabScenery->tab)[3]).surface, NULL, SDL_MapRGB(((tabScenery->tab)[3]).surface->format, 127, 127, 127));
  SDL_FillRect(((tabScenery->tab)[4]).surface, NULL, SDL_MapRGB(((tabScenery->tab)[4]).surface->format, 127, 127, 127));
  SDL_SetColorKey(((tabScenery->tab)[2]).surface, SDL_SRCCOLORKEY, SDL_MapRGB((((tabScenery->tab)[2]).surface)->format, 255, 51, 153));

  // Add scenaristic elements to the SDL window
  for (i = 0; i < tabScenery->number; i++)
      SDL_BlitSurface(((tabScenery->tab)[i]).surface, NULL, ((tabScenery->tab)[0]).surface, &(((tabScenery->tab)[i]).position));
  // Add blocs to the SDL window
  for (i = 0; i < tabBlocs->number; i++) {
      ((tabBlocs->tab)[i]).surface = SDL_LoadBMP("img/box.bmp");
      SDL_BlitSurface(((tabBlocs->tab)[i]).surface, NULL, ((tabScenery->tab)[0]).surface, &(((tabBlocs->tab)[i]).position));
  }
  // Add buttons to the SDL window
  for (i = 0; i < tabButtons->number; i++)
      SDL_BlitSurface(((tabButtons->tab)[i]).surface, NULL, ((tabScenery->tab)[0]).surface, &(((tabButtons->tab)[i]).position));

  SDL_Flip(((tabScenery->tab)[0]).surface);       // Update the game window
}





// Free the variables when the game is finished
// Return nothing
void finishGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabsprite* tabButtons) {
  int i;
  for (i = 0; i < tabBlocs->number; i++) {
    SDL_FreeSurface(((tabBlocs->tab)[i]).surface);
  }
  for (i = 0; i < tabScenery->number; i++) {
    SDL_FreeSurface(((tabScenery->tab)[i]).surface);
  }
  for (i = 0; i < tabButtons->number; i++) {
    SDL_FreeSurface(((tabButtons->tab)[i]).surface);
  }
  free(tabScenery->tab);
  free(tabBlocs->tab);
  SDL_Quit();
}





// Main code of the game + error handler
int main() {
  int code; int i;
  tabsprite tabBlocs;
  tabsprite tabScenery;
  tabsprite tabButtons;

  tabScenery.tab = calloc(10, sizeof(sprite));
  if (tabScenery.tab == NULL)
    exit(EXIT_FAILURE);
  tabBlocs.tab = calloc(10, sizeof(sprite));
  if (tabBlocs.tab == NULL) {
    free(tabScenery.tab);
    exit(EXIT_FAILURE);
  }
  code = loadData(&tabBlocs, &tabScenery, &tabButtons, 1);
  if (code < 0) {
    free(tabScenery.tab);
    free(tabBlocs.tab);
    exit(EXIT_FAILURE);
  }
  code = initGame(&tabBlocs, &tabScenery, &tabButtons);
  if (code < 0) {
    finishGame(&tabBlocs, &tabScenery, &tabButtons);
    puts("An error has occured");
    exit(EXIT_FAILURE);
  }
  code = welcomeGame(&tabBlocs, &tabScenery, &tabButtons);
  if (code < 0) {
    puts("An error has occured");
    finishGame(&tabBlocs, &tabScenery, &tabButtons);
    exit(EXIT_FAILURE);
  }
  code = playGame(&tabBlocs, &tabScenery, &tabButtons);
  if (code < 0) {
    puts("An error has occured");
    finishGame(&tabBlocs, &tabScenery, &tabButtons);
    exit(EXIT_FAILURE);
  }
  finishGame(&tabBlocs, &tabScenery, &tabButtons);
  exit(EXIT_SUCCESS);
}
