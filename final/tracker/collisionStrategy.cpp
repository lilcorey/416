#include <vector>
#include <algorithm>
#include "ioMod.h"
#include "collisionStrategy.h"
#include "viewport.h"
#include "renderContext.h"

void RectangularCollisionStrategy::draw() const {
  IOmod::
  getInstance().writeText("Strategy: Rectangular", 500, 30);
}

bool RectangularCollisionStrategy::execute(
      const Drawable& obj1, const Drawable& obj2) const {
  float left1 = obj1.getX();
  float left2 = obj2.getX();

  float scale1 = obj1.getScale();
  float scale2 = obj2.getScale();

  float right1 = left1+scale1*obj1.getFrame()->getWidth();
  float right2 = left2+scale2*obj2.getFrame()->getWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = obj1.getY();
  float top2 = obj2.getY();
  float bottom1 = top1+scale1*obj1.getFrame()->getHeight();
  float bottom2 = top2+scale2*obj2.getFrame()->getHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;
  return true;
}


float MidPointCollisionStrategy::
distance(float x1, float y1, float x2, float y2) const {
  float x = x1-x2, y = y1-y2;
  return hypot(x, y);
}

void MidPointCollisionStrategy::draw() const {
  IOmod::
  getInstance().writeText("Strategy: Distance", 500, 30);
}

bool MidPointCollisionStrategy::execute(
      const Drawable& obj1, const Drawable& obj2) const {

  float scale1 = obj1.getScale();
  float scale2 = obj2.getScale();

  int width1 = scale1*obj1.getFrame()->getWidth();
  int width2 = scale2*obj2.getFrame()->getWidth();
  int height1 = scale1*obj1.getFrame()->getHeight();
  int height2 = scale2*obj2.getFrame()->getHeight();

  int COLLISION_DISTANCE = width1/2 + width2/2;
  float xobj1 = obj1.getX() + width1/2;
  float yobj1 = obj1.getY() + height1/2;
  float xobj2 = obj2.getX() + width2/2;
  float yobj2 = obj2.getY() + height2/2;
  return distance(xobj1, yobj1, xobj2, yobj2) < COLLISION_DISTANCE;
}
