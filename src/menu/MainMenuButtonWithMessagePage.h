#ifndef MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE
#define MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE

#include "BaseMenuPage.h"

#define BACK_BUTTON_X_START 10
#define BACK_BUTTON_WIDTH 220
#define BACK_BUTTON_Y_START 185
#define BACK_BUTTON_HEIGHT 50

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
    bool isBackToMenuTouched(TS_Point point);
};

#endif