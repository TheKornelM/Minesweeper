#include "BoardSizeMenu.h"

BoardSizeMenu::BoardSizeMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp)
{
  this->display = display;
  this->ctp = ctp;
}

int BoardSizeMenu::getSize()
{
  drawTitle();
  showSizeButtons();

  int newSize = 0;
  do
  {
    TS_Point point = getMappedTouchPoint();
    newSize = getSizeFromPosition(point);
  } while (newSize == 0);

  return newSize;
}

/**
 * @brief Determines the board size based on touch coordinates from the menu.
 *
 * @param point The TS_Point object containing the touch coordinates.
 * @return The selected board size (6, 7, or 8), or 0 if no valid selection was made.
 */
int BoardSizeMenu::getSizeFromPosition(TS_Point point)
{
  // Size boxes are stacked vertically, so their X coordinates are equal.
  if (point.x < 70 || point.x > 170)
  {
    return 0;
  }

  if (point.y >= 90 && point.y <= 140)
  {
    return 6;
  }
  else if (point.y > 165 && point.y < 215)
  {
    return 7;
  }
  else if (point.y > 240 && point.y < 290)
  {
    return 8;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Displaying size buttons to select the board size.
 *
 * Sizes: 6, 7, 8
 */
void BoardSizeMenu::showSizeButtons()
{
  for (int i = 0; i < 3; i++)
  {
    display->fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display->setCursor(115, 105 + (i * 75));
    display->print(i + 6);
  }
}

/**
 * @brief Draws the title text for the board size selection menu.
 */
void BoardSizeMenu::drawTitle()
{
  display->fillScreen(DARKGRAY);
  display->setTextSize(2);
  display->setTextColor(ILI9341_WHITE);
  display->setCursor(50, 40);
  display->print("Board size:");
}

/**
 * @brief Waits for a touch input and returns the mapped screen coordinates.
 * @return A TS_Point with mapped x and y values.
 */
TS_Point BoardSizeMenu::getMappedTouchPoint()
{
  while (!ctp->touched());

  TS_Point point = ctp->getPoint();
  point.x = map(point.x, 0, 240, 240, 0);
  point.y = map(point.y, 0, 320, 320, 0);

  return point;
}
