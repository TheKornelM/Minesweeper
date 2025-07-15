#ifndef MINESWEEPER_MENU_BOARD_SIZE_MENU
#define MINESWEEPER_MENU_BOARD_SIYE_MENU

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

#include "Color.h"

class BoardSizeMenu
{
  public:

    BoardSizeMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp);
    /**
     * @brief Displays a menu for selecting the board size and waits for user input.
     *
     * @return The selected board size.
     */
    int getSize();

  private:

    Adafruit_ILI9341 *display;
    Adafruit_FT6206 *ctp;
    int getSizeFromPosition(TS_Point point);
    void showSizeButtons();
    void drawTitle();
    TS_Point getMappedTouchPoint();
};

#endif