/*
  240 × RGB × 320 (TFT)
*/

#ifndef MINESWEEPER_MAIN
#define MINESWEEPER_MAIN

#include "SPI.h"
#include <Adafruit_FT6206.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

#include "Color.h"
#include "FieldDisplay.h"
#include "Minesweeper.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 display(TFT_CS, TFT_DC);
Adafruit_FT6206 ctp = Adafruit_FT6206();

/**
 * @brief Determines the board size based on touch coordinates from the menu.
 *
 * @param point The TS_Point object containing the touch coordinates.
 * @return The selected board size (6, 7, or 8), or 0 if no valid selection was made.
 */
int getSizeFromTouch(TS_Point point)
{
  if (point.x >= 70 && point.x <= 170)
  {
    if (point.y >= 90 && point.y <= 140)
    {
      return 6;
    }
    else if (point.y > 165 && point.y < 215)
    {
      return 7;
    }
    else if (point.y > 240 && point.y < 290)
    {
      return 8;
    }
  }
  return 0;
}

/*
  The size of one field is 20x20 pixels.
  The maximum board size is 8x8.
*/

/**
 * @brief Displays a menu for selecting the board size and waits for user input.
 *
 * @return The selected board size.
 */
int getSizeFromMenu()
{
  display.fillScreen(DARKGRAY);
  display.setTextSize(2);
  display.setTextColor(ILI9341_WHITE);

  display.setCursor(50, 40);
  display.print("Board size:");

  // Displaying main menu buttons
  for (int i = 0; i < 3; i++)
  {
    display.fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display.setCursor(115, 105 + (i * 75));
    display.print(i + 6);
  }

  TS_Point point;
  int newSize = 0;
  do
  {
    while (!ctp.touched());
    TS_Point point = ctp.getPoint();
    point.x = map(point.x, 0, 240, 240, 0);
    point.y = map(point.y, 0, 320, 320, 0);
    newSize = getSizeFromTouch(point);
  } while (newSize == 0);

  return newSize;
}

/**
 * @brief Initializes the serial communication and the random seed.
 */
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

/**
 * @brief The main loop of the Minesweeper game.
 *
 * Initializes the display and touch screen, allows the user to select board size,
 * and then runs the game until a mine is revealed or all non-mine fields are uncovered.
 */
void loop()
{
  display.begin();

  if (!ctp.begin(40))
  {
    Serial.println("Error initializing touchscreen!");
    while (1);
  }

  int size = getSizeFromMenu();
  Minesweeper table(size);
  FieldDisplay fieldDisplay(&table, &display, &ctp);
  fieldDisplay.showTable();

  do
  {
    fieldDisplay.samplePoint();
    delay(500);
  } while (!table.hasRevealedMine && table.remainFields > table.mineCount);

  delay(4000);
}

#endif