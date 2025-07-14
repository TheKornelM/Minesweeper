#ifndef _MINESWEEPER_
#define _MINESWEEPER_

#include <Arduino.h>
#include "Field.cpp"

/**
 * @class Minesweeper
 * @brief Stores the data required for the Minesweeper game.
 *
 * Fields:
 * - size: The size of the board (size * size)
 * - mineCount: The number of mines in the game, which depends on the generated board size
 * - remainFields: The number of remaining unrevealed fields.
 *                 If this equals mineCount, the game is won.
 * - flaggedFields: The number of fields marked with a flag
 * - hasRevealedMine: Indicates whether the player has revealed a mine (true: yes, false: no)
 * - fields: A dynamically allocated matrix storing the fields
 */
class Minesweeper
{
  public:
    int size;
    int mineCount;
    int remainFields;
    int flaggedFields;
    bool hasRevealedMine;
    Field **fields;

    /**
     * @brief Constructor that initializes the game board.
     * @param s The size of the board
     */
    Minesweeper(int s) {
      size = s;
      mineCount = 0;
      hasRevealedMine = false;
      remainFields = size * size;
      mineCount = (size * size) * 0.16;
      flaggedFields = 0;
      allocateFields();
    }

    /**
     * @brief Destructor that frees allocated memory.
     */
    ~Minesweeper() {
      freeFields();
    }

    /**
     * @brief Checks whether the given field coordinates are valid.
     * @param row Row index
     * @param column Column index
     * @return True if the field exists on the board
     */
    bool fieldExists(int row, int column) {
      return row >= 0 && column >= 0 && row < size && column < size;
    }

    /**
     * @brief Handles the logic when a field is selected.
     *
     * The user cannot select a previously revealed or flagged field.
     * Mines are generated only after the first move to prevent immediate loss.
     * If a mine is revealed, the game is marked as lost.
     */
    void selectField(int row, int column) {
      /* The user cannot select a previously revealed or flagged field.
         It must be unmarked first. */
      if (fields[row][column].state == REVEALED ||
          fields[row][column].state == FLAGGED)
        return;

      if (fields[row][column].hasMine) {
        hasRevealedMine = true;
        fields[row][column].state = REVEALED;
      }

      /* Mines are generated only after the first move
         to prevent the user from stepping on a mine immediately. */
      if (remainFields == size * size) {
        generateMines(row, column);
      }
    }

    /**
     * @brief Recursively reveals adjacent non-mine fields.
     *
     * If the field is invalid or already revealed, recursion stops.
     * If the field has neighboring mines, it is revealed and recursion ends.
     */
    void unrevealField(int row, int column) {
      /* If the field is invalid (e.g., negative index) or
         already revealed, recursion stops. */
      if (!fieldExists(row, column) || fields[row][column].state == REVEALED) {
        return;
      }

      if(fields[row][column].state == FLAGGED){
        flaggedFields--;
      }

      fields[row][column].state = REVEALED;
      remainFields--;

      /* If the field has neighboring mines, it is revealed
         and recursion ends. */
      if (fields[row][column].neighborMineCount != 0) {
        return;
      }

      // Reveal neighboring fields.
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          unrevealField(row + i, column + j);
        }
      }
    }

    /**
     * @brief Toggles a flag on the selected field.
     */
    void changeFlag(int row, int column) {
      if (fields[row][column].state == FLAGGED) {
        fields[row][column].state = UNSELECTED;
        flaggedFields--;
      }
      else if (fields[row][column].state == UNSELECTED) {
        fields[row][column].state = FLAGGED;
        flaggedFields++;
      }
    }

    /**
     * @brief Returns the estimated number of remaining mines based on flags.
     * @return Number of unflagged mines
     */
    int getRemainFieldsByFlags() {
      return mineCount - flaggedFields;
    }

  private:

    /**
     * @brief Allocates memory for the field matrix.
     */
    void allocateFields()
    {
      fields = NULL;
      fields = new Field*[size];
      checkSuccessfulAllocation(fields);
      for (int i = 0; i < size; i++) {
        fields[i] = NULL;
        fields[i] = new Field[size];
        checkSuccessfulAllocation(fields[i]);
      }
    }

    /**
     * @brief Frees the memory allocated for the field matrix.
     */
    void freeFields()
    {
      for (int i = 0; i < size; i++) {
        delete(fields[i]);
        fields[i] = NULL;
      }
      delete(fields);
      fields = NULL;
    }

    /**
     * @brief Checks whether memory allocation was successful.
     *
     * If not, prints an error message and halts the program.
     */
    void checkSuccessfulAllocation(void *pointer)
    {
      if (pointer == NULL) {
        Serial.println("ERROR: Memory allocation failure occurred during program execution!");
        delay(10000);
        while (true);
      }
    }

    /**
     * @brief Generates mines on the board.
     *
     * @param firstStepRow The row index of the first selected field
     * @param firstStepColumn The column index of the first selected field
     *
     * Ensures that no mine is placed on the first selected field.
     * Also updates the neighbor mine count for adjacent fields.
     */
    void generateMines(int firstStepRow, int firstStepColumn) {
      for (int i = 0; i < mineCount;) {
        int row = random(0, size);
        int column = random(0, size);
        if (!fields[row][column].hasMine && !(row == firstStepRow && column == firstStepColumn)) {
          fields[row][column].hasMine = true;
          i++;
          for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
              if (fieldExists(row + j, column + k) && !(j == 0 && k == 0)) {
                fields[row + j][column + k].neighborMineCount++;
              }
            }
          }
        }
      }
    }
};

#endif
