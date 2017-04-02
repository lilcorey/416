#include <string>
#include <SDL.h>


class FrameGenerator {
public:
  FrameGenerator() : 
    frameCount(0) {}
  void makeFrame();
  unsigned int getFrameCount() const { return frameCount; }
private:
  unsigned int frameCount;
  FrameGenerator(const FrameGenerator&);
  FrameGenerator& operator=(const FrameGenerator&);
};
