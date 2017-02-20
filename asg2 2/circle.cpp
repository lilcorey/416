#include <math.h>
#include "circle.h"

void Circle::updateCircle(Clock clock){
	const int cx = 420, cy = 190;
	const float twoPi = 2*3.14159;

  if(getLaps() > 5) return;


  if(getRad() == 70){
    decAngle();
  }
  else{
    incAngle();
  }

	int tx = cx + cos(getAngle()*twoPi)*getRad();
	int ty = cy + sin(getAngle()*twoPi)*getRad();
  setRect(tx, ty);


	if(getAngle() > 1.0 || getAngle() < -1.0){
    resetAngle();
    incLaps();
	}

}
