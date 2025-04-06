#include"commonfunction.h"
class Explosion {
public :
 SDL_Texture *texture;
 SDL_Rect rect1;
 SDL_Rect rect2;
 int frameNumber;
Uint32 lastTime;
 Explosion(){
 this->texture =NULL;
 frameNumber=0;
 }

 Explosion (SDL_Texture *texture ,int x,int y,int frameNumber){
 this->frameNumber=frameNumber;
 this->texture=texture;
 rect1={0,0,EXP_WIDTH,EXP_HEIGHT};//thuc hien animation
 rect2={x,y,EXP_WIDTH,EXP_HEIGHT};//vi tri cua vu no
 }
};
