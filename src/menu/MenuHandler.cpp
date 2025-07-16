#include "MenuHandler.h"
#include "BoardSizeMenu.h"
#include <FieldDisplay.h>
#include <Minesweeper.h>

MenuHandler::MenuHandler(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp)
{
  this->display = display;
  this->ctp = ctp;
}

void MenuHandler::showMainMenu()
{
  showBoardSizeSelector();
}

void MenuHandler::showBoardSizeSelector()
{
  BoardSizeMenu sizeMenu(display, ctp);
  int size = sizeMenu.getSize();
  showNewGameBoard(size);
}

void MenuHandler::showNewGameBoard(int size)
{
  Minesweeper table(size);

  FieldDisplay fieldDisplay(&table, display, ctp);
  fieldDisplay.showTable();

  do
  {
    fieldDisplay.samplePoint();
    delay(500);
  } while (!table.hasRevealedMine && table.remainFields > table.mineCount);

  if (table.hasRevealedMine)
  {
    showFailedGame();
  }
  else if (table.remainFields <= table.mineCount)
  {
    showSuccessfulGame();
  }
}

void MenuHandler::showFailedGame()
{
  // TODO: implement failed game screen
}

void MenuHandler::showSuccessfulGame()
{
  // TODO: implement successful game screen
}
