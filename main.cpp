#include "commonfunction.h"
#include"Plane.cpp"
#include"backGround.cpp"
using namespace std;
//cac bien toan cuc
 bool quit = false;
 bool paused=false ;
  int blood_main=60;
 int numberenemy=3;
int numberkill=0;
long long Time=0;
long long bestTime=0;
 SDL_Texture* texture_plane_normal = NULL;
 SDL_Texture* texture_plane_fight = NULL;
 SDL_Texture *texture_enemy1=NULL;
 SDL_Texture *texture_enemy2=NULL;
Uint32 lastTime;
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
BackGround Map;
BackGround Bullet;
BackGround Continue;
BackGround Restart ;
BackGround Quit;
BackGround StartText;
//...

//cau truc may bay main va dich
class Player:public Plane {
    public:
    SDL_Rect blood;
    bool isFighted;
    bool moveUp,moveDown,moveRight,moveLeft;
    Player() {
        texture = NULL;
        rect = {0, 200, PLANE_WIDTH, PLANE_HEIGHT};
        isFighted = false;
        isDie = false;
        moveUp=moveDown=moveRight=moveLeft=false;
    }
};
Plane Enemy;
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

//ham xu li thoi gian chay
void Timer(){
if(paused==false){
Uint32 currentTime=SDL_GetTicks();
if(currentTime-lastTime>=1000){
    Time++;
    lastTime=currentTime;
}
}
}
//...

//ham xu li restart
void restart(){
blood_main=60;
Main.isDie=false;
paused=false;
enemys.clear();
bullets.clear();
Map.rect.x=0;
Main.rect.x=0;
}
//xu li su kien ban phim va chuot
void HandlingEvent() {
    while (SDL_PollEvent(&g_event)) {
    if (g_event.type == SDL_QUIT) {
            quit = true;

        }
    else if(g_event.type==SDL_KEYDOWN){
        if(!paused){
            switch(g_event.key.keysym.sym){
        case SDLK_UP:
            Main.moveUp=true;break;
        case SDLK_DOWN:
            Main.moveDown=true;break;
        case SDLK_LEFT:
           Main.moveLeft=true;break;
        case SDLK_RIGHT:
            Main.moveRight=true;break;
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
    else if(g_event.type==SDL_MOUSEBUTTONDOWN){
        int x=g_event.button.x,y=g_event.button.y;
        if(y>=520&&y<=600){
            if(x>400&&x<480)paused=!paused;
            else if(x>500&&x<700){
                quit=true;
            }
            else if(x>700&&x<780){
                restart();
            }

        }
    }
    }

        }

//...
//ham kiem tra check var giua 2 texture
bool checkVar(SDL_Rect rect1,SDL_Rect rect2)
{
return ((rect1.x<rect2.x+rect2.w)
        &&(rect1.x+rect1.w>rect2.x)
        &&(rect1.y<rect2.y+rect2.h)
        &&(rect1.y+rect1.h>rect2.y));
}
//xu li dan cua main
void DrawBullet(){

if(bullets.empty())return;
else {
    for(int i=bullets.size()-1;i>=0;i--){
        bullets[i].x+=speed_bullet;
    //kiem tra va cham giua dan cua main va enemy
    for(int j=enemys.size()-1;j>=0;j--){
        if(checkVar(bullets[i],enemys[j])){
            bullets.erase(bullets.begin()+i);
            enemys.erase(enemys.begin()+j);
            numberkill ++;
        }
    }
        if(bullets[i].x>=SCREEN_WIDTH){
            bullets.erase(bullets.begin()+i);
        }
        else {
            SDL_RenderCopy(g_screen,Bullet.texture,NULL,&bullets[i]);
        }
    }
}

}
void DrawEnemy(){
    while(enemys.size()<numberenemy){
        SDL_Rect newEnemy={SCREEN_WIDTH+ rand() % 300, rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT-130), ENEMY_WIDTH, ENEMY_HEIGHT};
        enemys.push_back(newEnemy);
    }
    for(int i=enemys.size()-1;i>=0;i--){
        enemys[i].x-=speed_enemy;
        //kiem tra check var enemys va nhan vat va giam hp nhan vat
        if(enemys[i].x<0-ENEMY_WIDTH){
            enemys.erase(enemys.begin()+i);
        }
           if(checkVar(enemys[i],Main.rect))
           {
            if(blood_main-speed_blood>=0){
                blood_main-=speed_blood;
            }
            else {
                Main.isDie=true;
            }
             enemys.erase(enemys.begin()+i);
        }
        else {SDL_RenderCopy(g_screen,Enemy.texture,NULL,&enemys[i]);
    }
}
}
//...
//ham ve chu
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
string kill="Kill:"+to_string(numberkill)+"/"+to_string(numberMaxEnemy);
string time_current="Time:"+to_string(Time)+"s";
string best="Best Time:"+to_string(bestTime)+"s";
     DrawRenderText(g_screen,font,kill.c_str(),0,0,white);
     DrawRenderText(g_screen,font,time_current.c_str(),0,30,white);
     DrawRenderText(g_screen,font,best.c_str(),0,60,black);
     SDL_RenderCopy(g_screen,Continue.texture,NULL,&Continue.rect);
     SDL_RenderCopy(g_screen,Restart.texture,NULL,&Restart.rect);
     SDL_RenderCopy(g_screen,Quit.texture,NULL,&Quit.rect);
if(Main.isDie==true){
    DrawRenderText(g_screen,font,"YOU LOSE",500,200,red);
    paused=true;
}
}
//...
//ham ve thanh mau cua player
void DrawBloodMain(){
Main.blood={Main.rect.x+7,Main.rect.y-10,blood_main,10};
SDL_SetRenderDrawColor(g_screen,219,26,33,255);
SDL_RenderFillRect(g_screen,&Main.blood);
//neu MainDie

}
// Hàm vẽ texture lên cửa sổ
void DrawOnWindow() {
    SDL_RenderClear(g_screen);
    EventPlayer();
    Timer();
    SDL_RenderCopy(g_screen, Map.texture, &Map.rect, NULL);
     SDL_RenderCopy(g_screen, Main.texture, NULL, &Main.rect);
     DrawMenu();
    DrawBloodMain();
    DrawBullet();
    DrawEnemy();
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
  Continue.rect={400,520,CONTINUE_WIDTH,CONTINUE_HEIGHT};

  Restart.texture=IMG_LoadTexture(g_screen,"image/playAgainButton.png");
  Restart.rect={700,520,CONTINUE_WIDTH,CONTINUE_HEIGHT};

   Quit.texture=IMG_LoadTexture(g_screen,"image/exitButton.png");
   Quit.rect={500,520,QUIT_WIDTH,QUIT_HEIGHT};

    texture_plane_normal=loadTexture("image/plane_fly.png",g_screen);
    texture_plane_fight=loadTexture("image/plane_fly_bullet.png",g_screen);
    Main.texture=texture_plane_normal;
    if(texture_plane_fight==NULL||texture_plane_normal==NULL){
        cout<<"loi tai texture plane"<<endl;
    }

    Bullet.texture=loadTexture("image/sphere2.png",g_screen);
    if(Bullet.texture==NULL){
        cout<<"loi tai anh dan ";
    }
    texture_enemy1=loadTexture("image/af1.png",g_screen);
    Enemy.texture=texture_enemy1;
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
    srand(time(0));
    first_state();
    while (!quit) {
        HandlingEvent();
        if(!paused)
        DrawOnWindow();

    }
    close();
    return 0;
}
