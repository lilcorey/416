#include "twowaysprite.h"

void TwoWaySprite::advanceFrame(Uint32 ticks){
	timeSinceLastFrame += ticks;
	if(timeSinceLastFrame > frameInterval){
		if(direction == 0){
			currentFrame = (currentFrame+1) % (numberOfFrames/2);
		}
		else{
			currentFrame = ((currentFrame-1) % (numberOfFrames/2) )+ 8;
		}
		timeSinceLastFrame = 0;
	}
}

TwoWaySprite::TwoWaySprite(const std::string& name) :
	MultiSprite(name), direction(0), gravity(14.0), onGround(false)
	{}

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
	MultiSprite(s), direction(s.direction), gravity(s.gravity), onGround(s.onGround)
	{}

void TwoWaySprite::update(Uint32 ticks){
  advanceFrame(ticks);
	//accelerate faster on falling
	if(getVelocityY() > 0) setVelocityY(getVelocityY() + gravity*1.5);
	else setVelocityY(getVelocityY() + gravity);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-(frameHeight*2)) {
    setVelocityY(0);
		setY(worldHeight-(frameHeight*2));
		onGround = true;
  }

  if ( getX() < 0) {
  	dirRight();
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
  	dirLeft();
    setVelocityX( -fabs( getVelocityX() ) );
  }
}
void TwoWaySprite::startJump(){
	if(onGround){
		std::cout << "startJump" << '\n';
		setVelocityY(-400.0);
		onGround = false;
	}
}

void TwoWaySprite::endJump(){

	if(getVelocityY() < -15.0){
		setVelocityY(-15.0);
	}
}
