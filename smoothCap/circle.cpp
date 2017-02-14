#include <math.h>
#include "circle.h"

void Circle::updateCircle(){
	const int cx = 420, cy = 190;
	//const int rad = 80;
	const float twoPi = 2*3.14159;

  if(getLaps() > 12) return;

  // static unsigned int remainder = 0u; // ***
  // const unsigned int DT = 17u; // ***
  // static unsigned int prevTicks = SDL_GetTicks();
  // unsigned int currentTicks = SDL_GetTicks();
  // unsigned int elapsedTicks = currentTicks - prevTicks + remainder; // ***

  //if( elapsedTicks < DT ) return;

  if(getRad() == 80){
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
 // prevTicks = currentTicks;
 // remainder = elapsedTicks - DT; // ***
}
