#include"commonfunction.h"
class BackGround {
public:
    SDL_Texture* texture;
    SDL_Rect rect;

    BackGround() {
        texture = NULL;
        rect={0,0,0,0};
    }

};
