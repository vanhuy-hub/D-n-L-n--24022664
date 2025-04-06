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
#include<iostream>
#include<cstring>
#include<fstream>
static SDL_Window* g_window=NULL;
static SDL_Renderer *g_screen=NULL;
static TTF_Font *font=NULL;
static SDL_Event g_event ;

//các biến dười để dùng trong kĩ thuật color key
const int COLOR_KEY_R =0 ;
const int COLOR_KEY_G =255 ;
const int COLOR_KEY_B =255 ;

const int SCREEN_WIDTH=1200;//chieu rong man hinh
const int SCREEN_HEIGHT=600;//chieu dai man hinh

const int MAP_WIDTH=4800;
const int MAP_HEIGHT=600;


const int PLANE_WIDTH=80;
const int PLANE_HEIGHT=60;
const int FIRST_X_POS_PLANE=0;
const int FIRST_Y_POS_PLANE=0;
const int MIN_X_PLANE=100;
const int MAX_X_PLANE=850;

const int CONTINUE_WIDTH=80;
const int CONTINUE_HEIGHT=80;
const int QUIT_WIDTH=200;
const int QUIT_HEIGHT=88;

const double speed_enemy=1;
const int number_bullet_main=1;


const double speed_plane=1;
const int speed_map=1;


const int BULLET_WIDTH=15;
const int BULLET_HEIGHT=15;
const int speed_bullet=2;

const int EXP_WIDTH=165 ;
const int EXP_HEIGHT=165 ;

const int numberMaxEnemy=30;


const int speed_blood=5;
const int frameDelay = 150 ;


const SDL_Color white={255,255,255};
const SDL_Color black={0,0,0};
const SDL_Color red={185,0,0};
#endif // COMMON_FUNCTION_H_
