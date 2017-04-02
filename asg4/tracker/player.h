#ifndef PLAYER_H
#define PLAYER_H
#include "twowaysprite.h"

class Player : public TwoWaySprite{
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  void stop();
  void left();
  void right();
  void guyStand();

protected:
  float health;
  bool directionX;
  int runSpeed;
  void advanceFrame(Uint32);
};
#endif
