#ifndef MINESWEEPER_MENU_GAME_DIFFICULTY_MENU
#define MINESWEEPER_MENU_GAME_DIFFICULTY_MENU

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>

#include "BaseMenuPage.h"
#include "Color.h"

#define VERTICAL_STACK_X_START 70
#define VERTICAL_STACK_X_END 170

#define EASY_BUTTON_Y_START 90
#define EASY_BUTTON_Y_END 140

#define MEDIUM_BUTTON_Y_START 165
#define MEDIUM_BUTTON_Y_END 215

#define HARD_BUTTON_Y_START 240
#define HARD_BUTTON_Y_END 290

class GameDifficultyMenu : public BaseMenuPage
{
  public:

    GameDifficultyMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp);
    /**
     * @brief Displays a menu for selecting the board size and waits for user input.
     *
     * @return The selected board size.
     */
    int getDifficulty();

  private:

    int getDifficultyFromPosition(TS_Point point);
    void showSizeButtons();
    void drawTitle();
    void printDifficultyLabel(int difficulty);

    virtual void showContent()
    {
    }

    virtual void handleInput()
    {
    }
};

#endif