#ifndef MINESWEEPER_SHAPES_SHAPE
#define MINESWEEPER_SHAPES_SHAPE

#include "Vector2D.h"

class Shape
{
  public:

    virtual bool isInside(Vector2D coords) = 0;
};

#endif