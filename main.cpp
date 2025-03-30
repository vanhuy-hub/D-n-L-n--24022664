#include "commonfunction.h"
#include"commonfunction.cpp"
#include <iostream>
using namespace std;
//cac bien toan cuc
 bool quit = false;
 bool paused=true;
int numberkill=0;
 SDL_Texture* texture_plane_normal = NULL;
 SDL_Texture* texture_plane_fight = NULL;
//..
//cac vector dan ,enemy
vector<SDL_Rect>bullets;
vector<SDL_Rect>enemys;

//...

// Hàm khởi tạo SDL và SDL_Image va SDL_ttf va SDL_mixer
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        return false;
    }
    if(TTF_Init()==-1){
        return false;
    }
    g_window = SDL_CreateWindow("huy ngu code", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        return false;
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL) {
        return false;
    }
    font=TTF_OpenFont("font/arial.ttf",30);
    if(font==NULL){
        return false;
    }
    return true;
}

// Hàm load texture từ ảnh
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer) {
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == NULL) {
        cout << "Không thể tải ảnh " << path << " Lỗi: " << IMG_GetError() << endl;
        return NULL;
    }

    // Color Key để xóa nền
    SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return texture;
}
//...


//...
// Cấu trúc background
struct BackGround {
    SDL_Texture* texture;
    SDL_Rect rect;

    BackGround() {
        texture = NULL;
        rect={0,0,0,0};
    }

};
BackGround Map;
BackGround Bullet;
BackGround Continue;
BackGround Restart ;
BackGround Point;
BackGround StartText;
//...

//cau truc nguoi choi
struct Player {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool isFighted;
    bool isDie;
    bool moveUp,moveDown,moveRight,moveLeft;
    Player() {
        texture = NULL;
        rect = {0, 200, PLANE_WIDTH, PLANE_HEIGHT};
        isFighted = false;
        isDie = false;
        moveUp=moveDown=moveRight=moveLeft=false;
    }
};
Player Main;

//xu li nguoi choi di chuyen

void EventPlayer(){
if(Main.moveDown==true){
        Main.rect.y+=speed_plane;
}
if(Main.moveUp==true){
        Main.rect.y-=speed_plane;
}
if(Main.moveLeft==true){
    Main.rect.x-=speed_plane;
}
if(Main.moveRight==true){
    Main.rect.x+=speed_plane;
}
if(Main.rect.x<0){
    Main.rect.x=0;
}
if(Main.rect.x>600){
    Main.rect.x=600;
    Map.rect.x+=speed_map;//xu li map di chuyen theo nhan vat
    if( Map.rect.x>3600){
        Map.rect.x=3600;
    }
}
if(Main.rect.y>=470){
    Main.rect.y=470;
}
if(Main.rect.y<0){
    Main.rect.y=0;
}
if(Main.isFighted){
    Main.texture=texture_plane_fight;
}
else Main.texture=texture_plane_normal;

}
//...

//xu li su kien ban phim va chuot
void HandlingEvent() {
    while (SDL_PollEvent(&g_event)) {
    if (g_event.type == SDL_QUIT) {
            quit = true;

        }
    else if(g_event.type==SDL_KEYDOWN){
            switch(g_event.key.keysym.sym){
        case SDLK_UP:
            Main.moveUp=true;break;
        case SDLK_DOWN:
            Main.moveDown=true;break;
        case SDLK_LEFT:
           Main.moveLeft=true;break;
        case SDLK_RIGHT:
            Main.moveRight=true;break;
        case SDLK_SPACE:
            paused=!paused;
            break;
        case SDLK_z:
            Main.isFighted=true;
            BackGround newBullet;
            newBullet.rect={Main.rect.x,Main.rect.y,BULLET_WIDTH,BULLET_HEIGHT};
            if(bullets.size()<=number_bullet_main){
            bullets.push_back(newBullet.rect);
            }
            break;
            }
    }
    else if(g_event.type==SDL_KEYUP&&g_event.key.repeat==0){
        switch(g_event.key.keysym.sym){
        case SDLK_UP:
            Main.moveUp=false;break;
        case SDLK_DOWN:
            Main.moveDown=false;break;
        case SDLK_LEFT:
            Main.moveLeft=false;break;
        case SDLK_RIGHT:
            Main.moveRight=false;break;
         case SDLK_z:
            Main.isFighted=false;break;
            }
    }


    }

         if(!paused)
         EventPlayer();
        }

//...
//xu li dan cua main
void DrawBullet(){
if(!paused){
if(bullets.empty())return;
else {
    for(int i=bullets.size()-1;i>=0;i--){
        bullets[i].x+=speed_bullet;
        if(bullets[i].x>=SCREEN_WIDTH){
            bullets.erase(bullets.begin()+i);
        }
        else {
            SDL_RenderCopy(g_screen,Bullet.texture,NULL,&bullets[i]);
        }
    }
}
}
}
//...

void DrawRenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
//ve menu
void DrawMenu(){
   // SDL_RenderCopy(g_screen,Point.texture,NULL,&Point.rect);

     DrawRenderText(g_screen,font,"Kill:   /100",0,0,white);
     DrawRenderText(g_screen,font,"Time:    s",0,30,white);
     //DrawRenderText(g_screen,font,NumberKilled,0,0,white);
     DrawRenderText(g_screen,font,"Highest:    s",0,60,black);

}
// Hàm vẽ texture lên cửa sổ
void DrawOnWindow() {
    SDL_RenderClear(g_screen);
    SDL_RenderCopy(g_screen, Map.texture, &Map.rect, NULL);
     SDL_RenderCopy(g_screen, Main.texture, NULL, &Main.rect);
    DrawMenu();
    DrawBullet();
    SDL_RenderPresent(g_screen);
}
//...

// Hàm load texture ban dau cua game
void first_state(){
 Map.texture = IMG_LoadTexture(g_screen, "image/bk2.png");
    Map.rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    if (Map.texture == NULL) {
        cout << "Không thể tải background!" << endl;

    }
  Continue.texture=IMG_LoadTexture(g_screen,"image/continueButton.png");
  Continue.rect={0,0,CONTINUE_WIDTH,CONTINUE_HEIGHT};

  Restart.texture=IMG_LoadTexture(g_screen,"image/playAgainButton.png");
  Restart.rect={80,0,CONTINUE_WIDTH,CONTINUE_HEIGHT};

   Point.texture=IMG_LoadTexture(g_screen,"image/scoreBoard.png");
   Point.rect={0,0,POINT_WIDTH,POINT_HEIGHT};

    texture_plane_normal=loadTexture("image/plane_fly.png",g_screen);
    texture_plane_fight=loadTexture("image/plane_fly_bullet.png",g_screen);
    if(texture_plane_fight==NULL||texture_plane_normal==NULL){
        cout<<"loi tai texture plane"<<endl;
    }

    Bullet.texture=loadTexture("image/sphere2.png",g_screen);
    if(Bullet.texture==NULL){
        cout<<"loi tai anh dan ";
    }
}
//...


// Hàm giải phóng tài nguyên
void close() {
    SDL_DestroyTexture(Map.texture);
    SDL_DestroyTexture(Main.texture);
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}
//...


// Main
int main(int argc, char* argv[]) {
    if (!init()) return -1;
    first_state();
    while (!quit) {
        HandlingEvent();
        DrawOnWindow();
        SDL_Delay(1);
    }
    close();
    return 0;
}
