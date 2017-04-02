#include <SDL.h>
#include <SDL_ttf.h>
#include "clock.h"
#include "ioMod.h"
#include "renderContext.h"

class HUD{
public:
  HUD();
  HUD(const HUD&) = delete;
  void updateHUD() const;

private:
  Clock& clock;
  IOmod& io;
  unsigned int xcord;
  unsigned int ycord;
  unsigned int width;
  unsigned int height;
  const RenderContext* rc;
};
