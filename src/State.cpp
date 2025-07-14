/**
 * @enum State
 * @brief Represents the state of a field in the game.
 *
 * This enumeration defines the possible states a field can be in during gameplay.
 */
enum State
{
  /**
   * @brief The field has not been selected or revealed by the user. Its content is unknown.
   */
  UNSELECTED,
  /**
   * @brief The field is marked with a flag, indicating a potential mine.
   */
  FLAGGED,
  /**
   * @brief The field has been revealed either by the user or automatically by the algorithm (e.g., when it's empty).
   */
  REVEALED
};
