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

// Constants for layout
#define START_Y 80
#define START_X 5
#define MAX_HEIGHT 320
#define MAX_WIDTH 240
#define FIELD_GAP 10

/**
 * @class FieldDisplay
 * @brief Handles the graphical rendering and touch input for the Minesweeper game.
 *
 * This class is responsible for drawing the game board on an Adafruit ILI9341 TFT display
 * and processing user input from an Adafruit FT6206 capacitive touch controller.
 */
class FieldDisplay : public BaseMenuPage
{
  public:

    /**
     * Pointer to the Minesweeper game logic.
     */
    Minesweeper *board;

    /**
     * @brief Constructs a FieldDisplay object.
     * @param fields Pointer to the Minesweeper game instance.
     * @param screen Pointer to the TFT display.
     * @param touch Pointer to the capacitive touch controller.
     */
    FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch);

    /**
     * @brief Redraws the entire game board based on the current game state.
     */
    void showTable();

    /**
     * @brief Waits for and processes a touch input.
     *
     * A short tap reveals a field, while a long press places or removes a flag.
     */
    void samplePoint();

    /**
     * @brief Reveals the field at the touched location.
     * @param point The touch point on the screen.
     */
    void markField(TS_Point point);

    /**
     * @brief Toggles a flag on the field at the touched location.
     * @param point The touch point on the screen.
     */
    void flagField(TS_Point point);

  private:

    /**
     * Helper class for drawing individual fields.
     */
    DrawField drawField;

    /**
     * @brief Calculates the Y coordinate of a field based on its row index.
     * @param row The row index.
     * @return The Y coordinate on the screen.
     */
    int calculateY(int row);

    /**
     * @brief Calculates the X coordinate of a field based on its column index.
     * @param column The column index.
     * @return The X coordinate on the screen.
     */
    int calculateX(int column);

    /**
     * @brief Draws a single field based on its current state.
     * @param row The row index of the field.
     * @param column The column index of the field.
     */
    void displayField(int row, int column);

    /**
     * @brief Draws an unrevealed field.
     * @param field The field data.
     * @param fieldPosition The top-left screen position of the field.
     */
    void displayUnselectedField(Field field, Vector2D fieldPosition);

    /**
     * @brief Converts a touch point to a grid coordinate.
     * @param point The touch point.
     * @return A pointer to a Vector2D containing the grid coordinates, or (-1, -1) if invalid.
     */
    Vector2D *getTouchedFieldArrayPosition(TS_Point point);

    /**
     * @brief Checks if a touch point is within a specific field.
     * @param point The touch point.
     * @param row The row index of the field.
     * @param column The column index of the field.
     * @return True if the point is within the field, false otherwise.
     */
    bool fieldHasTouched(TS_Point point, int row, int column);

    /**
     * @brief Draws a revealed field, showing either a mine or a number.
     * @param field The field data.
     * @param fieldPosition The top-left screen position of the field.
     */
    void displayRevealedField(Field field, Vector2D fieldPosition);

    /**
     * @brief Draws a flagged field.
     * @param field The field data.
     * @param fieldPosition The top-left screen position of the field.
     */
    void displayFlaggedField(Field field, Vector2D fieldPosition);

    /**
     * @brief Displays the number of remaining mines.
     */
    void showRemainMines();
};

#endif
