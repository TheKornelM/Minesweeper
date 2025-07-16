#ifndef MINESWEEPER_MENU_BOARD_SIZE_MENU
#define MINESWEEPER_MENU_BOARD_SIZE_MENU

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

#include "BaseMenuPage.h"
#include "Color.h"

#define VERTICAL_STACK_X_START 70
#define VERTICAL_STACK_X_END 170

#define SIZE_6_BUTTON_Y_START 90
#define SIZE_6_BUTTON_Y_END 140

#define SIZE_7_BUTTON_Y_START 165
#define SIZE_7_BUTTON_Y_END 215

#define SIZE_8_BUTTON_Y_START 240
#define SIZE_8_BUTTON_Y_END 290

class BoardSizeMenu : public BaseMenuPage
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

    int getSizeFromPosition(TS_Point point);
    void showSizeButtons();
    void drawTitle();
};

#endif