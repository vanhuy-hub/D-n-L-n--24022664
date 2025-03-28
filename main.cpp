#include "commonfunction.h"
#include <iostream>
using namespace std;
// Hàm khởi tạo SDL và SDL_Image
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Lỗi SDL: " << SDL_GetError() << endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "Lỗi SDL_image: " << IMG_GetError() << endl;
        return false;
    }
    g_window = SDL_CreateWindow("huy ngu code", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        cout << "Lỗi tạo cửa sổ: " << SDL_GetError() << endl;
        return false;
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL) {
        cout << "Lỗi tạo renderer: " << SDL_GetError() << endl;
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

    if (texture == NULL) {
        cout << "Không thể tạo texture từ " << path << " Lỗi: " << SDL_GetError() << endl;
    }
    return texture;
}

// Cấu trúc background
struct BackGround {
    SDL_Texture* texture;
    SDL_Rect rect;

    Mapp() {
        texture = NULL;
        rect={0,0,0,0};
    }

};
BackGround Map;
BackGround Menu;
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
        rect = {0, 0, PLANE_WIDTH, PLANE_HEIGHT};
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

}
// Hàm xử lý sự kiện bàn phím

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
             paused=!paused;break;

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
            }
    }

         }
         if(!paused)
         EventPlayer();
        }


// Hàm vẽ texture lên cửa sổ
void DrawOnWindow() {
    SDL_RenderClear(g_screen);
    SDL_RenderCopy(g_screen, Map.texture, &Map.rect, NULL);
    SDL_RenderCopy(g_screen, Main.texture, NULL, &Main.rect);
    if(paused==true)SDL_RenderCopy(g_screen,Menu.texture,NULL,&Menu.rect);
    SDL_RenderPresent(g_screen);
}

// Hàm giải phóng tài nguyên
void close() {
    SDL_DestroyTexture(Map.texture);
    SDL_DestroyTexture(Main.texture);
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}
// Main
int main(int argc, char* argv[]) {
    if (!init()) return -1;


    Map.texture = IMG_LoadTexture(g_screen, "image/bk2.png");
    Map.rect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    if (Map.texture == NULL) {
        cout << "Không thể tải background!" << endl;
        return -1;
    }
    Menu.texture=IMG_LoadTexture(g_screen, "image/menu.JPG");
    Menu.rect={350,150,MENU_WIDTH,MENU_HEIGHT};
   if (Menu.texture == NULL) {
        cout << "Không thể tải background!" << endl;
        return -1;
    }

    Main.texture = loadTexture("image/plane_fly.png", g_screen);
    if (Main.texture == NULL) {
        cout << "Lỗi khi tải texture của nhân vật!" << endl;
        return -1;
    }
    while (!quit) {
        HandlingEvent();

        DrawOnWindow();
        SDL_Delay(1);
    }
    close();
    return 0;
}
