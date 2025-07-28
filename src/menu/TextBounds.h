#ifndef MINESWEEPER_MENU_TEXT_BOUNDS
#define MINESWEEPER_MENU_TEXT_BOUNDS

#include <Adafruit_GFX.h>
#include <Arduino.h>

class TextBounds
{
  public:

    int16_t x1, y1;
    uint16_t w, h;

    TextBounds(Adafruit_GFX *display, const String &text, int y);
    int getCenteredX() const;

  private:

    Adafruit_GFX *display;
};

#endif
