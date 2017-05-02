#include <cmath>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

Vector2f SmartSprite::makeVelocity(int vx, int vy) const {
  float xMultiplier = Gamedata::getInstance().getRandInRange(vx, vx+70);
  float yMultiplier = Gamedata::getInstance().getRandInRange(vy-50, vy+25);

  return Vector2f(xMultiplier,yMultiplier);
}

SmartSprite::SmartSprite(const std::string& name, Player* p) :
 TwoWaySprite(name),
 player(p) {
   Vector2f t = getVelocity();
   setVelocity( makeVelocity(t[0], t[1]) );
 }

void SmartSprite::goLeft()  {
  //if (getX() > 0) setVelocityX( -fabs(getVelocityX()) );
  if(getX() > 0) dirLeft();
}
void SmartSprite::goRight() { dirRight(); }



void SmartSprite::update(Uint32 ticks) {
  TwoWaySprite::update(ticks);
  float x= getX()+getFrame()->getWidth()/2;

  float ex = player->getX()+player->getFrame()->getWidth()/2;


  if ( x > ex ) goLeft();
  if ( x < ex ) goRight();



}
