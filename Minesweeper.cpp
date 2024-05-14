#ifndef _MINESWEEPER_
#define _MINESWEEPER_

#include <Arduino.h>

/*
   State

   Feladata:
      Egy mező állapotának reprezentálása

   Állapotok:
      - UNSELECTED: a mező tartalma nem ismert a felhasználónak, nem volt még kiválasztva/felfedve
      - FLAGGED: zászlóval megjelölve, potenciálisan akna van ott
      - REVEALED: a mezőt kiválasztotta a felhasználó / az algoritmus felfedte, mert üres

*/

enum State
{
  UNSELECTED,
  FLAGGED,
  REVEALED
};

/*
  Field osztály: Tárolja egy mező adatait

  Mezők:
    HasMine: Tárolja, hogy az adott mező akna-e
      true : igen, false: nem
    neighborMineCount: Tárolja a szomszédos aknásított mezők számát
    state: a mező állapota
*/

class Field
{
  public:
    bool hasMine;
    int neighborMineCount;
    enum State state;

    Field() {
      hasMine = false;
      neighborMineCount = 0;
      state = UNSELECTED;
    }
};


/*
  Minesweeper osztály: Tárolja a játékhoz szükséges adatokat

  Mezők:
    size:
      A táblázat mérete (size*size)
    mineCount
      A játékban lévő aknák száma, ami a generált táblázat méretétől függ.
    remainFields
      A hátralévő, nem kiválasztott mezők száma
      Ha egyenlő a mineCount-al, akkor a játék megnyerésre került.
    flaggedFields:
      A zászlóval jelölt helyek száma
    hasRevealedMine
      Tárolja, hogy a játékos aknát fedett-e fel (true: igen, false: nem)
    fields:
      Mezőket tároló mátrix, dinamikus allokálású
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

    Minesweeper(int s) {
      size = s;
      mineCount = 0;
      hasRevealedMine = false;
      remainFields = size * size;
      mineCount = (size * size) * 0.16;
      flaggedFields = 0;
      allocateFields();
    }

    ~Minesweeper() {
      freeFields();
    }

    // fieldExists: ellenőrzi, hogy az adott mező hely létezik-e
    bool fieldExists(int row, int column) {
      return row >= 0 && column >= 0 && row < size && column < size;
    }

    void selectField(int row, int column) {
      /* Korábban felfedett mezőt, illetve zászlóval jelölt helyet
        nem választhat a felhasználó, előtte el kell azt távolítania. */
      if (fields[row][column].state == REVEALED ||
          fields[row][column].state == FLAGGED)
        return;


      if (fields[row][column].hasMine) {
        hasRevealedMine = true;
        fields[row][column].state = REVEALED;
      }

      /* Aknák csak az első lépés után generálódnak, hogy
        a felhasználó ne lépjen egyszerre aknára.*/
      if (remainFields == size * size) {
        generateMines(row, column);
      }
    }

    // unrevealField: rekurzív algoritmus, ami felfedi a környező aknamentes mezőket
    void unrevealField(int row, int column) {
      /* Amennyiben az adott mező nem érvényes (pl. negatív szám), vagy
        korábban már felfedésre került az adott hely, akkor a rekurzió befejeződik. */
      if (!fieldExists(row, column) || fields[row][column].state == REVEALED) {
        return;
      }

      if(fields[row][column].state == FLAGGED){
        flaggedFields--;
      }

      fields[row][column].state = REVEALED;
      remainFields--;

      /* Ha egy hely körül van szomszédos akna, akkor azt a helyet felfedjük,
        és utána fejeződik be a rekurzió. */
      if (fields[row][column].neighborMineCount != 0) {
        return;
      }

      // Felfedjük a szomszédos mezőket.
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          unrevealField(row + i, column + j);
        }
      }
    }

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

    int getRemainFieldsByFlags() {
      return mineCount - flaggedFields;
    }

  private:

    // allocateFields: mátrix dinamikus allokációjáért felelős metódus
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

    // freeFields: memória felszabadításért felelős metódus
    void freeFields()
    {
      for (int i = 0; i < size; i++) {
        delete[](fields[i]);
        fields[i] = NULL;
      }
      delete[](fields);
      fields = NULL;
    }

    void checkSuccessfulAllocation(void *pointer)
    {
      if (pointer == NULL) {
        Serial.println("HIBA: memoria allokacio hiba tortent a program futasa soran!");
        delay(10000);
        while (true);
      }
    }

    /* Aknák generálásáért felelős algoritmus
      firstStepRow: első kiválasztott akna sorszáma
      firstStepColumn: első kiválasztott akna oszlopszáma.
      Ezek szükségesek, hogy oda ne kerüljön akna.
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