#include "sprite.h"

class Circle : public mySprite{
public:
  Circle(const mySprite& sprite, const int radians) :
    mySprite(sprite),
    currentAngle(0.0),
    lapCount(0),
    rad(radians) {}

  float getAngle() const { return currentAngle; }
  void incAngle() {  (currentAngle += 0.025); }
  void decAngle() {  (currentAngle -= 0.025); }
  void resetAngle() {  (currentAngle = 0.0); }
  int getRad() const { return rad; }
  int getLaps() const { return lapCount; }
  int incLaps() { return (lapCount += 1); }
  void updateCircle(Clock clock);

private:
  float currentAngle;
  int lapCount;
  const int rad;
	Circle();
	Circle& operator=(const Circle&);
};

