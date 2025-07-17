#ifndef MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE
#define MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE

#include "BaseMenuPage.h"

class MainMenuButtonWithMessagePage : public BaseMenuPage
{
  public:

    String message;

    MainMenuButtonWithMessagePage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp, String message);
    void showContent();
    void handleInput();

  private:

    void drawBackToMenuButton();
    void drawMessageLabel();
};

#endif