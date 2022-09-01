#ifndef INCLUDE_MAZE_H
#define INCLUDE_MAZE_H

#include <vector>
#include "MazeCell.h"
#include "Player.h"

class Maze {
 public:
  int width;
  int height;
  std::vector<int> maps;
  std::vector<int> gems;
  std::vector<MazeCell> maze;
  std::vector<MazeCell> visibleMaze;
  Player player = Player();

  Maze(const int width, const int height);

  virtual ~Maze();

  void generateMaze(const int position);
  void generatePlayerLocation();
  void resetVisitedCells();
  void generateVisibleMaze(const int position, const int count);
  void generateMapLocations();
  void generateGemLocations();

  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();

  void toggleBreakMode();
  void explode();
  void takeMap();
  void takeGem();

  bool checkMap(const int position);
  bool checkInVisibleMaze(const int position);

  int checkUp(const int position);
  int checkDown(const int position);
  int checkLeft(const int position);
  int checkRight(const int position);

  int checkUpBF(const int position);
  int checkDownBF(const int position);
  int checkLeftBF(const int position);
  int checkRightBF(const int position);

  bool winCondition();

  void printGameState();
  void print();
};

#endif
