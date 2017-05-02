#include "birdsprite.h"

void BirdSprite::draw() const{
  if(!goingLeft){
    Sprite::draw();
  }
  else{
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    getFrame()->draw(getX(), getY(), getScale(), flip);

  }
}
void BirdSprite::update(Uint32 ticks){

  Sprite::update(ticks);

  if ( getX() < 0) {
    goingLeft = false;
  }
  if ( getX() > getWorldWidth()-getFrameWidth()) {
    goingLeft = true;
  }
}
