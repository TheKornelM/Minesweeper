#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

Vector2D Rectangle::getBottomRightCoords()
{
  return Vector2D(x + width, y + height);
}

bool Rectangle::isInside(Vector2D point)
{
  bool isCorrectXCoords = point.x >= x && point.x <= x + width;
  bool isCorrectYCoords = point.y >= y && point.y <= y + height;

  return isCorrectXCoords && isCorrectYCoords;
}