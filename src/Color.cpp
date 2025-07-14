#ifndef COLOR
#define COLOR

#include <Adafruit_ILI9341.h>

enum color
{
  BLUE = ILI9341_BLUE,
  RED = 0xc800,
  GREEN = 0x0500,
  DARKBLUE = 0x000f,
  DARKRED = 0xa000,
  CYAN = 0x0515,
  BLACK = ILI9341_BLACK,
  GRAY = 0x8430,
  DARKGRAY = 0x5AEB
};

#endif