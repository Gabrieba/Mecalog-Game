#include <stdio.h>
#include <SDL/SDL.h>


#define ENDVALUE 0
#define ERRORVALUE -1

#define XDIMGAME 1150
#define HDIMGAME 650

#define STYLE_BOLD    0x01
#define STYLE_OFF     0x00
#define COLOR_RED     31
#define COLOR_YELLOW  33
#define COLOR_BLUE    36


typedef struct {  SDL_Surface* surface;
                  SDL_Rect position;
               } sprite;


typedef struct{ sprite game;
                sprite robot;
                sprite box;
              } tabsprite;




// Function prototypes main.h
void errorMSG(char* msg);
void warningMSG(char* msg);
void infoMSG(char* msg);
int initGame(tabsprite* tab);
int welcomeGame(tabsprite* tab);
void finishGame(tabsprite* tab);


// Function prototypes welcome.h
int welcomeGame(tabsprite* tab);


// Function prototypes play.h
void pauseSDL(void);
void inputEvent(char** keytab);
int playGame(tabsprite* tab);
void jumpAnimation(tabsprite* tab, char** inputKey);
