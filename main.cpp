#include "defs.h"
#include "Maze.h"
#include "MazeCell.h"

#include <iostream>
#include <stdlib.h>

void gameLoop() {
  int input;
  Maze laby = Maze(WW, WH);
  laby.print();

  while(true) {
    input = getch();

    if (input == 'w') {
      laby.moveUp();
    }

    if (input == 's') {
      laby.moveDown();
    }

    if (input == 'a') {
      laby.moveLeft();
    }

    if (input == 'd') {
      laby.moveRight();
    }

    if (input == 'q') {
      laby.toggleBreakMode();
    }

    if (input == 't') {
      laby.printGameState();
    }

    if (input == 'z') {
      laby.explode();
    }

    clear();
    if (laby.winCondition()) break;
    laby.print();
  }
}

int main() {
  initscr();
  raw();
  noecho();
  start_color();
  cbreak();
  curs_set(0);
  srand(time(NULL));

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  
  gameLoop();
  
  endwin();
  
  return 0;
}


