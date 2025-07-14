/**
 * @file Display.h
 * @brief Manages the display and touch interactions for the Minesweeper game.
 *
 * This file defines the FieldDisplay class, which is responsible for rendering the
 * Minesweeper game board on an Adafruit ILI9341 TFT screen and handling user
 * input from an Adafruit FT6206 capacitive touch controller.
 */

#ifndef _DISPLAY_
#define _DISPLAY_

#include "Color.cpp"
#include "Minesweeper.cpp"
#include "Vector2D.cpp"
#include <Adafruit_FT6206.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define START_Y 80     // The starting Y coordinate for the game grid.
#define START_X 5      // The starting X coordinate for the game grid.
#define MAX_HEIGHT 320 // The maximum height of the display.
#define MAX_WIDTH 240  // The maximum width of the display.
#define FIELD_SIZE 20  // The size of a single game field in pixels.
#define FIELD_GAP 10   // The gap between game fields in pixels.

/**
 * @class FieldDisplay
 * @brief Handles the graphical rendering and touch input for the Minesweeper game.
 *
 * This class interfaces with the game logic (Minesweeper) and the hardware
 * (display and touch panel) to create an interactive game experience.
 */
class FieldDisplay
{

  public:

    /**
     * @var board
     * @brief Pointer to the Minesweeper game logic object.
     */
    Minesweeper *board;

    /**
     * @var display
     * @brief Pointer to the Adafruit ILI9341 display driver.
     */
    Adafruit_ILI9341 *display;

    /**
     * @var ctp
     * @brief Pointer to the Adafruit FT6206 capacitive touch panel driver.
     */
    Adafruit_FT6206 *ctp;

    /**
     * @brief Constructs a new FieldDisplay object.
     * @param fields Pointer to the Minesweeper game instance.
     * @param screen Pointer to the Adafruit_ILI9341 display instance.
     * @param touch Pointer to the Adafruit_FT6206 touch controller instance.
     */
    FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch)
    {
      board = fields;
      display = screen;
      ctp = touch;
    }

    /**
     * @brief Redraws the entire game board based on the current state.
     */
    void showTable()
    {
      display->fillScreen(DARKGRAY);
      showRemainMines();
      for (int i = 0; i < board->size; i++)
      {
        for (int n = 0; n < board->size; n++)
        {
          displayField(i, n);
        }
      }
    }

    /**
     * @brief Samples a touch point to determine user action.
     *
     * Waits for a touch event and determines if it's a short tap (to reveal a field)
     * or a long press (to place a flag).
     */
    void samplePoint()
    {
      // Wait for a touch.
      while (!ctp->touched());
      // A short touch reveals the field, a long touch places a flag.
      TS_Point point;
      int i;
      for (i = 0; i < 15 && ctp->touched(); i++)
      {
        point = getPoint();
        delay(50);
      }

      if (i < 10)
      {
        markField(point);
      }
      else
      {
        flagField(point);
      }
    }

    /**
     * @brief Reveals the field at the specified touch point.
     * @param point The TS_Point object representing the touch coordinates.
     */
    void markField(TS_Point point)
    {
      Vector2D fieldCoords = *getTouchedFieldArrayPosition(point);

      if (fieldCoords.x == -1 || fieldCoords.y == -1)
      {
        return;
      }

      Field *field = &board->fields[fieldCoords.y][fieldCoords.x];

      // If the field is already revealed or flagged, do nothing.
      if (field->state == REVEALED || field->state == FLAGGED)
      {
        return;
      }

      board->selectField(fieldCoords.y, fieldCoords.x);
      board->unrevealField(fieldCoords.y, fieldCoords.x);

      if (field->neighborMineCount == 0 || board->hasRevealedMine)
      {
        showTable();
      }
      else
      {
        displayField(fieldCoords.y, fieldCoords.x);
      }
    }

    /**
     * @brief Toggles a flag on the field at the specified touch point.
     * @param point The TS_Point object representing the touch coordinates.
     */
    void flagField(TS_Point point)
    {
      Vector2D coords = *getTouchedFieldArrayPosition(point);

      if (coords.x == -1 || coords.y == -1)
      {
        return;
      }

      board->changeFlag(coords.y, coords.x);
      displayField(coords.y, coords.x);
      showRemainMines();

      return;
    }

  private:

    /**
     * @brief Calculates the top-left Y coordinate for a given field row.
     * @param row The row index of the field.
     * @return The calculated Y coordinate on the screen.
     */
    int calculateY(int row)
    {
      return START_Y + row * (FIELD_SIZE + FIELD_GAP);
    }

    /**
     * @brief Calculates the top-left X coordinate for a given field column.
     * @param column The column index of the field.
     * @return The calculated X coordinate on the screen.
     */
    int calculateX(int column)
    {
      return START_X + column * (FIELD_SIZE + FIELD_GAP);
    }

    /**
     * @brief Converts raw touch coordinates into a grid position on the game board.
     * @param point The TS_Point object representing the touch coordinates.
     * @return A Vector2D pointer with the (x, y) grid coordinates. Returns (-1, -1) if no field was touched.
     */
    Vector2D *getTouchedFieldArrayPosition(TS_Point point)
    {
      for (int i = 0; i < board->size; i++)
      {
        for (int j = 0; j < board->size; j++)
        {
          if (fieldHasTouched(point, i, j))
          {
            return new Vector2D(j, i);
          }
        }
      }

      return new Vector2D(-1, -1);
    }

    /**
     * @brief Determines if a touch point is within the boundaries of a specific field.
     * @param point The TS_Point object representing the touch coordinates.
     * @param row The row index of the field to check.
     * @param column The column index of the field to check.
     * @return True if the touch point is within the field, false otherwise.
     */
    bool fieldHasTouched(TS_Point point, int row, int column)
    {
      int fieldStartX = calculateX(column);
      int fieldStartY = calculateY(row);
      bool xCheck = point.x >= fieldStartX && point.x <= fieldStartX + FIELD_SIZE;
      bool yCheck = point.y >= fieldStartY && point.y <= fieldStartY + FIELD_SIZE;

      return xCheck && yCheck;
    }

    /**
     * @brief Draws a flag icon at the specified screen coordinates.
     * @param xCoord The top-left X coordinate of the field.
     * @param yCoord The top-left Y coordinate of the field.
     */
    void displayFlag(int xCoord, int yCoord)
    {
      // Flagpole
      display->fillRect(xCoord + 14, yCoord + 12, 1, 5, BLACK);
      // Flag base
      display->fillRect(xCoord + 9, yCoord + 17, 7, 1, BLACK);
      // Flag banner
      display->fillTriangle(xCoord + 2, yCoord + 7, xCoord + 14, yCoord + 2, xCoord + 14, yCoord + 11, RED);
    }

    /**
     * @brief Draws a mine icon at the specified screen coordinates.
     * @param xCoord The top-left X coordinate of the field.
     * @param yCoord The top-left Y coordinate of the field.
     */
    void displayMine(int xCoord, int yCoord)
    {
      display->fillCircle(xCoord + FIELD_SIZE / 2, yCoord + FIELD_SIZE / 2, FIELD_SIZE * 0.3, BLACK);
    }

    /**
     * @brief Draws a single field based on its current state (unselected, revealed, flagged).
     * @param row The row index of the field.
     * @param column The column index of the field.
     */
    void displayField(int row, int column)
    {
      switch (board->fields[row][column].state)
      {
        case UNSELECTED:
          displayUnselectedField(row, column);
          break;
        case REVEALED:
          displayRevealedField(row, column);
          break;
        case FLAGGED:
          displayFlaggedField(row, column);
          break;
      }
    }

    /**
     * @brief Renders an unrevealed field.
     * @param row The row index of the field.
     * @param column The column index of the field.
     */
    void displayUnselectedField(int row, int column)
    {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->hasRevealedMine && board->fields[row][column].hasMine)
      {
        displayMine(xCoord, yCoord);
      }
      else
      {
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, GRAY);
      }
    }

    /**
     * @brief Renders a revealed field, showing either a mine or the number of neighboring mines.
     * @param row The row index of the field.
     * @param column The column index of the field.
     */
    void displayRevealedField(int row, int column)
    {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->fields[row][column].hasMine)
      {
        // The mine revealed by the user is shown on a red background.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, RED);
        displayMine(xCoord, yCoord);
      }
      else
      {
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, DARKGRAY);
        display->setCursor(xCoord + 2, yCoord);
        display->setTextColor(getColor(board->fields[row][column].neighborMineCount));
        display->setTextSize(3);
        display->println(board->fields[row][column].neighborMineCount);
      }
    }

    /**
     * @brief Renders a field that has been marked with a flag by the user.
     * @param row The row index of the field.
     * @param column The column index of the field.
     */
    void displayFlaggedField(int row, int column)
    {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->hasRevealedMine && !board->fields[row][column].hasMine)
      {
        // At the end of the game, a wrongly placed flag is shown on a dark red background.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, DARKRED);
      }
      else
      {
        // A correctly flagged field is shown on a gray background.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, GRAY);
      }
      displayFlag(xCoord, yCoord);
    }

    /**
     * @brief Gets the appropriate color for the number of neighboring mines.
     * @param neighborMinesCount The number of mines adjacent to the field.
     * @return The color value for the text.
     */
    color getColor(int neighborMinesCount)
    {
      switch (neighborMinesCount)
      {
        case 1:
          return BLUE;
        case 2:
          return GREEN;
        case 3:
          return RED;
        case 4:
          return DARKBLUE;
        case 5:
          return DARKRED;
        case 6:
          return CYAN;
        case 7:
          return BLACK;
        case 8:
          return GRAY;
        default:
          return DARKGRAY;
      }
    }

    /**
     * @brief Waits for and retrieves a touch point, mapping it to the screen coordinates.
     * @return A TS_Point object with the mapped (x, y) coordinates of the touch.
     */
    TS_Point getPoint()
    {
      while (!ctp->touched());
      TS_Point point = ctp->getPoint();
      point.x = map(point.x, 0, 240, 240, 0);
      point.y = map(point.y, 0, 320, 320, 0);
      return point;
    }

    /**
     * @brief Displays the remaining number of mines to be found.
     *
     * This is calculated by subtracting the number of placed flags from the total mine count.
     */
    void showRemainMines()
    {
      display->setCursor(START_X, 20);
      display->setTextColor(GRAY);
      display->setTextSize(2);
      display->println("Mines left:");
      display->fillRect(START_X, 40, MAX_WIDTH - 50, 30, DARKGRAY);
      display->setCursor(START_X, 40);
      display->setTextColor(GRAY);
      display->print(board->getRemainFieldsByFlags());
      display->print(" ");
    }
};

#endif
