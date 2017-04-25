#ifndef TWOWAYSPRITE_H
#define TWOWAYSPRITE_H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "multisprite.h"

class TwoWaySprite : public MultiSprite {
public:
	TwoWaySprite(const std::string&);
	TwoWaySprite(const TwoWaySprite&);
	virtual void update(Uint32 ticks);
	unsigned int getDir() const { return direction; }
	void switchDir() { direction ? direction=0 : direction=1; }
	void dirLeft() { direction = 1; setVelocityX( -fabs( getVelocityX() ) ); }
	void dirRight() { direction = 0; setVelocityX( fabs( getVelocityX() ) ); }

	void startJump();
	void endJump();

protected:
	unsigned int direction;
	float gravity;
	bool onGround;
	void advanceFrame(Uint32 ticks);
};
#endif
