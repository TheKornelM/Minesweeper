#include "FieldDisplay.h"

FieldDisplay::FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch)
    : BaseMenuPage(screen, touch), drawField(screen)
{
  board = fields;
}

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

void FieldDisplay::samplePoint()
{
  while (!ctp->touched());
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

void FieldDisplay::markField(TS_Point point)
{
  Vector2D fieldCoords = *getTouchedFieldArrayPosition(point);

  if (fieldCoords.x == -1 || fieldCoords.y == -1)
  {
    return;
  }

  Field *field = &board->fields[fieldCoords.y][fieldCoords.x];

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
}

int FieldDisplay::calculateY(int row)
{
  return START_Y + row * (FIELD_SIZE + FIELD_GAP);
}

int FieldDisplay::calculateX(int column)
{
  return START_X + column * (FIELD_SIZE + FIELD_GAP);
}

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

bool FieldDisplay::fieldHasTouched(TS_Point point, int row, int column)
{
  int fieldStartX = calculateX(column);
  int fieldStartY = calculateY(row);
  bool xCheck = point.x >= fieldStartX && point.x <= fieldStartX + FIELD_SIZE;
  bool yCheck = point.y >= fieldStartY && point.y <= fieldStartY + FIELD_SIZE;

  return xCheck && yCheck;
}

void FieldDisplay::displayField(int row, int column)
{
  int x = calculateX(column);
  int y = calculateY(row);
  Vector2D fieldPosition(x, y);

  Field field = board->fields[row][column];

  switch (field.state)
  {
    case UNSELECTED:
      displayUnselectedField(field, fieldPosition);
      break;
    case REVEALED:
      displayRevealedField(field, fieldPosition);
      break;
    case FLAGGED:
      displayFlaggedField(field, fieldPosition);
      break;
  }
}

void FieldDisplay::displayUnselectedField(Field field, Vector2D fieldPosition)
{
  if (board->hasRevealedMine && field.hasMine)
  {
    drawField.drawMine(fieldPosition);
  }
  else
  {
    display->fillRect(fieldPosition.x, fieldPosition.y, FIELD_SIZE, FIELD_SIZE, GRAY);
  }
}

void FieldDisplay::displayRevealedField(Field field, Vector2D fieldPosition)
{
  if (field.hasMine)
  {
    drawField.drawRevealedMine(fieldPosition);
  }
  else
  {
    drawField.drawRevealedFieldWithoutMine(fieldPosition, field.neighborMineCount);
  }
}

void FieldDisplay::displayFlaggedField(Field field, Vector2D fieldPosition)
{
  bool isWrongFlag = board->hasRevealedMine && !field.hasMine;
  drawField.drawFlag(fieldPosition, isWrongFlag ? DARKRED : GRAY);
}

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
