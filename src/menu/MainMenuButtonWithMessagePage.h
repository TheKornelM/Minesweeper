#ifndef MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE
#define MINESWEEPER_MAIN_MENU_BUTTON_WITH_MESSAGE_PAGE

#include "BaseMenuPage.h"
#include "TextBounds.h"
#include "shapes/Rectangle.h"

#define MESSAGE_LABEL_Y_START 125

#define BACK_BUTTON_Y_START 185
#define BACK_BUTTON_HEIGHT 50

#define BUTTON_LABEL_Y_START 200
#define BUTTON_MESSAGE "Back to menu"

class MainMenuButtonWithMessagePage : public BaseMenuPage
{
  public:

    String message;

    MainMenuButtonWithMessagePage(Adafruit_ILI9341 *display, Adafruit_FT6206 *ctp, String message);
    void showContent();
    void handleInput();

  private:

    Rectangle mainButton = Rectangle(0, 0, 0, 0);

    void drawBackToMenuButton();
    void drawMessageLabel();
    bool isBackToMenuTouched(TS_Point point);
};

#endif