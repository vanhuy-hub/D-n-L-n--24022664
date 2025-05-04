#include "backGround.cpp"
#include"commonfunction.h"
class Player:public BackGround
{
public:
    SDL_Rect energy;
    SDL_Rect blood;
    bool isDie;
    bool isFighted;
    bool moveUp,moveDown,moveRight,moveLeft;
    bool isFullEnergy;
    Player()
    {
        texture = NULL;
        rect = {0, 200, PLANE_WIDTH, PLANE_HEIGHT};
        isFighted=isFullEnergy = false;
        isDie = false;
        moveUp=moveDown=moveRight=moveLeft=false;
    }
};
