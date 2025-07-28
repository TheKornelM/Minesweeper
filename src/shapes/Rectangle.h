#ifndef MINESWEEPER_SHAPES_RECTANGLE
#define MINESWEEPER_SHAPES_RECTANGLE

#include "Shape.h"
#include "Vector2D.h"

class Rectangle : Shape
{
  public:

    /**
     * @brief X coordinate of the top-left corner
     */
    int x;
    /**
     * @brief Y coordinate of the top-left corner
     */
    int y;
    int width;
    int height;

    Rectangle(int x, int y, int width, int height);
    bool isInside(Vector2D coords);

  private:

    Vector2D getBottomRightCoords();
};

#endif