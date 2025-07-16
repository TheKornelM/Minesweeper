#ifndef MINESWEEPER_BASE_MENU_H
#define MINESWEEPER_BASE_MENU_H

#include "Color.h"
#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

class BaseMenuPage
{
  public:

    Adafruit_ILI9341 *display;
    Adafruit_FT6206 *ctp;

    BaseMenuPage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp);
    void clearDisplay();
    virtual void showContent() = 0;
    virtual void handleInput() = 0;
    TS_Point getMappedTouchPoint();
};

#endif