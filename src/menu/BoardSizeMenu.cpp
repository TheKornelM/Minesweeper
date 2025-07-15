#include "BoardSizeMenu.h"

BoardSizeMenu::BoardSizeMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp)
{
  this->display = display;
  this->ctp = ctp;
}

/**
 * @brief Displays a menu for selecting the board size and waits for user input.
 *
 * @return The selected board size.
 */
int BoardSizeMenu::getSize()
{
  display->fillScreen(DARKGRAY);
  display->setTextSize(2);
  display->setTextColor(ILI9341_WHITE);

  display->setCursor(50, 40);
  display->print("Board size:");

  // Displaying main menu buttons
  for (int i = 0; i < 3; i++)
  {
    display->fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display->setCursor(115, 105 + (i * 75));
    display->print(i + 6);
  }

  TS_Point point;
  int newSize = 0;
  do
  {
    while (!ctp->touched());
    TS_Point point = ctp->getPoint();
    point.x = map(point.x, 0, 240, 240, 0);
    point.y = map(point.y, 0, 320, 320, 0);
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
  if (point.x >= 70 && point.x <= 170)
  {
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
  }
  return 0;
}
