#ifndef MINESWEEPER_FIELD_H
#define MINESWEEPER_FIELD_H

#include "State.h"

/**
 * @class Field
 * @brief Stores data related to a single field in the game.
 *
 * This class represents a field that may or may not contain a mine,
 * and keeps track of its state and the number of neighboring mines.
 *
 */
class Field
{
  public:

    /**
     * @brief Indicates whether the field contains a mine.
     *
     * - true: the field contains a mine
     * - false: the field does not contain a mine
     */
    bool hasMine;
    /**
     * @brief Stores the number of neighboring fields that contain mines.
     */
    int neighborMineCount;
    /**
     * @brief Stores the state of the field.
     */
    enum State state;

    Field()
    {
      hasMine = false;
      neighborMineCount = 0;
      state = UNSELECTED;
    }
};

#endif
