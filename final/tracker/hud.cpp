#include "hud.h"
#include <sstream>
#include <string>
#include "gamedata.h"

HUD::HUD(Player* p) : clock(Clock::getInstance()),
             io(IOmod::getInstance()),
             player(p),
             xcord(Gamedata::getInstance().getXmlInt("hudfont/xcord")),
             ycord(Gamedata::getInstance().getXmlInt("hudfont/ycord")),
             width(Gamedata::getInstance().getXmlInt("hudfont/width")),
             height(Gamedata::getInstance().getXmlInt("hudfont/height")),
             rc(RenderContext::getInstance()){}

void HUD::updateHUD() const {

  SDL_SetRenderDrawColor( rc->getRenderer(), 208, 209, 210, 255 );

  SDL_Color colBlack = {0,0,0,0};
  SDL_Rect r;
  r.x = xcord;
  r.y = ycord;
  r.w = width;
  r.h = height;

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(rc->getRenderer(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( rc->getRenderer(), 255, 255, 255, 255/2 );

  // Render rect
  SDL_RenderFillRect( rc->getRenderer(), &r );

  SDL_SetRenderDrawColor( rc->getRenderer(), 255, 0, 0, 255 );
  SDL_RenderDrawRect( rc->getRenderer(), &r );

  // Render the rect to the screen
  TTF_SetFontStyle(io.getFont(), TTF_STYLE_BOLD);//set font to bold
  std::stringstream strm;
  strm << "sec: " << clock.getSeconds() << "  AvgFps: " << clock.getAvgFps();
  io.writeText(strm.str(), colBlack, xcord+5, ycord+5);
  io.writeText("jump: W", colBlack, xcord+45, ycord+30);
  io.writeText("left: A       right: D", colBlack, xcord+10, ycord+50);
  io.writeText("shoot: space", colBlack, xcord+40, ycord+70);
  io.writeText("Health: " + std::to_string((int)player->getHealth()), colBlack, xcord+5, ycord+95);
  io.writeText("Active bullets: " + std::to_string(player->getActiveBullets()), colBlack, xcord+5, ycord+125);
  io.writeText("Free bullets: " + std::to_string(player->getFreeBullets()), colBlack, xcord+5, ycord+145);
  TTF_SetFontStyle(io.getFont(), TTF_STYLE_NORMAL);//return font to normal

}
