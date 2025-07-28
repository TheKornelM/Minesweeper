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
  int y = 200;

  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(BUTTON_MESSAGE, 0, y, &x1, &y1, &w, &h);
  display->fillRect((display->width() - w) / 2 - x1 - 5, BACK_BUTTON_Y_START, w + 10, BACK_BUTTON_HEIGHT, GRAY);
  display->setCursor((display->width() - w) / 2 - x1, y);
  display->print(BUTTON_MESSAGE);
}

void MainMenuButtonWithMessagePage::drawMessageLabel()
{
  int y = 125;

  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(message, 0, y, &x1, &y1, &w, &h);
  display->setCursor((display->width() - w) / 2 - x1, y);
  display->print(message);
}

bool MainMenuButtonWithMessagePage::isBackToMenuTouched(TS_Point point)
{
  bool isCorrectXCoords = point.x >= BACK_BUTTON_X_START && point.x <= BACK_BUTTON_X_START + BACK_BUTTON_WIDTH;
  bool isCorrectYCoords = point.y >= BACK_BUTTON_Y_START && point.y <= BACK_BUTTON_Y_START + BACK_BUTTON_HEIGHT;

  return isCorrectXCoords && isCorrectYCoords;
}