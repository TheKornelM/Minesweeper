/*
  Simple "Hello World" for ILI9341 LCD

  https://wokwi.com/arduino/projects/308024602434470466
*/

/*
  240 × RGB × 320(TFT)
*/

#ifndef _MAIN_
#define _MAIN_

#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#include "FieldDisplay.cpp"
#include "Minesweeper.cpp"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 display (TFT_CS, TFT_DC);
Adafruit_FT6206 ctp = Adafruit_FT6206();

/*
  Egy hely mérete 20x20 pixel.
  A tábla maximum 8x8-as lehet.
*/

int getSizeFromMenu() {
  display.fillScreen(DARKGRAY);
  display.setTextSize(2);
  display.setTextColor(ILI9341_WHITE);

  display.setCursor(50, 40);
  display.print("Tabla meret:");

  // Főmenü gombjainak megjelenítése
  for (int i = 0; i < 3; i++) {
    display.fillRect(70, (90 + (i * 75)), 100, 50, GRAY);
    display.setCursor(115, 105 + (i * 75));
    display.print(i + 6);
  }

  TS_Point point;
  int newSize = 0;
  do {
    while (!ctp.touched());
    TS_Point point = ctp.getPoint();
    point.x = map(point.x, 0, 240, 240, 0);
    point.y = map(point.y, 0, 320, 320, 0);
    newSize = getSizeFromTouch(point);
  } while (newSize == 0);
  return newSize;
}

int getSizeFromTouch(TS_Point point) {
  if (point.x >= 70 && point.x <= 170) {
    if (point.y >= 90 && point.y <= 140) {
      return 6;
    } else if (point.y > 165 && point.y < 215) {
      return 7;
    } else if (point.y > 240 && point.y < 290) {
      return 8;
    }
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  display.begin();

  if (! ctp.begin(40)) {
    Serial.println("Hiba történt az érintőképernyő inicializálása során!");
    while (1);
  }

  int size = getSizeFromMenu();
  Minesweeper table (size);
  FieldDisplay fieldDisplay(&table, &display, &ctp);
  fieldDisplay.showTable();

  do {
    fieldDisplay.samplePoint();
    delay(500);
  } while (!table.hasRevealedMine && table.remainFields > table.mineCount);

  delay(4000);
}

#endif