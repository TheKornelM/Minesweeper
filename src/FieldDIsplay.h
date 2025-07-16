#ifndef MINESWEEPER_FIELD_DISPLAY_H
#define MINESWEEPER_FIELD_DISPLAY_H

#include <Adafruit_FT6206.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#include "Color.h"
#include "DisplayDefines.h"
#include "DrawField.h"
#include "Minesweeper.h"
#include "Vector2D.h"
#include "menu/BaseMenuPage.h"

#define START_Y 80
#define START_X 5
#define MAX_HEIGHT 320
#define MAX_WIDTH 240
#define FIELD_GAP 10

/**
 * @class FieldDisplay
 * @brief Handles the graphical rendering and touch input for the Minesweeper game.
 */
class FieldDisplay : public BaseMenuPage
{
  public:

    Minesweeper *board;

    FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch);
    void showTable();
    void samplePoint();
    void markField(TS_Point point);
    void flagField(TS_Point point);

  private:

    DrawField drawField;

    int calculateY(int row);
    int calculateX(int column);
    void displayField(int row, int column);
    void displayUnselectedField(int row, int column, Vector2D fieldPosition);
    Vector2D *getTouchedFieldArrayPosition(TS_Point point);
    bool fieldHasTouched(TS_Point point, int row, int column);
    void displayRevealedField(int row, int column, Vector2D fieldPosition);
    void displayFlaggedField(int row, int column, Vector2D fieldPosition);
    void showRemainMines();
};

#endif
