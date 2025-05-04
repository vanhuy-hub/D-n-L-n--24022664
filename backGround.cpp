#ifndef BACK_GROUND_
#define BACK_GROUND_
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
#endif // BACK_GROUND_
