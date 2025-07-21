#include "MainMenuButtonWithMessagePage.h"

MainMenuButtonWithMessagePage::MainMenuButtonWithMessagePage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp,
                                                             String message)
    : BaseMenuPage(display, ctp)
{
  this->message = message;
}

void MainMenuButtonWithMessagePage::showContent()
{
  clearDisplay();
  display->setTextColor(ILI9341_WHITE);
  drawBackToMenuButton();
  drawMessageLabel();
}

void MainMenuButtonWithMessagePage::handleInput()
{
  TS_Point p;
  do
  {
    p = getMappedTouchPoint();
  } while (!isBackToMenuTouched(p));
}

void MainMenuButtonWithMessagePage::drawBackToMenuButton()
{
  display->fillRect(BACK_BUTTON_X_START, BACK_BUTTON_Y_START, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT, GRAY);
  display->setCursor(15, 200);
  display->print("Back to menu");
}

void MainMenuButtonWithMessagePage::drawMessageLabel()
{
  display->setCursor(15, 125);
  display->print(message);
}

bool MainMenuButtonWithMessagePage::isBackToMenuTouched(TS_Point point)
{
  bool isCorrectXCoords = point.x >= BACK_BUTTON_X_START && point.x <= BACK_BUTTON_X_START + BACK_BUTTON_WIDTH;
  bool isCorrectYCoords = point.y >= BACK_BUTTON_Y_START && point.y <= BACK_BUTTON_Y_START + BACK_BUTTON_HEIGHT;

  return isCorrectXCoords && isCorrectYCoords;
}