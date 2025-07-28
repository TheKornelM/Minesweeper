#include "BaseMenuPage.h"

BaseMenuPage::BaseMenuPage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp)
{
  this->display = display;
  this->ctp = ctp;
}

void BaseMenuPage::clearDisplay()
{
  display->fillScreen(DARKGRAY);
}

/**
 * @brief Waits for a touch input and returns the mapped screen coordinates.
 * @return A TS_Point with mapped x and y values.
 */
TS_Point BaseMenuPage::getMappedTouchPoint()
{
  while (!ctp->touched());

  TS_Point point = ctp->getPoint();
  point.x = map(point.x, 0, 240, 240, 0);
  point.y = map(point.y, 0, 320, 320, 0);

  return point;
}