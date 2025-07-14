#ifndef FIELD
#define FIELD

#include "State.cpp"

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

#endif