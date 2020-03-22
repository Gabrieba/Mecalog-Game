#include <stdio.h>
#include <SDL/SDL.h>


#define ENDVALUE 0
#define ERRORVALUE -1

#define XDIMGAME 1150     // Horizontal dimension of the global window
#define HDIMGAME 650      // Vertical dimension of the global window

#define STYLE_BOLD    0x01
#define STYLE_OFF     0x00
#define COLOR_RED     31
#define COLOR_YELLOW  33
#define COLOR_BLUE    36

#define XROBOT 38
#define HROBOT 97
#define XBOX   67
#define HBOX   70



typedef struct {  SDL_Surface* surface;
                  SDL_Rect position;
               } sprite;


typedef struct{ sprite game;
                sprite floor;
                sprite robot;
                sprite box;
              } tabsprite;




// Function prototypes main.h
void errorMSG(char* msg);
void warningMSG(char* msg);
void infoMSG(char* msg);
int initGame(tabsprite* tab);
void updateWindow(tabsprite* tab);
int welcomeGame(tabsprite* tab);
void finishGame(tabsprite* tab);


// Function prototypes welcome.h
int welcomeGame(tabsprite* tab);


// Function prototypes play.h
void pauseSDL(void);
void inputEvent(char** keytab);
int conflictX(sprite anime, sprite object);
int conflictY(sprite anime, sprite object);
void hitboxLeftX(tabsprite* tab, int shifting);
void hitboxRightX(tabsprite* tab, int shifting);
void hitbox_DownY(tabsprite* tab, int shifting);
void hitbox_UpY(tabsprite* tab, int shifting);
int playGame(tabsprite* tab);
void jumpUpAnimation(tabsprite* tab, char** inputKey);
void jumpDownAnimation(tabsprite* tab, char** inputKey);
