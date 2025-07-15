#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "Field.h"
#include <Arduino.h>

/**
 * @class Minesweeper
 * @brief Stores the data required for the Minesweeper game.
 */
class Minesweeper
{
  public:

    int size;             /** The size of the board (size * size) */
    int mineCount;        /** The number of mines in the game, which depends on the generated board size */
    int remainFields;     /** The number of remaining unrevealed fields. If this equals mineCount, the game is won. */
    int flaggedFields;    /** The number of fields marked with a flag */
    bool hasRevealedMine; /** Indicates whether the player has revealed a mine (true: yes, false: no) */
    Field **fields;       /** A dynamically allocated matrix storing the fields */

    /**
     * @brief Constructor that initializes the game board.
     * @param s The size of the board
     */
    Minesweeper(int s);

    /**
     * @brief Destructor that frees allocated memory.
     */
    ~Minesweeper();

    /**
     * @brief Checks whether the given field coordinates are valid.
     * @param row Row index
     * @param column Column index
     * @return True if the field exists on the board
     */
    bool fieldExists(int row, int column);

    /**
     * @brief Handles the logic when a field is selected.
     *
     * The user cannot select a previously revealed or flagged field.
     * Mines are generated only after the first move to prevent immediate loss.
     * If a mine is revealed, the game is marked as lost.
     * @param row Row index of the selected field
     * @param column Column index of the selected field
     */
    void selectField(int row, int column);

    /**
     * @brief Recursively reveals adjacent non-mine fields.
     *
     * If the field is invalid or already revealed, recursion stops.
     * If the field has neighboring mines, it is revealed and recursion ends.
     * @param row Row index of the field
     * @param column Column index of the field
     */
    void unrevealField(int row, int column);

    /**
     * @brief Toggles a flag on the selected field.
     * @param row Row index of the field
     * @param column Column index of the field
     */
    void changeFlag(int row, int column);

    /**
     * @brief Returns the estimated number of remaining mines based on flags.
     * @return Number of unflagged mines
     */
    int getRemainFieldsByFlags();

  private:

    /**
     * @brief Allocates memory for the field matrix.
     */
    void allocateFields();

    /**
     * @brief Frees the memory allocated for the field matrix.
     */
    void freeFields();

    /**
     * @brief Checks whether memory allocation was successful.
     *
     * If not, prints an error message and halts the program.
     * @param pointer Pointer to check
     */
    void checkSuccessfulAllocation(void *pointer);

    /**
     * @brief Generates mines on the board.
     *
     * @param firstStepRow The row index of the first selected field
     * @param firstStepColumn The column index of the first selected field
     *
     * Ensures that no mine is placed on the first selected field.
     * Also updates the neighbor mine count for adjacent fields.
     */
    void generateMines(int firstStepRow, int firstStepColumn);
};

#endif
