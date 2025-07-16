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
  for (int i = 0; i < 3; i++)
  {
    display->fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display->setCursor(115, 105 + (i * 75));
    display->print(message);
  }
}

void MainMenuButtonWithMessagePage::handleInput()
{
}