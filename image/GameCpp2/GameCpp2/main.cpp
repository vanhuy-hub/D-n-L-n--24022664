// GameCpp2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include "BossObject.h"

#define USE_AUDIO
BaseObject g_background;
TTF_Font* g_font_text = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init( SDL_INIT_VIDEO);
    if(ret < 0) return false;


    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    //Create window
    g_window = SDL_CreateWindow("SDL 2.0 Game Demo - Phat Trien Phan Mem 123AZ",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if(g_window == NULL )
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }


        g_font_text = TTF_OpenFont("font//dlxfont.ttf", 15);
        if (g_font_text == NULL)
        {
            return false;
        }
    }

   
    return success;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false)
    {
        return false;
    }

    return true;
}

std::vector<ThreatsObject*> MakeThreadList()
{

    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* thread_objs = new ThreatsObject[20];

    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* thread_obj = (thread_objs+i);
        if (thread_obj != NULL)
        {
            thread_obj->LoadImg("img//threat_left.png", g_screen);
            thread_obj->set_clips();
            thread_obj->InitPlayer();
            thread_obj->set_xpos(500 + i*500);
            thread_obj->set_ypos(200);
            thread_obj->set_type_move(ThreatsObject::MOVE_IN_SPACE_TH);
            int pos1 = thread_obj->get_x_pos() - 60;
            int pos2 = thread_obj->get_x_pos() + 60;
            thread_obj->SetAnimationPos(pos1, pos2);
            list_threats.push_back(thread_obj);
        }
    }

    ThreatsObject* thread_objs2 = new ThreatsObject[15];
    for (int i = 0; i < 15; i++)
    {
        ThreatsObject* thread_obj3 = (thread_objs2+i);
        if (thread_obj3 != NULL)
        {
            thread_obj3->LoadImg("img//threat_level.png", g_screen);
            thread_obj3->set_clips();
            thread_obj3->InitPlayer();
            thread_obj3->set_input_left(0);
            thread_obj3->set_xpos(700 + i*1500);
            thread_obj3->set_ypos(250);

            thread_obj3->set_type_move(ThreatsObject::STATIC_TH);

            BulletObject* p_bullet = new BulletObject();
            thread_obj3->InitBullet(p_bullet, g_screen);
            //thread_obj3->set_is_alive(true);
            list_threats.push_back(thread_obj3);
        }
    }


    return list_threats;
}


int main(int argc, char* argv[])
{
    ImpTimer fps;
    //Start up SDL and create window
    if (InitData() == false)
    {
        return -1;
    }


    if(!LoadBackground())
    {
        return -1;
    }

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadMapTiles(g_screen);


    MainObject p_player;
    p_player.LoadImg(g_name_main_right, g_screen);
    p_player.set_clips();


    PlayerPower player_power;
    player_power.Init(g_screen);


    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5 - 300, 8);


    std::vector<ThreatsObject*> threats_list = MakeThreadList();


    //Init Boss Object
    BossObject bossObject;
    bossObject.LoadImg("img//boss_object.png", g_screen);
    bossObject.set_clips();
    int xPosBoss = MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*0.6;
    bossObject.set_xpos(xPosBoss);
    bossObject.set_ypos(10);

    ExplosionObject exp_threats;
    ExplosionObject exp_main;

    bool ret = exp_threats.LoadImg("img//exp3.png", g_screen);
    exp_threats.set_clips();
    if (!ret) return -1;


    ret = exp_main.LoadImg("img//exp3.png", g_screen);
    exp_main.set_clips();
    if (!ret) return 0;


    TextObject time_game;
    time_game.setColor(TextObject::WHITE_TEXT);

    TextObject mark_game;
    mark_game.setColor(TextObject::WHITE_TEXT);

    TextObject money_count;
    money_count.setColor(TextObject::WHITE_TEXT);

    int num_die = 0;
    unsigned int mark_value = 0;

    bool quit = false;
    while(!quit)
    {
        fps.start();
        while( SDL_PollEvent(&g_event) != 0 )
        {
            //User requests quit
            if(g_event.type == SDL_QUIT )
            {
                quit = true;
            }

            p_player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        Map ga_map = game_map.GetMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(ga_map.start_x_, ga_map.start_y_);
        p_player.DoPlayer(ga_map);
        p_player.Show(g_screen);

        game_map.SetMap(ga_map);
       game_map.DrawMap(g_screen);

       //Draw Geometric
       GeometricFormat rectange_size(0, 0, SCREEN_WIDTH, 40);
       ColorData color_data(36, 36, 36);
       Gemometric::RenderRectange(rectange_size, color_data, g_screen);

       GeometricFormat outlie_size(1, 1, SCREEN_WIDTH - 1, 38);
       ColorData color_data1(255, 255, 255);
       Gemometric::RenderOutline(outlie_size, color_data1, g_screen);




       player_power.Show(g_screen);
       player_money.Show(g_screen);

       for (int i = 0; i < threats_list.size(); i++)
       {
           ThreatsObject* obj_threat = threats_list.at(i);
           obj_threat->SetMapXY(ga_map.start_x_, ga_map.start_y_);
           obj_threat->ImpMoveType(g_screen);
           obj_threat->DoPlayer(ga_map);
           obj_threat->Show(g_screen);
           obj_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);


           SDL_Rect rect_player = p_player.GetRectFrame();

           //COLLISION THREAT BULLET -> MAIN OBJECT
           bool is_col1 = false;
           std::vector<BulletObject*> bullet_list = obj_threat->get_bullet_list();
           for (int am = 0; am < bullet_list.size(); am++)
           {
               BulletObject* p_bullet = bullet_list.at(am);
               if (p_bullet)
               {
                   is_col1 = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), rect_player);
                   if (is_col1 == true)
                   {
                       obj_threat->ResetBullet(p_bullet);
                       break;
                   }
               }
           }

           SDL_Rect rect_threat = obj_threat->GetRectFrame();
           bool is_col2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

           if (is_col2 || is_col1)
           {
               //obj_threat->Reset(SCREEN_WIDTH, SCREEN_HEIGHT);
               //walk_object.set_is_move(true);
               int width_exp_frame = exp_main.get_frame_height();
               int heiht_exp_height = exp_main.get_frame_width();
               for (int ex = 0; ex < 4; ex++)
               {
                   int x_pos = (p_player.GetRect().x + p_player.get_frame_width()*0.5) - width_exp_frame*0.5;
                  int y_pos = (p_player.GetRect().y + p_player.get_frame_height()*0.5) - heiht_exp_height*0.5;

                   exp_main.set_frame(ex);
                   exp_main.SetRect(x_pos, y_pos);
                   exp_main.Show(g_screen);
                   SDL_RenderPresent(g_screen);
               }
#ifdef USE_AUDIO 
               Mix_PlayChannel(-1, g_sound_ex_main, 0);
#endif
               num_die++;
               if (num_die <= 3)
               {
                   p_player.SetRect(0, 0);
                   p_player.set_think_time(60);
                   SDL_Delay(1000);
                   player_power.Decrease();
                   player_power.Render(g_screen);
                   continue;
               }
               else
               {
                   if(MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
                   {
                       obj_threat->Free();
                       close();
                       SDL_Quit();
                       return 0;
                   }
               }
           }
       }


       //COLLISION THREAT -> Main Bullet
       std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
       int frame_exp_width = exp_threats.get_frame_width();
       int frame_exp_height = exp_threats.get_frame_width();

       for (int am = 0; am < bullet_arr.size(); am++)
       {
           BulletObject* p_bullet = bullet_arr.at(am);
           if (p_bullet)
           {

               for (int i = 0; i < threats_list.size(); i++)
               {
                   ThreatsObject* obj_threat = threats_list.at(i);

                   SDL_Rect threat_rect;
                   threat_rect.x = obj_threat->GetRect().x;
                   threat_rect.y = obj_threat->GetRect().y;
                   threat_rect.w = obj_threat->get_width_frame();
                   threat_rect.h = obj_threat->get_height_frame();

                   bool is_col = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), threat_rect);
                   if (is_col)
                   {
                       mark_value++;
                       for (int ex = 0; ex < 8; ex++)
                       {
                           int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                           int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                           exp_threats.set_frame(ex);
                           exp_threats.SetRect(x_pos, y_pos);
                           exp_threats.Show(g_screen);
                       }

                       // p_threat->Reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                       p_player.RemoveBullet(am);

                       //if (obj_threat->get_type_move() == ThreatsObject::MOVING_CONTINOUS)
                       //{
                       //    obj_threat->Reset();
                       //}
                       //else
                       //{
                           obj_threat->Free();
                           threats_list.erase(threats_list.begin() + i);
                       //}

#ifdef USE_AUDIO 
                       Mix_PlayChannel(-1, g_sound_explosion, 0);
#endif
                   }
               }
           }
       }


       //Show time for game
       std::string str_time = "Time: ";
       Uint32 time_val = SDL_GetTicks() / 1000;
       Uint32 val_time = 300 - time_val;

       if (val_time <= 0)
       {
           if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
           {
               quit = true;
               break;
           }
       }
       else
       {
           std::string str_val = std::to_string(val_time);
           str_time += str_val;

           time_game.SetText(str_time);
           time_game.loadFromRenderedText(g_font_text, g_screen);
           time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
       }


       //Show mark value to screen
       std::string val_str_mark = std::to_string(mark_value);
       std::string strMark("Mark: ");
       strMark += val_str_mark;

       mark_game.SetText(strMark);
       mark_game.loadFromRenderedText(g_font_text, g_screen);
       mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 50, 15);

       int money_val = p_player.GetMoneyCount();
       std::string money_count_str = std::to_string(money_val);
       money_count.SetText(money_count_str);
       money_count.loadFromRenderedText(g_font_text, g_screen);
       money_count.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, 15);



       //Process Boss
       int val = MAX_MAP_X*TILE_SIZE - (ga_map.start_x_ + p_player.GetRect().x);
       if (val <= SCREEN_WIDTH)
       {
        bossObject.SetMapXY(ga_map.start_x_, ga_map.start_y_);
        bossObject.DoPlayer(ga_map);
        bossObject.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
        bossObject.Show(g_screen);
       }

        //Update screen
        SDL_RenderPresent(g_screen);

        int imp_time = fps.get_ticks();
        int time_for_one_frame = 1000/FRAMES_PER_SECOND;
        if(imp_time < time_for_one_frame)
        {
            SDL_Delay(time_for_one_frame - imp_time);
        }

    }


    for (int i = 0; i < threats_list.size(); i++)
    {
        ThreatsObject* p_thread = threats_list.at(i);
        p_thread->Free();
        p_thread = NULL;
    }

    threats_list.clear();

    close();

    return 0;
}

