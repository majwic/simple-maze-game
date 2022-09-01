#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

class Player {
 public:
  int pos;
  int vision;
  int breaks;
  int gems;
  int bombs;

  Player();

  virtual ~Player();

  void updateItems();
};

#endif
