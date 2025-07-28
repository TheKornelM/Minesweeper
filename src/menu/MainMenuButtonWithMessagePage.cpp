#include "MainMenuButtonWithMessagePage.h"

MainMenuButtonWithMessagePage::MainMenuButtonWithMessagePage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp,
                                                             String message)
    : BaseMenuPage(display, ctp)
{
  this->message = message;

  TextBounds bounds(display, BUTTON_MESSAGE, BUTTON_LABEL_Y_START);
  this->mainButton = Rectangle(bounds.getCenteredX() - 5, BACK_BUTTON_Y_START, bounds.w + 10, BACK_BUTTON_HEIGHT);
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
  display->fillRect(mainButton.x, mainButton.y, mainButton.width, mainButton.height, GRAY);

  TextBounds bounds(display, BUTTON_MESSAGE, BUTTON_LABEL_Y_START);
  int x = bounds.getCenteredX();
  display->setCursor(x, BUTTON_LABEL_Y_START);
  display->print(BUTTON_MESSAGE);
}

void MainMenuButtonWithMessagePage::drawMessageLabel()
{
  TextBounds bounds(display, message, MESSAGE_LABEL_Y_START);

  display->setCursor(bounds.getCenteredX(), MESSAGE_LABEL_Y_START);
  display->print(message);
}

bool MainMenuButtonWithMessagePage::isBackToMenuTouched(TS_Point point)
{
  return mainButton.isInside(Vector2D(point.x, point.y));
}