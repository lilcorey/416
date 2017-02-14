
#include "sprite.h"

class Circle : public mySprite{
public:
  Circle(const mySprite& sprite, const int radians) :
    mySprite(sprite),
    currentAngle(0.0),
    lapCount(0),
    rad(radians) {}
  float getAngle() const { return currentAngle; }
  int getRad() const { return rad; }
  void incAngle() {  (currentAngle += 0.04); }
  void decAngle() {  (currentAngle -= 0.04); }
  void resetAngle() {  (currentAngle = 0.0); }
  int getLaps() const { return lapCount; }
  int incLaps() { return (lapCount += 1); }
  void updateCircle();

private:
  float currentAngle;
  int lapCount;
  const int rad;
};

