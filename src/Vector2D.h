#ifndef MINESWEEPER_VECTOR2D_H
#define MINESWEEPER_VECTOR2D_H

/**
 * @class Vector2D
 * @brief Represents a simple 2D vector with integer coordinates.
 */
class Vector2D
{
  public:

    int x; // The x-coordinate of the vector.
    int y; // The y-coordinate of the vector.

    /**
     * @brief Constructs a new Vector2D object.
     * @param x The initial x-coordinate.
     * @param y The initial y-coordinate.
     */
    Vector2D(int x, int y)
    {
      this->x = x;
      this->y = y;
    }
};

#endif
