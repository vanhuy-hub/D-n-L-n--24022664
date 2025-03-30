
#include "stdafx.h"
#include "BaseObject.h"



BaseObject::BaseObject()
{
  p_object_ = NULL;
  rect_.x = 0;
  rect_.y = 0;
  rect_.w = 0;
  rect_.h = 0;
}

BaseObject::~BaseObject()
{
  Free();
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface != NULL )
  {
    //Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface );
    if(newTexture != NULL )
    {
      //Get image dimensions
      rect_.w = loadedSurface->w;
      rect_.h = loadedSurface->h;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  //Return success
  p_object_ = newTexture;
  return p_object_ != NULL;
}

void BaseObject::Free()
{

  if(p_object_ != NULL)
  {
    SDL_DestroyTexture(p_object_);
    p_object_ = NULL;
    rect_.w = 0;
    rect_.h = 0;
  }
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /*=NULL*/)
{
  SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h};
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopy(des, p_object_, clip, &renderQuad);
}

void BaseObject::setColor(const Uint8& red, const Uint8& green, const Uint8& blue)
{
  SDL_SetTextureColorMod(p_object_, red, green, blue);
}

void BaseObject::setBlendMode(const SDL_BlendMode& blending)
{
  SDL_SetTextureBlendMode(p_object_, blending);
}

void BaseObject::setAlpha(const Uint8& alpha)
{
  SDL_SetTextureAlphaMod(p_object_, alpha);
}