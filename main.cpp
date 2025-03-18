
#include"commonfunction.h"
#include<iostream>
using namespace std;
vector <SDL_Rect>Bullet_main;
vector<SDL_Rect>Enemy;
vector<SDL_Rect>Exp_enemy;
vector <Explosion>Explosions ;
vector<Explosion>Explosions_main;
Uint32 lastFrameTime = 0;

bool  paused=false;//stop khi space
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


SDL_Texture *texture_main,*texture_main_not_bullet,*texture_main_bullet,*texture_bk_main;
 texture_main_not_bullet=loadTexture("image/plane_fly.png",g_screen);
 texture_main_bullet=loadTexture("image/plane_fly_bullet.png",g_screen);
 texture_bk_main=loadTexture("image/bk_main.png",g_screen);
 texture_main=texture_main_not_bullet;
bool check_enemy=false;
if(texture_bk==NULL||texture_main==NULL)return -1;

//lay kick thuoc anh main


int x_main=FIRST_X_POS_PLANE,y_main=FIRST_Y_POS_PLANE;
//set up vi tri cua may bay
SDL_Rect dstrect_plane={x_main,y_main,PLANE_WIDTH,PLANE_HEIGHT};

//lop ke thu
SDL_Texture* texture_enemy1=loadTexture("image/af1.png",g_screen);
SDL_Texture * texture_new_enemy=loadTexture("image/new_enemy.png",g_screen);
SDL_Texture *texture_enemy=texture_enemy1;
int number_enemy=3;
int PLANE_ENEMY_WIDTH=90;
int PLANE_ENEMY_HEIGHT=50;
bool check_new_enemy=true;
//lop đạn
SDL_Texture *texture_Bullet=loadTexture("image/sphere2.png",g_screen);
srand(time(0));

//texture exp enemy
SDL_Texture *texture_exp_enemy=loadTexture("image/exp.png",g_screen);

//texture boss
SDL_Texture *texture_boss1=loadTexture("image/boss2.png",g_screen);
SDL_Texture *texture_boss=texture_boss1;
SDL_Rect dstrect_boss={1200,80,BOSS_WIDTH,BOSS_HEIGHT};
bool check_boss_die=false;
int STEP_BOSS=1;

//thanh blood
SDL_Rect blood_main={0,0,MAX_BLOOD,10};
SDL_Rect blood_boss={0,0,MAX_BLOOD*3,20};
bool check_die=false;
//exp _main
SDL_Texture * texture_exp_main=loadTexture("image/exp_main.png",g_screen);


//vong lap game
bool quit=false;

while(!quit){
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
                    if(y_main+speed_plane<SCREEN_HEIGHT-130)
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
                    if(x_bk-speed_map>=0&&x_bk<3400)

                        x_bk-=speed_map;
                        ;break;
            case SDLK_SPACE:
                    paused=!paused;
                    break;

            case SDLK_r:
                  dstrect_plane.x = 0;
                  dstrect_plane.y = 0;
                  x_bk=0;
    // Reset máu
           blood_main.w = MAX_BLOOD;
           blood_boss.w=MAX_BLOOD*3;
         texture_main=texture_main_not_bullet;
         texture_boss=texture_boss1;
         texture_enemy=texture_enemy1;
         PLANE_ENEMY_WIDTH=90;
         PLANE_ENEMY_HEIGHT=50;
         check_new_enemy=true;//danh dau dan co the pha may bay dich
         number_enemy=3;
    // Xóa danh sách kẻ địch và vụ nổ
    Enemy.clear();
    Explosions_main.clear();
    Explosions.clear();
    Bullet_main.clear();
    // Reset biến trạng thái
    check_die = false;
    check_boss_die=false;
    paused = false;
    break;
            }
            }
            //xử lí sự kiện nhấp chuột phải và nhả chuột phải
            if(g_event.type==SDL_MOUSEBUTTONDOWN){
               if (g_event.button.button == SDL_BUTTON_RIGHT ) {
                    texture_main = texture_main_bullet;
                    //ktao dan moi
                    SDL_Rect newBullet;
                    // Thiết lập vị trí ban đầu cho viên đạn
                    newBullet.x=dstrect_plane.x+40;
                    newBullet.y=dstrect_plane.y+37;
                    newBullet.w=BULLET_WIDTH;
                    newBullet.h=BULLET_HEIGHT;
                    if(Bullet_main.size()<4 )
                    Bullet_main.push_back(newBullet);
                }
            }
            else {
                 texture_main=texture_main_not_bullet;
            }
        }
    }

 if(paused)continue;
  SDL_SetRenderDrawColor(g_screen, 209, 25,32 , 255);
    SDL_RenderClear(g_screen);

    // Vẽ background
    srcrect_bk.x = x_bk;
    SDL_RenderCopy(g_screen, texture_bk, &srcrect_bk, NULL);

       //xu li khi main va phai enemy
   if(check_enemy){
    texture_main=texture_bk_main;
    SDL_Delay(0);
    check_enemy=false;
   }
    // Vẽ máy bay chính
    dstrect_plane.x = x_main;
    dstrect_plane.y = y_main;
    SDL_RenderCopy(g_screen, texture_main, NULL, &dstrect_plane);

    //ve thanh mau
    blood_main.x=dstrect_plane.x+15;
    blood_main.y=dstrect_plane.y-15;
    SDL_RenderFillRect(g_screen,&blood_main);

    blood_boss.x=dstrect_boss.x+30;
    blood_boss.y=dstrect_boss.y-30;
    if(check_boss_die==true){
        paused=true;
        cout<<"you win";
    }



    // Xử lý đạn

    for (int i = Bullet_main.size() - 1; i >= 0; i--) {
        Bullet_main[i].x += speed_bullet;
     if(check_new_enemy){//kiem tra neu la dich thuong thi moi va vao dan cua main
        for (int j = Enemy.size() - 1; j >= 0; j--) {
            if (Bullet_main[i].x <= Enemy[j].x + PLANE_ENEMY_WIDTH &&
                Bullet_main[i].x + BULLET_WIDTH >= Enemy[j].x &&
                Bullet_main[i].y <= Enemy[j].y + PLANE_ENEMY_HEIGHT &&
                Bullet_main[i].y + BULLET_HEIGHT >= Enemy[j].y) {

                // Tạo vụ nổ
                Explosion newExplosion;
                newExplosion.rect.x = Enemy[j].x;
                newExplosion.rect.y = Enemy[j].y;
                newExplosion.rect.w = EXP_WIDTH;
                newExplosion.rect.h = EXP_HEIGHT;
                newExplosion.frame = 0;
                Explosions.push_back(newExplosion);

                Bullet_main.erase(Bullet_main.begin() + i);
                Enemy.erase(Enemy.begin() + j);
                break;
            }
        }
     }
     else{//check dan va cham voi boss
            if(Bullet_main[i].x<=dstrect_boss.x+BOSS_WIDTH&&
               Bullet_main[i].x+BULLET_WIDTH>=dstrect_boss.x&&
               Bullet_main[i].y<=dstrect_boss.y+BOSS_HEIGHT&&
               Bullet_main[i].y+BULLET_HEIGHT>=dstrect_boss.y){
               if(blood_boss.w-3>0){
                blood_boss.w-=5;
               }
               else{
                blood_boss.w=0;
                texture_boss=texture_bk_main;
                 Explosion newExplosion;
                newExplosion.rect.x = dstrect_boss.x;
                newExplosion.rect.y = dstrect_boss.y;
                newExplosion.rect.w = EXP_WIDTH*1.5;
                newExplosion.rect.h = EXP_HEIGHT*1.5;
                newExplosion.frame = 0;
                Explosions.push_back(newExplosion);
                check_boss_die=true;

               }
                Bullet_main.erase(Bullet_main.begin() + i);

     }
     }
        if (Bullet_main[i].x >= SCREEN_WIDTH) {
            Bullet_main.erase(Bullet_main.begin() + i);
        } else {
            SDL_RenderCopy(g_screen, texture_Bullet, NULL, &Bullet_main[i]);
        }
    }

    // Hiển thị vụ nổ may bay dich
    for (int i = Explosions.size() - 1; i >= 0; i--) {
        SDL_Rect exp_src = { Explosions[i].frame * EXP_WIDTH, 0, EXP_WIDTH, EXP_HEIGHT };
        SDL_RenderCopy(g_screen, texture_exp_enemy, &exp_src, &Explosions[i].rect);
        SDL_Delay(5);
        Explosions[i].frame++;

        // Xóa vụ nổ khi hết frame
        if (Explosions[i].frame >= 4) {
            Explosions.erase(Explosions.begin() + i);
        }
    }

    //hien thi vu no main
    if(check_die){
      texture_main=texture_bk_main;
        if(Explosions_main[0].frame>0){
            SDL_Rect exp_src_main = { Explosions_main[0].frame * EXP_WIDTH, 0, EXP_WIDTH, EXP_HEIGHT };
            SDL_RenderCopy(g_screen,texture_exp_main,&exp_src_main,&Explosions_main[0].rect);
            SDL_Delay(150);
           Explosions_main[0].frame --;
        }
      else {
        paused =true;
      }
    }

//xuat hien boss
    if(srcrect_bk.x>=3600){ // Xuất hiện boss
    if(dstrect_boss.x>850){
    dstrect_boss.x-=2;}
    SDL_RenderFillRect(g_screen,&blood_boss);//ve thanh mau cho boss
    texture_enemy=texture_new_enemy;
    number_enemy=4;
     PLANE_ENEMY_WIDTH=80;
    PLANE_ENEMY_HEIGHT=80;
    check_new_enemy=false;//danh dau đạn của main ko phá được đạn của boss

    //boss di chuyen
    dstrect_boss.y+=STEP_BOSS*SPEED_BOSS;
    if(dstrect_boss.y<=10||dstrect_boss.y>=400){
        STEP_BOSS=-STEP_BOSS;
    }
    SDL_RenderCopy(g_screen, texture_boss, NULL, &dstrect_boss);

    }


    // Xử lý máy bay địch

    while (Enemy.size() < number_enemy) {
    SDL_Rect newEnemy = {SCREEN_WIDTH+ rand() % 300, rand() % (SCREEN_HEIGHT - PLANE_ENEMY_HEIGHT-130), PLANE_ENEMY_WIDTH, PLANE_ENEMY_HEIGHT};
    Enemy.push_back(newEnemy);
}
    for (int i = Enemy.size() - 1; i >= 0; i--) {
        Enemy[i].x -= speed_enemy;

        if (dstrect_plane.x <= Enemy[i].x + PLANE_ENEMY_WIDTH &&
            dstrect_plane.x + PLANE_WIDTH >= Enemy[i].x &&
            dstrect_plane.y <= Enemy[i].y + PLANE_ENEMY_HEIGHT &&
            dstrect_plane.y + PLANE_HEIGHT >= Enemy[i].y) {
            check_enemy=true;
            if(blood_main.w-13>=0){
            blood_main.w-=13;}
            else {
                blood_main.w=0;
                 Explosion new_exp_main;
                 new_exp_main.frame=3;
                 new_exp_main.rect.x=dstrect_plane.x;
                 new_exp_main.rect.y=dstrect_plane.y;
                 new_exp_main.rect.w=EXP_WIDTH;
                 new_exp_main.rect.h=EXP_HEIGHT;
                 Explosions_main.push_back(new_exp_main);
                 check_die=true;
            }
            Enemy.erase(Enemy.begin() + i);
        }

        if (Enemy[i].x < -70) {
            Enemy.erase(Enemy.begin() + i);
        } else {
            SDL_RenderCopy(g_screen, texture_enemy, NULL, &Enemy[i]);
        }
    }



    SDL_RenderPresent(g_screen);
    SDL_Delay(1);
}
SDL_DestroyTexture(texture_bk);
SDL_DestroyTexture(texture_main);
SDL_DestroyRenderer(g_screen);
SDL_DestroyWindow(g_window);
SDL_Quit();
return 0;
}

