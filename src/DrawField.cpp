#include "DrawField.h"

DrawField::DrawField(Adafruit_ILI9341 *display)
{
  this->display = display;
}

void DrawField::drawRevealedFieldWithoutMine(Vector2D fieldPosition, int neighborMines)
{
  display->fillRect(fieldPosition.x, fieldPosition.y, FIELD_SIZE, FIELD_SIZE, DARKGRAY);
  display->setCursor(fieldPosition.x + 2, fieldPosition.y);
  display->setTextColor(getColor(neighborMines));
  display->setTextSize(3);
  display->println(neighborMines);
}

void DrawField::drawFlag(Vector2D fieldPosition, color backgroundColor)
{
  // Background
  display->fillRect(fieldPosition.x, fieldPosition.y, FIELD_SIZE, FIELD_SIZE, backgroundColor);
  // Flagpole
  display->fillRect(fieldPosition.x + 14, fieldPosition.y + 12, 1, 5, BLACK);
  // Flag base
  display->fillRect(fieldPosition.x + 9, fieldPosition.y + 17, 7, 1, BLACK);
  // Flag banner
  display->fillTriangle(fieldPosition.x + 2, fieldPosition.y + 7, fieldPosition.x + 14, fieldPosition.y + 2,
                        fieldPosition.x + 14, fieldPosition.y + 11, RED);
}

void DrawField::drawMine(Vector2D fieldPosition)
{
  display->fillCircle(fieldPosition.x + FIELD_SIZE / 2, fieldPosition.y + FIELD_SIZE / 2, FIELD_SIZE * 0.3, BLACK);
}

void DrawField::drawRevealedMine(Vector2D fieldPosition)
{
  display->fillRect(fieldPosition.x, fieldPosition.y, FIELD_SIZE, FIELD_SIZE, RED);
  drawMine(fieldPosition);
}

/**
 * @brief Gets the appropriate color for the number of neighboring mines.
 * @param neighborMines The number of mines adjacent to the field.
 * @return The color value for the text.
 */
color DrawField::getColor(int neighborMines)
{
  switch (neighborMines)
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