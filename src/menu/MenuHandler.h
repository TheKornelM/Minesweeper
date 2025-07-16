#ifndef MINESWEEPER_MENU_HANDLER
#define MINESWEEPER_MENU_HANDLER

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

#include "BaseMenuPage.h"

class MenuHandler
{
  public:

    MenuHandler(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp);
    void showMainMenu();
    void showBoardSizeSelector();
    void showNewGameBoard(int size);
    void showFailedGame();
    void showSuccessfulGame();

  private:

    Adafruit_ILI9341 *display;
    Adafruit_FT6206 *ctp;
};

#endif