
#include "Minesweeper.h"

Minesweeper::Minesweeper(int s)
{
  size = s;
  mineCount = 0;
  hasRevealedMine = false;
  remainFields = size * size;
  mineCount = (size * size) * 0.16;
  flaggedFields = 0;
  allocateFields();
}

Minesweeper::~Minesweeper()
{
  freeFields();
}

bool Minesweeper::fieldExists(int row, int column)
{
  return row >= 0 && column >= 0 && row < size && column < size;
}

void Minesweeper::selectField(int row, int column)
{
  /* The user cannot select a previously revealed or flagged field.
     It must be unmarked first. */
  if (fields[row][column].state == REVEALED || fields[row][column].state == FLAGGED)
    return;

  if (fields[row][column].hasMine)
  {
    hasRevealedMine = true;
    fields[row][column].state = REVEALED;
  }

  /* Mines are generated only after the first move
     to prevent the user from stepping on a mine immediately. */
  if (remainFields == size * size)
  {
    generateMines(row, column);
  }
}

void Minesweeper::unrevealField(int row, int column)
{
  /* If the field is invalid (e.g., negative index) or
     already revealed, recursion stops. */
  if (!fieldExists(row, column) || fields[row][column].state == REVEALED)
  {
    return;
  }

  if (fields[row][column].state == FLAGGED)
  {
    flaggedFields--;
  }

  fields[row][column].state = REVEALED;
  remainFields--;

  /* If the field has neighboring mines, it is revealed
     and recursion ends. */
  if (fields[row][column].neighborMineCount != 0)
  {
    return;
  }

  // Reveal neighboring fields.
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      unrevealField(row + i, column + j);
    }
  }
}

void Minesweeper::changeFlag(int row, int column)
{
  if (fields[row][column].state == FLAGGED)
  {
    fields[row][column].state = UNSELECTED;
    flaggedFields--;
  }
  else if (fields[row][column].state == UNSELECTED)
  {
    fields[row][column].state = FLAGGED;
    flaggedFields++;
  }
}

int Minesweeper::getRemainFieldsByFlags()
{
  return mineCount - flaggedFields;
}

void Minesweeper::allocateFields()
{
  fields = NULL;
  fields = new Field *[size];
  checkSuccessfulAllocation(fields);
  for (int i = 0; i < size; i++)
  {
    fields[i] = NULL;
    fields[i] = new Field[size];
    checkSuccessfulAllocation(fields[i]);
  }
}

void Minesweeper::freeFields()
{
  for (int i = 0; i < size; i++)
  {
    delete (fields[i]);
    fields[i] = NULL;
  }
  delete (fields);
  fields = NULL;
}

void Minesweeper::checkSuccessfulAllocation(void *pointer)
{
  if (pointer == NULL)
  {
    Serial.println("ERROR: Memory allocation failure occurred during program execution!");
    delay(10000);
    while (true);
  }
}

void Minesweeper::generateMines(int firstStepRow, int firstStepColumn)
{
  for (int i = 0; i < mineCount;)
  {
    int row = random(0, size);
    int column = random(0, size);
    if (!fields[row][column].hasMine && !(row == firstStepRow && column == firstStepColumn))
    {
      fields[row][column].hasMine = true;
      i++;
      for (int j = -1; j <= 1; j++)
      {
        for (int k = -1; k <= 1; k++)
        {
          if (fieldExists(row + j, column + k) && !(j == 0 && k == 0))
          {
            fields[row + j][column + k].neighborMineCount++;
          }
        }
      }
    }
  }
}
