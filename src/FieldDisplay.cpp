/**
 * @file FieldDisplay.cpp
 * @brief Manages the display and touch interactions for the Minesweeper game.
 *
 * This file defines the FieldDisplay class, which is responsible for rendering the
 * Minesweeper game board on an Adafruit ILI9341 TFT screen and handling user
 * input from an Adafruit FT6206 capacitive touch controller.
 */

#include "FieldDisplay.h"

/**
 * @brief Constructs a new FieldDisplay object.
 * @param fields Pointer to the Minesweeper game instance.
 * @param screen Pointer to the Adafruit_ILI9341 display instance.
 * @param touch Pointer to the Adafruit_FT6206 touch controller instance.
 */
FieldDisplay::FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch)
    : BaseMenuPage(screen, touch), drawField(screen)
{
  board = fields;
}

/**
 * @brief Redraws the entire game board based on the current state.
 */
void FieldDisplay::showTable()
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
void FieldDisplay::samplePoint()
{
  // Wait for a touch.
  while (!ctp->touched());
  // A short touch reveals the field, a long touch places a flag.
  TS_Point point;
  int i;
  for (i = 0; i < 15 && ctp->touched(); i++)
  {
    point = getMappedTouchPoint();
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
void FieldDisplay::markField(TS_Point point)
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
void FieldDisplay::flagField(TS_Point point)
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

/**
 * @brief Calculates the top-left Y coordinate for a given field row.
 * @param row The row index of the field.
 * @return The calculated Y coordinate on the screen.
 */
int FieldDisplay::calculateY(int row)
{
  return START_Y + row * (FIELD_SIZE + FIELD_GAP);
}

/**
 * @brief Calculates the top-left X coordinate for a given field column.
 * @param column The column index of the field.
 * @return The calculated X coordinate on the screen.
 */
int FieldDisplay::calculateX(int column)
{
  return START_X + column * (FIELD_SIZE + FIELD_GAP);
}

/**
 * @brief Converts raw touch coordinates into a grid position on the game board.
 * @param point The TS_Point object representing the touch coordinates.
 * @return A Vector2D pointer with the (x, y) grid coordinates. Returns (-1, -1) if no field was touched.
 */
Vector2D *FieldDisplay::getTouchedFieldArrayPosition(TS_Point point)
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
bool FieldDisplay::fieldHasTouched(TS_Point point, int row, int column)
{
  int fieldStartX = calculateX(column);
  int fieldStartY = calculateY(row);
  bool xCheck = point.x >= fieldStartX && point.x <= fieldStartX + FIELD_SIZE;
  bool yCheck = point.y >= fieldStartY && point.y <= fieldStartY + FIELD_SIZE;

  return xCheck && yCheck;
}

/**
 * @brief Draws a single field based on its current state (unselected, revealed, flagged).
 * @param row The row index of the field.
 * @param column The column index of the field.
 */
void FieldDisplay::displayField(int row, int column)
{
  int x = calculateX(column);
  int y = calculateY(row);
  Vector2D fieldPosition(x, y);

  switch (board->fields[row][column].state)
  {
    case UNSELECTED:
      displayUnselectedField(row, column, fieldPosition);
      break;
    case REVEALED:
      displayRevealedField(row, column, fieldPosition);
      break;
    case FLAGGED:
      displayFlaggedField(row, column, fieldPosition);
      break;
  }
}

/**
 * @brief Renders an unrevealed field.
 * @param row The row index of the field.
 * @param column The column index of the field.
 */
void FieldDisplay::displayUnselectedField(int row, int column, Vector2D fieldPosition)
{
  if (board->hasRevealedMine && board->fields[row][column].hasMine)
  {
    drawField.drawMine(fieldPosition);
  }
  else
  {
    display->fillRect(fieldPosition.x, fieldPosition.y, FIELD_SIZE, FIELD_SIZE, GRAY);
  }
}

/**
 * @brief Renders a revealed field, showing either a mine or the number of neighboring mines.
 * @param row The row index of the field.
 * @param column The column index of the field.
 */
void FieldDisplay::displayRevealedField(int row, int column, Vector2D fieldPosition)
{
  if (board->fields[row][column].hasMine)
  {
    drawField.drawRevealedMine(fieldPosition);
  }
  else
  {
    drawField.drawRevealedFieldWithoutMine(fieldPosition, board->fields[row][column].neighborMineCount);
  }
}

/**
 * @brief Renders a field that has been marked with a flag by the user.
 * @param row The row index of the field.
 * @param column The column index of the field.
 */
void FieldDisplay::displayFlaggedField(int row, int column, Vector2D fieldPosition)
{
  bool isWrongFlag = board->hasRevealedMine && !board->fields[row][column].hasMine;
  drawField.drawFlag(fieldPosition, isWrongFlag ? DARKRED : GRAY);
}

/**
 * @brief Displays the remaining number of mines to be found.
 *
 * This is calculated by subtracting the number of placed flags from the total mine count.
 */
void FieldDisplay::showRemainMines()
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