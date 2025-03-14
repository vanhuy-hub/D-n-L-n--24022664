
#include"commonfunction.h"
#include<iostream>
using namespace std;
vector <SDL_Rect>Bullet_main;
bool init(){
if(SDL_Init(SDL_INIT_VIDEO)<0){
    cout<<"khoi tao SDL that bai";
    return false;
}
if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    return false;
}
g_window=SDL_CreateWindow("huy ngu code",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,600,SDL_WINDOW_SHOWN);
if(g_window==NULL)return false;

g_screen=SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);

if(g_screen==NULL)return false;

return true;
}


SDL_Texture *loadTexture(string path,SDL_Renderer *renderer){
SDL_Surface *loadSurface=IMG_Load(path.c_str());
SDL_SetColorKey(loadSurface,SDL_TRUE,SDL_MapRGB(loadSurface->format,COLOR_KEY_R,COLOR_KEY_G,COLOR_KEY_B));
SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,loadSurface);
SDL_FreeSurface(loadSurface);
return texture;
}



int main(int argc,char * argv[]){
if(!init())return -1;
SDL_Texture *texture_bk=loadTexture("image/bk2.png",g_screen);
//set up vi tri cua background va phan anh lay tu background
int x_bk=0;
SDL_Rect srcrect_bk={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


SDL_Texture *texture_main,*texture_main_not_bullet,*texture_main_bullet;
 texture_main_not_bullet=loadTexture("image/plane_fly.png",g_screen);
 texture_main_bullet=loadTexture("image/plane_fly_bullet.png",g_screen);
 texture_main=texture_main_not_bullet;

if(texture_bk==NULL||texture_main==NULL)return -1;

//lay kick thuoc anh main


int x_main=FIRST_X_POS_PLANE,y_main=FIRST_Y_POS_PLANE;
//set up vi tri cua may bay
SDL_Rect dstrect_plane={x_main,y_main,PLANE_WIDTH,PLANE_HEIGHT};

//lop đạn
SDL_Texture *texture_Bullet=loadTexture("image/sphere.png",g_screen);

//vong lap game
bool quit=false;

while(!quit){
  SDL_SetRenderDrawColor(g_screen,255,255,255,255);
  SDL_RenderClear(g_screen);

  srcrect_bk.x=x_bk;
  SDL_RenderCopy(g_screen,texture_bk,&srcrect_bk,NULL);

  dstrect_plane.x=x_main;
  dstrect_plane.y=y_main;

  SDL_RenderCopy(g_screen,texture_main,NULL,&dstrect_plane);


  for (int i = Bullet_main.size() - 1; i >= 0; i--) {
    Bullet_main[i].x += speed_bullet;

    if (Bullet_main[i].x >= SCREEN_WIDTH) {
        Bullet_main.erase(Bullet_main.begin() + i);  //
    } else {
        SDL_RenderCopy(g_screen, texture_Bullet, NULL, &Bullet_main[i]);
    }
}


     SDL_RenderPresent(g_screen);
    SDL_Delay(1);//tranh cpu chay qua nhanh va giam toc do moi khung hinh
    while(SDL_PollEvent(&g_event)!=0){
        if(g_event.type==SDL_QUIT)quit=true;

        else {
            //xử lí lên xuống trái phải
            if(g_event.type==SDL_KEYDOWN){
                switch (g_event.key.keysym.sym){

                case SDLK_UP:
                    if(y_main-speed_plane>0)
                        y_main-=speed_plane;
                    break;
                case SDLK_DOWN:
                    if(y_main+speed_plane<SCREEN_HEIGHT-150)
                        y_main+=speed_plane;
                    break;
                case SDLK_RIGHT:
                    if(x_main+speed_plane<SCREEN_WIDTH-MAX_X_PLANE)
                        x_main+=speed_plane;
                    if(x_bk+speed_map<=MAP_WIDTH-SCREEN_WIDTH)
                        x_bk+=speed_map;
                    break;
                case SDLK_LEFT:
                    if(x_main-speed_plane>MIN_X_PLANE)
                        x_main-=speed_plane;
                    if(x_bk-speed_map>=0)
                        if(x_main)
                        x_bk-=speed_map;
                        ;break;
                case SDLK_ESCAPE:quit=true;break;
                }
            }
            //xử lí sự kiện nhấp chuột phải và nhả chuột phải
            if(g_event.type==SDL_MOUSEBUTTONDOWN){
               if (g_event.button.button == SDL_BUTTON_RIGHT ) {
                    texture_main = texture_main_bullet;
                    //ktao dan moi
                    SDL_Rect newBullet;
                    // Thiết lập vị trí ban đầu cho viên đạn
                    newBullet.x=dstrect_plane.x+50;
                    newBullet.y=dstrect_plane.y+50;
                    newBullet.w=BULLET_WIDTH;
                    newBullet.h=BULLET_HEIGHT;
                    Bullet_main.push_back(newBullet);
                }
            }
            else {
                 texture_main=texture_main_not_bullet;
            }
        }
    }

}
SDL_DestroyTexture(texture_bk);
SDL_DestroyTexture(texture_main);
SDL_DestroyRenderer(g_screen);
SDL_DestroyWindow(g_window);
SDL_Quit();
return 0;
}


