#include "TextBounds.h"

TextBounds::TextBounds(Adafruit_GFX *display, const String &text, int y) : display(display)
{
  display->getTextBounds(text, 0, y, &x1, &y1, &w, &h);
}

int TextBounds::getCenteredX() const
{
  return (display->width() - w) / 2 - x1;
}