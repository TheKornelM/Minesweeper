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
  // TS_Point p = getMappedTouchPoint();
}

void MainMenuButtonWithMessagePage::drawBackToMenuButton()
{
  display->fillRect(10, 185, 220, 50, GRAY);
  display->setCursor(15, 200);
  display->print("Back to menu");
}

void MainMenuButtonWithMessagePage::drawMessageLabel()
{
  display->setCursor(15, 125);
  display->print(message);
}