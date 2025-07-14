#ifndef _DISPLAY_
#define _DISPLAY_

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "Minesweeper.cpp"
#include "Color.cpp"
#include "Vector2D.cpp"

#define START_Y 80
#define START_X 5
#define MAX_HEIGHT 320
#define MAX_WIDTH 240
#define FIELD_SIZE 20
#define FIELD_GAP 10

class FieldDisplay {

  public:

    Minesweeper *board;
    Adafruit_ILI9341 *display;
    Adafruit_FT6206 *ctp;

    FieldDisplay(Minesweeper *fields, Adafruit_ILI9341 *screen, Adafruit_FT6206 *touch) {
      board = fields;
      display = screen;
      ctp = touch;
    }

    // showTable : megjeleníti újból a táblázatot a jelenlegi állapottal.
    void showTable() {
      display->fillScreen(DARKGRAY);
      showRemainMines();
      for (int i = 0; i < board->size; i++) {
        for (int n = 0; n < board->size; n++) {
          displayField(i, n);
        }
      }
    }

    void samplePoint() {
      // Várunk egy érintésre.
      while (!ctp->touched());
      // Rövid érintés esetén az adott helyet felfedjük, hosszú érintés esetén zászlót helyezünk rá.
      TS_Point point; int i;
      for (i = 0; i < 15 && ctp->touched(); i++) {
        point = getPoint();
        delay(50);
      }

      if (i < 10) {
        markField(point);
      } else {
        flagField(point);
      }
    }


    void markField(TS_Point point) {
      Vector2D fieldCoords = *getTouchedFieldArrayPosition(point);

      if (fieldCoords.x == -1 || fieldCoords.y == -1) {
        return;
      }

      Field *field = &board->fields[fieldCoords.y][fieldCoords.x];

      // Ha már fel van fedve a mező, akkor nem módosítunk semmit sem.
      if (field->state == REVEALED || field->state == FLAGGED) {
        return;
      }

      board->selectField(fieldCoords.y, fieldCoords.x);
      board->unrevealField(fieldCoords.y, fieldCoords.x);

      if (field->neighborMineCount == 0 || board->hasRevealedMine) {
        showTable();
      } else {
        displayField(fieldCoords.y, fieldCoords.x);
      }

    }

    void flagField(TS_Point point) {
      Vector2D coords = *getTouchedFieldArrayPosition(point);

      if (coords.x == -1 || coords.y == -1) {
        return;
      }

      board->changeFlag(coords.y, coords.x);
      displayField(coords.y, coords.x);
      showRemainMines();

      return;
    }

  private:

    // calculateY: kiszámolja egy adott mező kezdőpontjának Y koordinátáját.
    int calculateY(int row) {
      return START_Y + row * (FIELD_SIZE + FIELD_GAP);
    }

    // calculateX: kiszámolja egy adott mező kezdőpontjának X koordinátáját.
    int calculateX(int column) {
      return START_X + column * (FIELD_SIZE + FIELD_GAP);
    }

    Vector2D* getTouchedFieldArrayPosition(TS_Point point) {
      for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
          if (fieldHasTouched(point, i, j)) {
            return new Vector2D(j, i);
          }
        }
      }

      return new Vector2D(-1, -1);
    }

    // fieldHasTouched: meghatározza, hogy az adott mező meg lett-e érintve
    bool fieldHasTouched(TS_Point point, int row, int column) {
      int fieldStartX = calculateX(column);
      int fieldStartY = calculateY(row);
      bool xCheck = point.x >= fieldStartX && point.x <= fieldStartX + FIELD_SIZE;
      bool yCheck = point.y >= fieldStartY && point.y <= fieldStartY + FIELD_SIZE;

      return xCheck && yCheck;
    }

    void displayFlag(int xCoord, int yCoord) {
      // Fejlesztési lehetőség: több táblaméret esetén méretarányosan (%) megoldani.
      // Zászló rúd
      display->fillRect(xCoord + 14, yCoord + 12, 1, 5, BLACK);
      // Zászló talpa
      display->fillRect(xCoord + 9, yCoord + 17, 7, 1, BLACK);
      // Zászló
      display->fillTriangle(xCoord + 2, yCoord + 7, xCoord + 14, yCoord + 2, xCoord + 14, yCoord + 11, RED);
    }

    void displayMine(int xCoord, int yCoord) {
      display->fillCircle(xCoord + FIELD_SIZE / 2, yCoord + FIELD_SIZE / 2, FIELD_SIZE * 0.3, BLACK);
    }

    void displayField(int row, int column) {
      switch (board->fields[row][column].state) {
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

    // displayUnselectedField: Egy fel nem fedett mező grafikus megjelenítése
    void displayUnselectedField(int row, int column) {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->hasRevealedMine && board->fields[row][column].hasMine) {
        displayMine(xCoord, yCoord);
      } else {
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, GRAY);
      }
    }

    // displayRevealedField: Egy felfedett mező grafikus megjelenítése
    void displayRevealedField(int row, int column) {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->fields[row][column].hasMine) {
        // A felhasználó által felfedett aknát piros alapon jelenítjük meg.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, RED);
        displayMine(xCoord, yCoord);
      } else {
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, DARKGRAY);
        display->setCursor(xCoord + 2, yCoord);
        display->setTextColor(getColor(board->fields[row][column].neighborMineCount));
        display->setTextSize(3);
        display->println(board->fields[row][column].neighborMineCount);
      }
    }

    // displayFlaggedField: A felhasználó által zászlóval jelölt mező megjelenítése
    void displayFlaggedField(int row, int column) {
      int xCoord = calculateX(column);
      int yCoord = calculateY(row);
      if (board->hasRevealedMine && !board->fields[row][column].hasMine) {
        // A játék végén piros alapon jelenik meg a zászló, ha nincs rajta akna.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, DARKRED);
      } else {
        // Ha helyesen van megjelölve zászlóval a hely, akkor szürke alapon jelenik meg.
        display->fillRect(xCoord, yCoord, FIELD_SIZE, FIELD_SIZE, GRAY);
      }
      displayFlag(xCoord, yCoord);
    }

    // getColor: megadja, hogy az adott mezőn milyen színnel legyen kiírva a szomszédos aknák száma.
    color getColor(int neighborMinesCount) {
      switch (neighborMinesCount) {
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

    TS_Point getPoint() {
      while (!ctp->touched());
      TS_Point point = ctp->getPoint();
      point.x = map(point.x, 0, 240, 240, 0);
      point.y = map(point.y, 0, 320, 320, 0);
      return point;
    }

    void showRemainMines() {
      // A zászlóval jelölt helyek száma alapján kiírja a maradék aknásított helyek számát.
      display->setCursor(START_X, 20);
      display->setTextColor(GRAY);
      display->setTextSize(2);
      display->println("Maradek aknak:");
      display->fillRect(START_X, 40, MAX_WIDTH - 50, 30, DARKGRAY);
      display->setCursor(START_X, 40);
      display->setTextColor(GRAY);
      display->print(board->getRemainFieldsByFlags());
      display->print(" db");
    }

};

#endif