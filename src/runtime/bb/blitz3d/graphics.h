#ifndef B3DGRAPHICS_H
#define B3DGRAPHICS_H

#include "mesh.h"
#include "scene.h"

class B3DGraphics{
public:
  virtual BBScene *createScene( int flags )=0;
  virtual BBScene *verifyScene( BBScene *scene )=0;
  virtual void freeScene( BBScene *scene )=0;
};

extern B3DGraphics *bbSceneDriver;

#endif
