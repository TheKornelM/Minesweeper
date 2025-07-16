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

#include "menu/MenuHandler.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 display(TFT_CS, TFT_DC);
Adafruit_FT6206 ctp = Adafruit_FT6206();

/**
 * @brief Initializes the serial communication and the random seed.
 */
void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));

  display.begin();

  if (!ctp.begin(40))
  {
    Serial.println("Error initializing touchscreen!");
    while (1);
  }
}

/**
 * @brief The main loop of the Minesweeper game.
 *
 * Initializes the display and touch screen, allows the user to select board size,
 * and then runs the game until a mine is revealed or all non-mine fields are uncovered.
 */
void loop()
{
  MenuHandler menuHandler(&display, &ctp);
  menuHandler.showMainMenu();

  delay(4000);
}

#endif