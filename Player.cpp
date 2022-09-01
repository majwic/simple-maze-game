#include "Player.h"

Player::Player()
  :pos(0), vision(3), breaks(0), gems(0)
{}

Player::~Player(){}

void Player::updateItems() {
  gems++;
  if (gems % 5 == 0) {
    breaks++;
  }

  if (gems % 10 == 0) {
    bombs++;
  }
}
