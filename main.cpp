#include "commonfunction.h"
#include"PlaneBoss.cpp"
#include"Player.cpp"
#include"backGround.cpp"
#include"Explosion.cpp"
#include"Enemy.cpp"
using namespace std;

// Hàm khởi tạo SDL và SDL_Image va SDL_ttf va SDL_mixer
bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        return false;
    }
    if(TTF_Init()==-1)
    {
        return false;
    }
    if (Mix_Init(MIX_INIT_MP3)==0)
    {
        return false;
    }
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)==-1)
    {
        return false;
    }
    g_window = SDL_CreateWindow("huy ngu code", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        return false;
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL)
    {
        return false;
    }
    font=TTF_OpenFont("font/arial_v2.ttf",30);
    if(font==NULL)
    {
        return false;
    }
    return true;
}

// Hàm load texture từ ảnh
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer)
{
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (loadSurface == NULL)
    {
        return NULL;
    }

    // Color Key để xóa nền
    SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return texture;
}
//...

//cac bien toan cuc
bool isScreenMain=false;
bool notpressPaused=false;
bool quit = false;
bool paused=false;
bool SwapBullet=true;
bool PressX=false;
bool NhapName=false;

int number_bullet_main=1;
int blood_main=60;
int energy_man=2;
int blood_boss=100;
int numberenemy=3;
int numberkill=0;
int lastBoxKill = -1;
long long Time=0;
long long bestTime=0;
static bool musicStarted = false;
bool boxDrop=false ;
int map_x=0;
SDL_Texture* texture_plane_normal = NULL;
SDL_Texture* texture_plane_fight = NULL;
SDL_Texture *texture_enemy1=NULL;
SDL_Texture *texture_enemy2=NULL;
SDL_Texture *texture_exp_main=NULL;
SDL_Texture *texture_exp_enemy=NULL;
SDL_Texture *texture_boss=NULL;
SDL_Texture *texture_bullet_boss=NULL;
SDL_Texture *texture_bullet1=NULL;
SDL_Texture *texture_bullet2=NULL;
SDL_Texture *texture_bullet3=NULL;
SDL_Texture *texture_bullet_power=NULL;
SDL_Texture *texture_box_hp=NULL;
SDL_Texture *texture_back=NULL;
Mix_Chunk *bulletSound=NULL;
Mix_Chunk *laze=NULL;
Mix_Chunk *expEnemySound=NULL;
Mix_Chunk *expMainSound=NULL;
Mix_Chunk *bulletBossSound=NULL;
Mix_Music *WinSound=nullptr;
Mix_Music *LoseSound=nullptr;
Mix_Music *FreeFireSound=nullptr;
Uint32 lastTime;
Uint32 lastTimeDie;
Uint32 lastTimeWin;
Uint32 lastBossAppear;
Uint32 lastTimeMap;
Uint32 lastTimeX;
//..
//cac vector dan ,enemy,vu no
vector<SDL_Rect>bullets;
vector<Enemy> enemys;
vector<Explosion>Exp;
vector<SDL_Rect>bulletBoss;
vector<SDL_Rect>Boxs;
vector<SDL_Texture*>Enemys;
vector<SDL_Texture*>TextureMap;
vector<int> enemyTextures;
//...

// Cac doi tuong cua class background
BackGround Map;
BackGround Bullet;
BackGround Continue;
BackGround Restart ;
BackGround Quit;
BackGround StartText;
BackGround BoxHp;
BackGround Bullet2;
BackGround Back;
BackGround BkFirst;
BackGround Name;
//...

//cau truc may bay main va dich

Player Main;
PlaneBoss  Boss;
//xu li nguoi choi di chuyen

void EventPlayer()
{
    if(Main.moveDown==true)
    {
        Main.rect.y+=speed_plane;
    }
    if(Main.moveUp==true)
    {
        Main.rect.y-=speed_plane;
    }
    if(Main.moveLeft==true)
    {
        Main.rect.x-=speed_plane;
    }
    if(Main.moveRight==true)
    {
        Main.rect.x+=speed_plane;
    }
    if(Main.rect.x<0)
    {
        Main.rect.x=0;
    }
    if(Main.rect.x>1200-50)
    {
        Main.rect.x=1200-50;
    }
    if(Main.rect.y>=470)
    {
        Main.rect.y=470;
    }
    if(Main.rect.y<0)
    {
        Main.rect.y=0;
    }
    if(Main.isFighted)
    {
        Main.texture=texture_plane_fight;
    }
    else Main.texture=texture_plane_normal;

    Uint32 current_time=SDL_GetTicks();
    if(current_time-lastTimeMap>10)
    {
        Map.rect.x++;
        lastTimeMap=current_time;
    }
}
//...

//ham xu li thoi gian chay
void Timer()
{
    if(paused==false)
    {
        Uint32 currentTime=SDL_GetTicks();
        if(currentTime-lastTime>=1000)
        {
            Time++;
            lastTime=currentTime;
        }
    }
}
//...

//ham xu li restart
void restart()
{
    blood_boss=100;
    blood_main=60;
    energy_man=2;
    Boss.isDie=false;
    Main.isDie=false;
    paused=false;
    enemys.clear();
    bullets.clear();
    bulletBoss.clear();
    Map.rect.x=0;
    Main.rect.x=0;
    Boss.rect.y=-100;
    Time=0;
    numberkill=0;
    notpressPaused=false;
    musicStarted = false;
}
//xu li su kien ban phim va chuot
void HandlingEventFirst()
{
    while (SDL_PollEvent(&g_event))
    {
        if (g_event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if(g_event.type==SDL_MOUSEBUTTONDOWN)
        {
            int x=g_event.button.x,y=g_event.button.y;
            std:: cout <<x<<" "<<y<<endl;
            if (y>405&&y<550)
            {
                if(x>50&&x<300)
                {
                    Map.texture=TextureMap[0];
                    isScreenMain=true;
                    restart();
                }
                else if(x>320&&x<580)
                {
                    Map.texture=TextureMap[1];
                    isScreenMain=true;
                    restart();
                }
                else if(x>600&&x<860)
                {
                    Map.texture=TextureMap[2];
                    isScreenMain=true;
                    restart();
                }
                else if(x>900&&x<1180)
                {
                    Map.texture=TextureMap[3];
                    isScreenMain=true;
                    restart();
                }
            }
            else if(x>1100&&y<100)
            {
                isScreenMain=true;
            }
        }
    }
}
void HandlingEvent()
{
    while (SDL_PollEvent(&g_event))
    {
        if (g_event.type == SDL_QUIT)
        {
            quit = true;

        }
        else if(g_event.type==SDL_KEYDOWN)
        {
            if(!paused)
            {
                switch(g_event.key.keysym.sym)
                {
                case SDLK_UP:
                    Main.moveUp=true;
                    break;
                case SDLK_DOWN:
                    Main.moveDown=true;
                    break;
                case SDLK_LEFT:
                    Main.moveLeft=true;
                    break;
                case SDLK_RIGHT:
                    Main.moveRight=true;
                    break;
                case SDLK_z:
                {
                    Main.isFighted=true;
                    BackGround newBullet;
                    newBullet.rect= {Main.rect.x+60,Main.rect.y+40,BULLET_WIDTH,BULLET_HEIGHT};
                    if(bullets.size()<=number_bullet_main)
                    {
                        bullets.push_back(newBullet.rect);

                        //tai am thanh bullet
                        Mix_PlayChannel(-1,bulletSound,0);
                    }
                }
                break;
                case SDLK_x:
                    if(Main.isFullEnergy)
                    {
                        Main.isFullEnergy = false;
                        PressX = true;
                        energy_man = 2;
                        lastTimeX = SDL_GetTicks();
                        Bullet2.rect = {-150, rand()%600, 120, 40};   // bắn từ vị trí máy bay chính
                        Mix_PlayChannel(-1,laze,0);
                    }
                    break;
                }
            }
        }
        else if(g_event.type==SDL_KEYUP&&g_event.key.repeat==0)
        {
            switch(g_event.key.keysym.sym)
            {
            case SDLK_UP:
                Main.moveUp=false;
                break;
            case SDLK_DOWN:
                Main.moveDown=false;
                break;
            case SDLK_LEFT:
                Main.moveLeft=false;
                break;
            case SDLK_RIGHT:
                Main.moveRight=false;
                break;
            case SDLK_z:
                Main.isFighted=false;
                break;

            }
        }
        else if(g_event.type==SDL_MOUSEBUTTONDOWN)
        {
            int x=g_event.button.x,y=g_event.button.y;
            if(y>=520&&y<=600)
            {
                if(x>400&&x<480&&!notpressPaused)paused=!paused;
                else if(x>500&&x<700)
                {
                    quit=true;
                }
                else if(x>700&&x<780)
                {
                    restart();
                }
                else if(x>5&&x<60)
                {
                    isScreenMain=false;
                }
            }
        }
    }

}
//ve MAP
void DrawMap()
{
    Uint32 current_time = SDL_GetTicks();
    if (current_time - lastTimeMap > 3)
    {
        Map.rect.x += 1;
        Map.rect.x %= MAP_WIDTH; // Cuộn vô hạn trong giới hạn ảnh
        lastTimeMap = current_time;
    }

    int w1 = std::min(MAP_WIDTH - Map.rect.x, SCREEN_WIDTH);

    SDL_Rect src1 = { Map.rect.x, 0, w1, Map.rect.h };
    SDL_Rect dst1 = { 0, 0, w1, Map.rect.h };
    SDL_RenderCopy(g_screen, Map.texture, &src1, &dst1);

    if (w1 < SCREEN_WIDTH)
    {
        SDL_Rect src2 = { 0, 0, SCREEN_WIDTH - w1, Map.rect.h };
        SDL_Rect dst2 = { w1, 0, src2.w, src2.h };
        SDL_RenderCopy(g_screen, Map.texture, &src2, &dst2);
    }
}
//...
//...
//ham kiem tra check var giua 2 texture
bool checkVar(SDL_Rect rect1,SDL_Rect rect2)
{
    return ((rect1.x<rect2.x+rect2.w)
            &&(rect1.x+rect1.w>rect2.x)
            &&(rect1.y<rect2.y+rect2.h)
            &&(rect1.y+rect1.h>rect2.y));
}
//...




//xu li dan cua main
void DrawBullet()
{
    if (bullets.empty()) return;
    if(numberkill<10)
    {
        number_bullet_main=1;
        Bullet.texture=texture_bullet1;
    }
    else if(numberkill==10&&SwapBullet==true)
    {
        bullets.clear();
        SwapBullet=false;
        number_bullet_main=2;
    }
    else if(numberkill>10&&numberkill<25)
    {
        SwapBullet==true;
        Bullet.texture=texture_bullet2;
    }
    else if(numberkill==25&&SwapBullet==true)
    {
        bullets.clear();
        SwapBullet=false;
        number_bullet_main=3;
    }
    else if(numberkill>25)
    {
        SwapBullet==true;
        Bullet.texture=texture_bullet3;
    }
    // Duyệt qua từng viên đạn từ cuối đến đầu
    for (int i = bullets.size() - 1; i >= 0; i--)
    {
        bullets[i].x += speed_bullet;

        // Kiểm tra va chạm giữa viên đạn và kẻ thù
        for (int j = enemys.size() - 1; j >= 0; j--)
        {
            if (checkVar(bullets[i], enemys[j].rect))
            {
                if(energy_man<50)
                {
                    energy_man+=5;
                }
                else
                {
                    Main.isFullEnergy=true;
                }
                bullets.erase(bullets.begin() + i); // Xóa viên đạn
                numberkill++;

                // Thêm vụ nổ
                Explosion newExpEnemy(texture_exp_enemy, enemys[j].rect.x, enemys[j].rect.y,0);
                newExpEnemy.lastTime = SDL_GetTicks();
                Exp.push_back(newExpEnemy);

                //tai am thanh no enemy
                Mix_PlayChannel(-1,expEnemySound,0);

                // Xóa kẻ thù bị bắn
                enemys.erase(enemys.begin() + j);
                break; // Đã xử lý xong, ra khỏi vòng lặp kiểm tra kẻ thù

            }
        }

        // Xóa viên đạn nếu nó ra ngoài màn hình
        if (bullets[i].x >= SCREEN_WIDTH)
        {
            bullets.erase(bullets.begin() + i);
        }
        else
        {
            SDL_RenderCopy(g_screen, Bullet.texture, NULL, &bullets[i]);
        }
    }


}


void DrawExp()
{
    // Duyệt qua từng vụ nổ từ cuối đến đầu
    for (int i = Exp.size() - 1; i >= 0; i--)
    {
        Exp[i].rect1.x = (Exp[i].frameNumber) * EXP_WIDTH;
        SDL_RenderCopy(g_screen, Exp[i].texture, &Exp[i].rect1, &Exp[i].rect2);

        Uint32 CurrentTime = SDL_GetTicks();
        if (CurrentTime - Exp[i].lastTime > 24)   // Cập nhật frame số vụ nổ
        {
            Exp[i].frameNumber++;
            Exp[i].lastTime = CurrentTime;
        }

        // Nếu vụ nổ đã hoàn thành, xóa nó
        if (Exp[i].frameNumber == 4)
        {
            Exp.erase(Exp.begin() + i);
        }
    }
}
void DrawBullet2()
{
    if (!PressX) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTimeX > 6 )
    {
        // Move đạn hướng về Boss
        if (Bullet2.rect.y > Boss.rect.y + 5)
        {
            Bullet2.rect.y -= 2;
        }
        else if (Bullet2.rect.y < Boss.rect.y - 5)
        {
            Bullet2.rect.y += 2;
        }

        Bullet2.rect.x += 4; // bay ngang ra
        lastTimeX=currentTime;
    }

    // Va chạm với Boss
    if (checkVar(Boss.rect, Bullet2.rect))
    {

        if(blood_boss-10>=0)
        {
            blood_boss-=10;
            Explosion newExpEnemy(texture_exp_enemy, Boss.rect.x+15, Boss.rect.x+25,0);
            Exp.push_back(newExpEnemy);
            Mix_PlayChannel(-1,expEnemySound,0);

        }
        else
        {
            Boss.isDie=true;
            lastTimeWin=SDL_GetTicks();
            Explosion newExpMain(texture_exp_main,Boss.rect.x+30,Boss.rect.y+30,0);
            newExpMain.lastTime=SDL_GetTicks();
            Exp.push_back(newExpMain);

            //tai am thanh may bay main no
            Mix_PlayChannel(-1,expMainSound,0);
        }
        PressX = false; // đạn biến mất khi trúng Boss
        // xử lý va chạm nếu muốn
    }

    SDL_RenderCopy(g_screen, Bullet2.texture, NULL, &Bullet2.rect);
}
void DrawBoss()
{
    Uint32 current_time_boss=SDL_GetTicks();
    if(current_time_boss-lastBossAppear>9)
    {
        if(Boss.rect.y>Main.rect.y+5)
        {
            Boss.rect.y-=2;
        }
        else if(Boss.rect.y<Main.rect.y-5)
        {
            Boss.rect.y+=2;
        }
        else
        {
            SDL_Rect newbullet= {Boss.rect.x,Boss.rect.y+50,48,19};
            if(bulletBoss.size()<2)
            {
                bulletBoss.push_back(newbullet);
                Mix_PlayChannel(-1,bulletBossSound,0);
            }
        }
        lastBossAppear=current_time_boss;
    }
    for(int i=bullets.size()-1; i>=0; i--)
    {
        if(checkVar(bullets[i],Boss.rect))
        {
            if(blood_boss-5>=0)
            {
                blood_boss-=5;
                Explosion newExpEnemy(texture_exp_enemy, bulletBoss[i].x-25, bulletBoss[i].y-25,0);
                Exp.push_back(newExpEnemy);
                Mix_PlayChannel(-1,expEnemySound,0);

            }
            else
            {
                Boss.isDie=true;
                lastTimeWin=SDL_GetTicks();
                Explosion newExpMain(texture_exp_main,Boss.rect.x+30,Boss.rect.y+30,0);
                newExpMain.lastTime=SDL_GetTicks();
                Exp.push_back(newExpMain);

                //tai am thanh may bay main no
                Mix_PlayChannel(-1,expMainSound,0);
            }
            bullets.erase(bullets.begin()+i);
        }
    }
    SDL_RenderCopy(g_screen,Boss.texture,NULL,&Boss.rect);
}
void DrawBulletBoss()
{
    for(int i=bulletBoss.size()-1; i>=0; i--)
    {
        bulletBoss[i].x-=speed_bullet;
        if(bulletBoss[i].x==-20)
        {
            bulletBoss.erase(bulletBoss.begin()+i);
        }
        else if(checkVar(bulletBoss[i],Main.rect))
        {
            if(blood_main-2>=0)
            {
                blood_main-=2;
                Explosion newExpEnemy(texture_exp_enemy, bulletBoss[i].x-25, bulletBoss[i].y-25,0);
                newExpEnemy.lastTime = SDL_GetTicks();
                Exp.push_back(newExpEnemy);

                //tai am thanh no enemy
                Mix_PlayChannel(-1,expEnemySound,0);

            }
            else
            {
                Main.isDie=true;
                Explosion newExpMain(texture_exp_main,Main.rect.x,Main.rect.y,0);
                newExpMain.lastTime=SDL_GetTicks();
                lastTimeDie=newExpMain.lastTime;
                SDL_Delay(10);
                Exp.push_back(newExpMain);

                //tai am thanh may bay main no
                Mix_PlayChannel(-1,expMainSound,0);
            }
            bulletBoss.erase(bulletBoss.begin()+i);
        }
        else
        {
            SDL_RenderCopy(g_screen,texture_bullet_boss,NULL,&bulletBoss[i]);
        }
    }
}

void DrawEnemy()
{
    // Cập nhật số lượng enemy dựa theo kill
    if (numberkill <= 6)
    {
        numberenemy = 3;
    }
    else if (numberkill < 20)
    {
        numberenemy = 4;
    }
    else
    {
        numberenemy = 5;
    }

    // Thêm enemy mới nếu chưa đủ số lượng
    while (enemys.size() < static_cast<size_t>(numberenemy))
    {
        Enemy newEnemy;
        newEnemy.rect = {SCREEN_WIDTH + rand() % 300, rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT - 130), 100, 55};
        newEnemy.textureIndex = rand() % Enemys.size();  // Chọn texture
        enemys.push_back(newEnemy);
    }

    // Duyệt ngược để dễ xóa
    for (int i = enemys.size() - 1; i >= 0; i--)
    {
        // Di chuyển enemy
        enemys[i].rect.x -= (numberkill < 15 ? 1 : 2);

        // Nếu enemy ra khỏi màn hình
        if (enemys[i].rect.x < -ENEMY_WIDTH)
        {
            enemys.erase(enemys.begin() + i);
            continue;
        }

        // Nếu bị bắn bởi Bullet2
        if (checkVar(enemys[i].rect, Bullet2.rect))
        {
            Explosion newExp(texture_exp_enemy, enemys[i].rect.x, enemys[i].rect.y, 0);
            newExp.lastTime = SDL_GetTicks();
            Exp.push_back(newExp);
            Mix_PlayChannel(-1, expEnemySound, 0);
            enemys.erase(enemys.begin() + i);
            continue;
        }

        // Nếu va chạm với máy bay chính
        if (checkVar(enemys[i].rect, Main.rect))
        {
            if (blood_main >= 3)
            {
                blood_main -= 3;
                Explosion newExp(texture_exp_enemy, enemys[i].rect.x, enemys[i].rect.y, 0);
                newExp.lastTime = SDL_GetTicks();
                Exp.push_back(newExp);
                Mix_PlayChannel(-1, expEnemySound, 0);
            }
            else
            {
                Main.isDie = true;
                Explosion newExp(texture_exp_main, Main.rect.x, Main.rect.y, 0);
                newExp.lastTime = SDL_GetTicks();
                lastTimeDie = newExp.lastTime;
                Exp.push_back(newExp);
                Mix_PlayChannel(-1, expMainSound, 0);
            }
            enemys.erase(enemys.begin() + i);
            continue;
        }

        // Vẽ enemy
        SDL_Texture* tex = Enemys[enemys[i].textureIndex];
        SDL_RenderCopy(g_screen, tex, NULL, &enemys[i].rect);
    }
}

//...
//ham ve chu
void DrawRenderText(SDL_Renderer* renderer, TTF_Font* font, const string &text, int x, int y, SDL_Color color)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), white);
    if(textSurface==NULL)return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(texture==NULL)return;
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};  // Vị trí và kích thước của văn bản
    SDL_RenderCopy(renderer, texture, NULL,&textRect);  // Render texture
    // Giải phóng surface và texture sau khi sử dụng
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(texture);
}
//ham luu diem
void SaveTime(long long x)
{
    ofstream file ("BestTime.txt");
    if(!file.is_open())cout<<"ERROR OPEN FILE";
    file<<x;
    file.close();
}
//...
void DrawName()
{
    static string textName = "";  // Giữ lại text giữa các frame

    while (SDL_PollEvent(&g_event))
    {
        if (g_event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (g_event.type == SDL_KEYDOWN && !NhapName)
        {
            SDL_Keycode key = g_event.key.keysym.sym;

            if (key == SDLK_BACKSPACE && !textName.empty())
            {
                textName.pop_back();
            }
            else if (key >= SDLK_SPACE && key <= SDLK_z)
            {
                textName += static_cast<char>(key);
            }
        }
    }

    DrawRenderText(g_screen, font, textName, 500, 220, black);
}

//hàm doc file điểm đã được lưu
long long MaxPoint()
{
    ifstream file ("BestTime.txt");
    if(!file.is_open())
    {
        return 0;
    }
    long long x;
    file>>x;
    file.close();
    return x;
}
//...
//ve box
void DrawBoxs()
{
    if(numberkill%10==0&&numberkill!=0&&Boxs.empty()&&numberkill!=lastBoxKill)
    {
        lastBoxKill=numberkill;
        SDL_Rect newBoxHp= {rand()%300,0,100,100};
        if(Boxs.size()<=2)
        {
            Boxs.push_back(newBoxHp);
        }
    }

    for(int i=Boxs.size()-1; i>=0; i--)
    {
        Boxs[i].y+=1;
        if(Boxs[i].y>=1200)
        {
            Boxs.erase(Boxs.begin()+i);
        }
        if(checkVar(Boxs[i],Main.rect))
        {
            Boxs.erase(Boxs.begin()+i);
            if(blood_main+10<=60)
                blood_main+=10;
        }
        else
        {
            SDL_RenderCopy(g_screen,BoxHp.texture,NULL,&Boxs[i]);
        }
    }
}
//...
//ve menu
void DrawMenu()
{
    if (!musicStarted)
    {
        Mix_PlayMusic(FreeFireSound, -1); // -1 để lặp vô hạn nếu muốn
        musicStarted = true;
    }
    string kill="Kill:"+to_string(numberkill);
    string time_current="Time:"+to_string(Time)+" s";
    string best="Best Time:"+to_string(bestTime)+" s";
    DrawRenderText(g_screen,font,kill.c_str(),0,0,white);
    DrawRenderText(g_screen,font,time_current.c_str(),0,30,white);
    DrawRenderText(g_screen,font,best.c_str(),0,60,black);
    SDL_RenderCopy(g_screen,Continue.texture,NULL,&Continue.rect);
    SDL_RenderCopy(g_screen,Restart.texture,NULL,&Restart.rect);
    SDL_RenderCopy(g_screen,Quit.texture,NULL,&Quit.rect);
    SDL_RenderCopy(g_screen,Back.texture,NULL,&Back.rect);
    if(Main.isDie==true)
    {
        Mix_PlayMusic(LoseSound,0);
        DrawRenderText(g_screen,font,"YOU LOSE",500,200,white);
        notpressPaused=true;
        Uint32 Current_Time=SDL_GetTicks();
        if(Current_Time-lastTimeDie>300)paused=true;
    }

    if(Boss.isDie)
    {
        Mix_PlayMusic(WinSound, 0);
        DrawRenderText(g_screen,font,"YOU WIN",500,200,black);
        string thanh_tich="YOUR TIME: "+to_string(Time);
        DrawRenderText(g_screen,font,thanh_tich.c_str(),470,250,black);
        notpressPaused=true;
        Uint32 Current_Time=SDL_GetTicks();
        if(Current_Time-lastTimeWin>300)paused=true;
        if(bestTime==0)bestTime=Time;
        else
        {
            if(bestTime>Time)
            {
                bestTime=Time;
                SaveTime(Time);
            }
        }
        notpressPaused=true;
    }
}
//...
//ham ve thanh mau cua player
void DrawBlood()
{

    Main.blood= {Main.rect.x+7,Main.rect.y-20,blood_main,10};
    Main.energy= {Main.rect.x+12,Main.rect.y-10,energy_man,7};
    Boss.blood= {Boss.rect.x+7,Boss.rect.y-20,blood_boss,15};
    SDL_SetRenderDrawColor(g_screen,219,26,33,255);
    SDL_RenderFillRect(g_screen,&Main.blood);
    SDL_SetRenderDrawColor(g_screen,194,23,29,255);
    SDL_RenderFillRect(g_screen,&Boss.blood);
    SDL_SetRenderDrawColor(g_screen,255,231,75,255);
    SDL_RenderFillRect(g_screen,&Main.energy);
//neu MainDie

}
// Hàm vẽ texture lên cửa sổ
void DrawOnWindow()
{
    SDL_RenderClear(g_screen);
    if(isScreenMain==false)
    {
        HandlingEventFirst();
        SDL_RenderCopy(g_screen,BkFirst.texture,NULL,NULL);

    }
    else
    {
        EventPlayer();
        Timer();
        DrawMap();
        if(!Main.isDie) SDL_RenderCopy(g_screen, Main.texture, NULL, &Main.rect);
        DrawMenu();
        DrawBlood();
        DrawBullet();
        if(PressX==true)DrawBullet2();
        DrawBoxs();
        DrawEnemy();
        if(!Boss.isDie)DrawBoss();
        DrawBulletBoss();
        DrawExp();
    }
    SDL_RenderPresent(g_screen);
}
//...

// Hàm load texture ban dau cua game
void first_state()
{
//cap nhat best time tu file txt
    isScreenMain=false;
    bestTime=MaxPoint();
    lastTimeMap=SDL_GetTicks();
    Map.texture = IMG_LoadTexture(g_screen, "image/bk2.png");
    Map.rect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    if (Map.texture == NULL)
    {
        cout << "Không thể tải background!" << endl;

    }
    for (int i = 1; i <= 4; i++)
    {
        std::string path;
        if (i == 2)
        {
            path = "image/bk" + std::to_string(i) + ".jpg";
        }
        else
        {
            path = "image/bk" + std::to_string(i) + ".png";
        }

        SDL_Texture* texture = IMG_LoadTexture(g_screen, path.c_str());
        if (texture != nullptr)
        {
            TextureMap.push_back(texture);
        }
        else
        {
            std::cerr << "Failed to load texture: " << path << " SDL_Error: " << SDL_GetError() << std::endl;
        }
    }
    Map.texture=TextureMap[3];

    BkFirst.texture=IMG_LoadTexture(g_screen,"image/bk0.png");

    Back.texture=IMG_LoadTexture(g_screen,"image/back.png");
    Back.rect= {10,520,60,60};
    Continue.texture=IMG_LoadTexture(g_screen,"image/continueButton.png");
    Continue.rect= {400,520,CONTINUE_WIDTH,CONTINUE_HEIGHT};

    Restart.texture=IMG_LoadTexture(g_screen,"image/playAgainButton.png");
    Restart.rect= {700,520,CONTINUE_WIDTH,CONTINUE_HEIGHT};

    Quit.texture=IMG_LoadTexture(g_screen,"image/exitButton.png");
    Quit.rect= {500,520,QUIT_WIDTH,QUIT_HEIGHT};

    texture_plane_normal=loadTexture("image/plane_fly.png",g_screen);
    texture_plane_fight=loadTexture("image/plane_fly_bullet.png",g_screen);
    Main.texture=texture_plane_normal;
    if(texture_plane_fight==NULL||texture_plane_normal==NULL)
    {
        cout<<"loi tai texture plane"<<endl;
    }
    Bullet2.texture=IMG_LoadTexture(g_screen,"image/torpedo_black.png");
    Bullet2.rect= {-150,Main.rect.y,120,40};
    texture_bullet1=IMG_LoadTexture(g_screen,"image/bullet2.png");
    texture_bullet2=IMG_LoadTexture(g_screen,"image/bullet3.png");
    texture_bullet3=IMG_LoadTexture(g_screen,"image/bullet1.png");
    Bullet.texture=texture_bullet1;
    if(Bullet.texture==NULL)
    {
        cout<<"loi tai anh dan ";
    }
    for (int i = 1; i <= 12; ++i)
    {
        std::string path = "image/af" + std::to_string(i) + ".png";
        SDL_Texture* texture = IMG_LoadTexture(g_screen, path.c_str());
        if (texture != nullptr)
        {
            Enemys.push_back(texture);
        }
        else
        {
            // In lỗi nếu không load được ảnh
            std::cerr << "Failed to load texture: " << path << " SDL_Error: " << SDL_GetError() << std::endl;
        }
    }
    number_bullet_main=1;

    texture_enemy2=loadTexture("image/boss3.jpg",g_screen);

    texture_exp_main=loadTexture("image/exp_main.png",g_screen);
    texture_exp_enemy=loadTexture("image/exp.png",g_screen);
    if(texture_exp_enemy==NULL)cout<<"null";

    texture_boss=IMG_LoadTexture(g_screen,"image/boss.png");
    Boss.texture=texture_boss;
    lastBossAppear=SDL_GetTicks();
    Boss.rect= {1050,0,BOSS_WIDTH,BOSS_HEIGHT};

    texture_bullet_boss=IMG_LoadTexture(g_screen,"image/bullet_boss.png");
    texture_box_hp=IMG_LoadTexture(g_screen,"image/hp2.png");
    BoxHp.texture=texture_box_hp;

    Name.texture=IMG_LoadTexture(g_screen,"image/name.png");
    Name.rect= {550,200,200,100};

    bulletSound=Mix_LoadWAV("sound/bullet.wav");
    laze=Mix_LoadWAV("sound/laser.wav");
    bulletBossSound=Mix_LoadWAV("sound/Laser.wav");
    expEnemySound=Mix_LoadWAV("sound/exp_enemy.wav");
    expMainSound=Mix_LoadWAV("sound/exp_main.wav");
    WinSound=Mix_LoadMUS("sound/win_sound.mp3");
    LoseSound=Mix_LoadMUS("sound/lose_sound.mp3");
    FreeFireSound=Mix_LoadMUS("sound/freefire.mp3");

    if(bulletSound==NULL||expEnemySound==NULL||expMainSound==NULL||WinSound==NULL||LoseSound==NULL)
    {
        cout<<"sound null";
    }

}
//...

// Hàm giải phóng tài nguyên
void close()
{
    SDL_DestroyTexture(Map.texture);
    SDL_DestroyTexture(Main.texture);
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
    Mix_CloseAudio();
    Mix_Quit();
}
//...


// Main
int main(int argc, char* argv[])
{
    if (!init()) return -1;
    srand(time(0));
    first_state();
    while (!quit)
    {
        if(isScreenMain==true)
        {
            HandlingEvent();
        }
        if(!paused)
            DrawOnWindow();
    }
    close();
    return 0;
}
