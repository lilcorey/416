#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H

#include <iostream>
#include "sprite.h"
#include "twowaysprite.h"
#include "player.h"

// SmartSprite is an observer of subject SubjectSprite, which
// in this example is a spinning star.

class SmartSprite : public TwoWaySprite {
public:
  SmartSprite(const std::string&, Player*);
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  Vector2f makeVelocity(int, int) const;

private:
  Player* player;

  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};
#endif
