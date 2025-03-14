#ifndef COMMON_FUNCTION_H_//định nghĩa thư viên
#define COMMON_FUNCTION_H_

#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<windows.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<vector>

//extern SDL_Renderer *g_renderer;//ve anh
static SDL_Window* g_window=NULL;
static SDL_Renderer *g_screen=NULL;
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
const int PLANE_HEIGHT=80;
const int FIRST_X_POS_PLANE=0;
const int FIRST_Y_POS_PLANE=0;
const int MIN_X_PLANE=200;
const int MAX_X_PLANE=600;

const int speed_plane=5;
const int speed_map=10;

const int BULLET_WIDTH=20;
const int BULLET_HEIGHT=20;
const int speed_bullet=1;
#endif // COMMON_FUNCTION_H_
