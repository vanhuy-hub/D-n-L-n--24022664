
#ifndef PLAYER_POWER_H_
#define  PLAYER_POWER_H_
#include "CommonFunc.h"
#include "BaseObject.h"
class PlayerPower : public BaseObject
{
public:
  PlayerPower(void);
  ~PlayerPower(void);

  void SetNum(const int& num) {number_ = num;}
  void AddPos(const int& xPos);
  void Show(SDL_Renderer* screen);
  void Init(SDL_Renderer* screen);
  void InCrease();
  void Decrease();
private:
  int number_;
  std::vector<int> position_list_;
};



class PlayerMoney : public BaseObject
{
public:
    PlayerMoney(void);
    ~PlayerMoney(void);

    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);
    void SetPos(const int& x, const int& y) {x_pos_ = x; y_pos_ = y;};
private:
    int x_pos_;
    int y_pos_;
};
#endif
