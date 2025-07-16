#ifndef MINESWEEPER_DRAW_FIELD_H
#define MINESWEEPER_DRAW_FIELD_H

#include <Adafruit_ILI9341.h>

#include "Color.h"
#include "DisplayDefines.h"
#include "Minesweeper.h"
#include "Vector2D.h"

/**
 * @class DrawField
 * @brief Responsible for rendering the Minesweeper game field on a TFT display.
 */
class DrawField
{
  public:

    /**
     * Pointer to the TFT display used for rendering.
     */
    Adafruit_ILI9341 *display;

    /**
     * @brief Constructs a DrawField object.
     * @param fields Pointer to the Minesweeper game logic.
     * @param display Pointer to the Adafruit_ILI9341 display object.
     */
    DrawField(Adafruit_ILI9341 *display);

    /**
     * @brief Draws a revealed field that does not contain a mine.
     * @param fieldPosition The position of the field on the board.
     * @param neighbourMines The number of mines adjacent to this field.
     */
    void drawRevealedFieldWithoutMine(Vector2D fieldPosition, int neighbourMines);

    /**
     * @brief Draws a flag icon at the specified field position.
     * @param fieldPosition The position of the field on the board.
     * @param backgroundColor The background color of the field.
     */
    void drawFlag(Vector2D fieldPosition, color backgroundColor);

    /**
     * @brief Draws a mine icon at the specified field position.
     * @param fieldPosition The position of the field on the board.
     */
    void drawMine(Vector2D fieldPosition);

    /**
     * @brief Draws a revealed mine with a red background at the specified field position.
     *
     * This indicates that the mine was triggered by the user.
     * @param fieldPosition The position of the field on the board.
     */
    void drawRevealedMine(Vector2D fieldPosition);

  private:

    /**
     * @brief Returns the color associated with a given number of neighboring mines.
     * @param neighborMines The number of adjacent mines.
     * @return A color corresponding to the number of neighboring mines.
     */
    color getColor(int neighborMines);
};

#endif
