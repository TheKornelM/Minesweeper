#ifndef MINESWEEPER_BASE_MENU_H
#define MINESWEEPER_BASE_MENU_H

#include "Color.h"
#include <Adafruit_ILI9341.h>

class BaseMenuPage
{
  public:

    Adafruit_ILI9341 *display;

    BaseMenuPage(Adafruit_ILI9341 *display);
    void clearDisplay();

  private:
};

#endif