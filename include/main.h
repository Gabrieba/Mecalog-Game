#include <stdio.h>
#include <SDL/SDL.h>


#define ENDVALUE 0
#define ERRORVALUE -1

#define XDIMGAME 1150     // Horizontal dimension of the global window
#define HDIMGAME 650      // Vertical dimension of the global window

#define ROBOT 2     // Position of robot sprite in scenery array
#define DOOR 1      // Postion of door sprite in scenery array
#define SCREEN 0    // Position of main SDL window sprite in scenery array

#define STYLE_BOLD    0x01
#define STYLE_OFF     0x00
#define COLOR_RED     31
#define COLOR_YELLOW  33
#define COLOR_BLUE    36


typedef struct {  SDL_Surface* surface;
                  SDL_Rect position;
                  int dimx;
                  int dimy;
                  int activated;
               } plateform;


typedef struct {  SDL_Surface* surface;
                  SDL_Rect position;
                  int dimx;
                  int dimy;
               } sprite;


typedef struct{ sprite* tab;
                int number;
              } tabsprite;

typedef struct{
              plateform* tab;
              int number;
            } tabplat;



// Function prototypes main.h
void errorMSG(char* msg);
void warningMSG(char* msg);
void infoMSG(char* msg);
int initGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons);
void updateWindow(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons);
void finishGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons);


// Function prototypes welcome.h
int welcomeGame(tabsprite* tabBlocs, tabsprite* tabScenery);


// Function prototypes play.h
void pauseSDL(void);
void inputEvent(char** keytab);
int conflictX(sprite anime, sprite object);
int conflictY(sprite anime, sprite object);
void plateformAnimation(tabsprite tabBlocs, tabplat* tabButtons);
void gravityBlocks(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons, char** inputKey, int shifting);
void hitboxLeftX(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons, char** inputKey, int shifting);
void hitboxRightX(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons, char** inputKey, int shifting);
void hitbox_DownY(tabsprite* tabBlocs, tabsprite* tabScenery, int shifting);
void hitbox_UpY(tabsprite* tabBlocs, tabsprite* tabScenery, int shifting);
int playGame(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons);
void jumpUpAnimation(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons, char** inputKey);
void jumpDownAnimation(tabsprite* tabBlocs, tabsprite* tabScenery, tabplat* tabButtons, char** inputKey);
