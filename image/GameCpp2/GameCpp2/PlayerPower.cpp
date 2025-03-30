
#include "stdafx.h"
#include "PlayerPower.h"


PlayerPower::PlayerPower(void)
{
}


PlayerPower::~PlayerPower(void)
{
}


void PlayerPower::AddPos(const int& xPos)
{
  position_list_.push_back(xPos);
}

void PlayerPower::Show(SDL_Renderer* screen)
{
  for (int i = 0; i < position_list_.size(); i++)
  {
    rect_.x = position_list_.at(i);
    rect_.y = 0;
    Render(screen);
  }
}

void PlayerPower::Init(SDL_Renderer* screen)
{
  LoadImg("img//player_pw.png", screen);
  number_ = 3;
  if (position_list_.size() > 0)
  {
    position_list_.clear();
  }

  AddPos(20);
  AddPos(60);
  AddPos(100);
}

void PlayerPower::Decrease()
{
  number_--;
  position_list_.pop_back();
}

void PlayerPower::InCrease()
{
    number_++;
    int last_post = position_list_.back();
    last_post += 40;
    position_list_.push_back(last_post);

    Mix_Chunk* two_beep_chunk = Mix_LoadWAV("sound//two_beep.wav");
    if (two_beep_chunk != NULL)
        Mix_PlayChannel(-1, two_beep_chunk, 0 );
}

//Player Money

PlayerMoney::PlayerMoney()
{

}

PlayerMoney::~PlayerMoney()
{

}


void PlayerMoney::Init(SDL_Renderer* screen)
{
    LoadImg("img//money.png", screen);
}


void PlayerMoney::Show(SDL_Renderer* screen)
{
   rect_.x = x_pos_;
   rect_.y = y_pos_;
   Render(screen);
}