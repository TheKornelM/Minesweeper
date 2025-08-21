#include "GameDifficultyMenu.h"

GameDifficultyMenu::GameDifficultyMenu(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp) : BaseMenuPage(display, ctp)
{
}

int GameDifficultyMenu::getDifficulty()
{
  drawTitle();
  showDifficultyButtons();

  int newSize = 0;
  do
  {
    TS_Point point = getMappedTouchPoint();
    newSize = getDifficultyFromPosition(point);
  } while (newSize == 0);

  return newSize;
}

/**
 * @brief Determines the game difficulty based on touch coordinates from the menu.
 *
 * @param point The TS_Point object containing the touch coordinates.
 * @return The selected difficulty (easy, medium, hard), or 0 if no valid selection was made.
 */
int GameDifficultyMenu::getDifficultyFromPosition(TS_Point point)
{
  // Size boxes are stacked vertically, so their X coordinates are equal.
  if (point.x < VERTICAL_STACK_X_START || point.x > VERTICAL_STACK_X_END)
  {
    return 0;
  }

  if (point.y >= EASY_BUTTON_Y_START && point.y <= EASY_BUTTON_Y_END)
  {
    return 6;
  }
  else if (point.y >= MEDIUM_BUTTON_Y_START && point.y <= MEDIUM_BUTTON_Y_END)
  {
    return 7;
  }
  else if (point.y >= HARD_BUTTON_Y_START && point.y <= HARD_BUTTON_Y_END)
  {
    return 8;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Displaying size buttons to select the board size.
 *
 * Sizes: 6, 7, 8
 */
void GameDifficultyMenu::showDifficultyButtons()
{
  for (int i = 0; i < 3; i++)
  {
    String difficulty = getDifficultyString(i);
    TextBounds currentDifficultyBounds(display, difficulty, 115);

    display->fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display->setCursor(currentDifficultyBounds.getCenteredX();, 105 + (i * 75));
    display->print(difficulty);
  }
}

/**
 * @brief Draws the title text for the board size selection menu.
 */
void GameDifficultyMenu::drawTitle()
{
  clearDisplay();
  display->setTextSize(2);
  display->setTextColor(ILI9341_WHITE);

  String message = "Select difficulty";
  TextBounds difficultyBounds(display, message, 40);
  display->setCursor(difficultyBounds.getCenteredX(), 40);
  display->print(message);
}

String GameDifficultyMenu::getDifficultyString(int difficulty)
{
  String message;
  switch (difficulty)
  {
    case 0:
      message = "Easy";
      break;
    case 1:
      message = "Medium";
      break;
    case 2:
      message = "Hard";
      break;
  }

  return message;
}