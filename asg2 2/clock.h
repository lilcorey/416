#ifndef clock
#define clock

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>

class Clock{
public:
  Clock() : PERIOD(ceil(1000.0f/60u)), prevTicks(SDL_GetTicks()),
            currentTicks(SDL_GetTicks()), startTime(SDL_GetTicks()), elapsedTicks(currentTicks - prevTicks), dt(0.0) {}

  void updatePrev() { prevTicks = currentTicks; }
	unsigned int getstartTime() const { return startTime; }
	float getDt() const { return dt; }
  int calcClock(){
    currentTicks = SDL_GetTicks();
    elapsedTicks = currentTicks - prevTicks;
    if(elapsedTicks < PERIOD) return 1;
		dt = elapsedTicks *0.001f;
    return 0;
  }

private:
  const int PERIOD;
  int prevTicks;
  int currentTicks;
  int startTime;
  int elapsedTicks;
	float dt;
};
#endif

