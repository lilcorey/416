#include <iostream>
#include <sstream>
#include <iomanip>
#include "frameGenerator.h"
#include "renderContext.h"


void FrameGenerator::makeFrame() {
  if ( int(frameCount) > Gamedata::getInstance().getXmlInt("frameMax") ) return;
  SDL_Surface* screenCap = SDL_CreateRGBSurface(0,
   Gamedata::getInstance().getXmlInt("view/width"), 
   Gamedata::getInstance().getXmlInt("view/height"),
   32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  if ( screenCap ) {
    SDL_RenderReadPixels(RenderContext::getInstance()->getRenderer(), NULL, 
      SDL_GetWindowPixelFormat(RenderContext::getInstance()->getWindow()), 
      screenCap->pixels, screenCap->pitch); 
  }
  std::stringstream strm;
  strm << "frames/" << Gamedata::getInstance().getXmlStr("username").c_str() << '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screenCap, filename.c_str());
}

