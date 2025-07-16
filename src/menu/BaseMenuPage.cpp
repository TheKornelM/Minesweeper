#include "BaseMenuPage.h"

BaseMenuPage::BaseMenuPage(Adafruit_ILI9341 *display)
{
  this->display = display;
}

void BaseMenuPage::clearDisplay()
{
  display->fillScreen(DARKGRAY);
}
