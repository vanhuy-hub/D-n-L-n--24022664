#ifndef COMMON_FUNCTION_H_//định nghĩa thư viên
#define COMMON_FUNCTION_H_
#include<algorithm>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<windows.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>
#include <cstdlib>  // Thư viện cho rand()
#include <ctime>    // Thư viện cho time()
//extern SDL_Renderer *g_renderer;//ve anh

static SDL_Window* g_window=NULL;
static SDL_Renderer *g_screen=NULL;
static SDL_Event g_event ;

static bool quit = false;
static bool paused=false;
//các biến dười để dùng trong kĩ thuật color key
const int COLOR_KEY_R =0 ;
const int COLOR_KEY_G =255 ;
const int COLOR_KEY_B =255 ;

const int SCREEN_WIDTH=1200;//chieu rong man hinh
const int SCREEN_HEIGHT=600;//chieu dai man hinh

const int MAP_WIDTH=4800;
const int MAP_HEIGHT=600;
const int MENU_WIDTH=500;
const int MENU_HEIGHT=300;

const int PLANE_WIDTH=80;
const int PLANE_HEIGHT=60;
const int FIRST_X_POS_PLANE=0;
const int FIRST_Y_POS_PLANE=0;
const int MIN_X_PLANE=100;
const int MAX_X_PLANE=850;


const int speed_enemy=1;



const double speed_plane=1;
const int speed_map=1;

const int BULLET_WIDTH=15;
const int BULLET_HEIGHT=15;
const int speed_bullet=2;

const int EXP_WIDTH=165 ;
const int EXP_HEIGHT=165 ;

const int BOSS_WIDTH=313;
const int BOSS_HEIGHT=163;
const int SPEED_BOSS=1;

const int MAX_BLOOD=60;
const int frameDelay = 150 ;

struct Explosion {
    SDL_Rect rect; // Vị trí vụ nổ
    int frame;     // Frame hiện tại của vụ nổ
};

struct Blood{
SDL_Rect rect;
};

#endif // COMMON_FUNCTION_H_
