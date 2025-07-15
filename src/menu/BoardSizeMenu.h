#ifndef MINESWEEPER_MENU_BOARD_SIZE_MENU
#define MINESWEEPER_MENU_BOARD_SIYE_MENU

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

#include "Color.h"

class BoardSizeMenu
{
  public:

    BoardSizeMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp);
    int getSize();

  private:

    Adafruit_ILI9341 *display;
    Adafruit_FT6206 *ctp;
    int getSizeFromPosition(TS_Point point);
};

#endif