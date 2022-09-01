#include "Maze.h"
#include "MazeCell.h"
#include "defs.h"
#include "Player.h"

#include <vector>
#include <stdlib.h>
#include <string>

Maze::Maze(const int width, const int height)
  :width(width), height(height)
{
  int randPos;
  for (int i = 0; i < width * height; i++) {
    maze.push_back(MazeCell(i));
  }

  randPos = rand() % maze.size();
  while (randPos % 2 == 0) {
    randPos = rand() % maze.size();
  }
  generateMaze(randPos);
  generatePlayerLocation();
  generateMapLocations();
  generateGemLocations();
}

Maze::~Maze(){}

void Maze::printGameState() {
  std::string gameState = "Game State";
  std::string gemsLeft = "Gems Left:";
  std::string breaksLeft = "Breaks Remaining:";
  std::string bombsLeft = "Bombs Remaining:";
  std::string exitMSG = "Click Any Button To Exit";

  clear();
  mvprintw(0, (WW / 2) - (gameState.length() / 2),
	   "%s", gameState.c_str());

  mvprintw(2, (WW / 2) - (gemsLeft.length() / 2),
	   "%s %d", gemsLeft.c_str(), gems.size());

  mvprintw(3, (WW / 2) - (breaksLeft.length() / 2),
	   "%s %d", breaksLeft.c_str(), player.breaks);

  mvprintw(4, (WW / 2) - (bombsLeft.length() / 2),
	   "%s %d", bombsLeft.c_str(), player.bombs);

  mvprintw(6, (WW / 2) - (exitMSG.length() / 2),
	   "%s", exitMSG.c_str());
  
  getch();
}

void Maze::print() {
  resetVisitedCells();
  generateVisibleMaze(player.pos, player.vision);
  clear();

  for (int i = 0; i < visibleMaze.size(); i++) {
    attron(COLOR_PAIR(1));
    mvprintw((int) visibleMaze[i].position / width,
	     visibleMaze[i].position % width, "%c", visibleMaze[i].data);
    attroff(COLOR_PAIR(1));
    
    for (int j = 0; j < gems.size(); j++) {
      if (visibleMaze[i].position == gems[j]) {
	attron(COLOR_PAIR(4));
	mvprintw((int) gems[j] / width, gems[j] % width,
		 "%c", '*');
	attroff(COLOR_PAIR(4));
      }
    }
  }
  for (int z = 0; z < maps.size(); z++) {
    attron(COLOR_PAIR(2));
    mvprintw((int) maps[z] / width, maps[z] % width,
	     "%c", '#');
    attroff(COLOR_PAIR(2));
  }
  
  attron(COLOR_PAIR(3));
  mvprintw((int) player.pos / width, player.pos % width, "%c", '@');
  attroff(COLOR_PAIR(3));
  
  visibleMaze.clear();
}

void Maze::generateMaze(const int pos) {
  int rdm;
  maze[pos].visit();
  maze[pos].data = ' ';

  while(checkUp(pos) || checkDown(pos) || checkLeft(pos) || checkRight(pos)) {
    rdm = rand() % 4;
    if (checkUp(pos) && rdm == 0) {
      maze[pos - width].data = ' ';
      generateMaze(pos - width * 2);
      continue;
    }

    if (checkDown(pos) && rdm == 1) {
      maze[pos + width].data = ' ';
      generateMaze(pos + width * 2);
      continue;
    }

    if (checkLeft(pos) && rdm == 2) {
      maze[pos - 1].data = ' ';
      generateMaze(pos - 2);
      continue;
    }

    if (checkRight(pos) && rdm == 3) {
      maze[pos + 1].data = ' ';
      generateMaze(pos + 2);
      continue;
    }
  }
}

int Maze::checkDown(const int pos) {
  if (pos + width * 2 < maze.size() && !maze[pos + width * 2].visited) {
     return 1;
   }
   return 0;
}

int Maze::checkUp(const int pos) {
  if (pos - width * 2 >= 0 && !maze[pos - width * 2].visited) {
    return 1;
  }
  return 0;
}

int Maze::checkRight(const int pos) {
  if (pos + 2 < maze.size() && !maze[pos + 2].visited
      && pos / width == (pos + 2) / width) {
    return 1;
  }
  return 0;
}

int Maze::checkLeft(const int pos) {
  if (pos - 2 >= 0 && !maze[pos - 2].visited && pos / width == (pos - 2) / width) {
    return 1;
  }
  return 0;
}

void Maze::generatePlayerLocation() {
  player.pos = rand() % maze.size();

  while (maze[player.pos].data != ' ') {
    player.pos = rand() % maze.size();
  }
}

void Maze::moveUp() {
  if (player.pos - width < 0 ||
      maze[player.pos - width].data == '%') {
    return;
  }
  player.pos = player.pos - width;

  takeMap();
  takeGem();
}

void Maze::moveDown() {
  if (player.pos + width >= maze.size() ||
      maze[player.pos + width].data == '%') {
    return;
  }
  player.pos = player.pos + width;

  takeMap();
  takeGem();
}

void Maze::moveLeft() {
  if (maze[player.pos - 1].data == '%' || player.pos % width == 0) {
    return;
  }
  player.pos = player.pos - 1;

  takeMap();
  takeGem();
}

void Maze::moveRight() {
  if (maze[player.pos + 1].data == '%' || player.pos / width != (player.pos + 1) / width) {
    return;
  }
  player.pos = player.pos + 1;

  takeMap();
  takeGem();
}

void Maze::resetVisitedCells() {
  for (int i = 0; i < maze.size(); i++) {
    maze[i].visited = 0;
  }
}

void Maze::generateVisibleMaze(const int position, const int count) {
  if (count == 0) {
    return;
  }

  if (checkInVisibleMaze(position)) { 
    visibleMaze.push_back(maze[position]);
  }
  maze[position].visited = 1;

  if (position - width >= 0 && maze[position - width].data == '%' &&
      !maze[position - width].visited) {
    visibleMaze.push_back(maze[position - width]);
    maze[position - width].visited = 1;
  }

  if (position + width < maze.size() &&
      maze[position + width].data == '%' &&
      !maze[position + width].visited) {
    visibleMaze.push_back(maze[position + width]);
    maze[position + width].visited = 1;
  }

  if (position / width == (position - 1) / width &&
      maze[position - 1].data == '%' &&
      !maze[position - 1].visited) {
    visibleMaze.push_back(maze[position - 1]);
    maze[position - 1].visited = 1;
  }

  if (position / width == (position + 1) / width &&
      maze[position + 1].data == '%' &&
      !maze[position + 1].visited) {
    visibleMaze.push_back(maze[position + 1]);
    maze[position + 1].visited = 1;
  }

  if (checkUpBF(position)) {
    generateVisibleMaze(position - width, count - 1);
  }

  if (checkDownBF(position)) {
      generateVisibleMaze(position + width, count - 1);
  }

  if (checkLeftBF(position)) {
      generateVisibleMaze(position - 1, count - 1);
  }

  if (checkRightBF(position)) {
      generateVisibleMaze(position + 1, count - 1);
  }
}

int Maze::checkUpBF(const int pos) {
   if (pos - width >= 0 && maze[pos - width].data == ' ') {
    return 1;
  }
  return 0;
}

int Maze::checkDownBF(const int pos) {
   if (pos + width < maze.size() && maze[pos + width].data == ' ') {
     return 1;
   }
   return 0;
}

int Maze::checkLeftBF(const int pos) {
   if (pos - 1 >= 0 && pos / width == (pos - 1) / width &&
       maze[pos - 1].data == ' ') {
    return 1;
  }
  return 0;
}

int Maze::checkRightBF(const int pos) {
   if (pos + 1 < maze.size() && pos / width == (pos + 1) / width &&
       maze[pos + 1].data == ' ') {
    return 1;
  }
  return 0;
}

void Maze::generateMapLocations() {
  int count = 0;
  int rdm;

  for (int i = 0; i < maze.size(); i++) {
    if (maze[i].data == ' ') {
      count++; 
    }
  }

  count /= (width * 2);

  for (int i = 0; i < count; i++) {
    rdm = rand() % maze.size();

    while (maze[rdm].data != ' ') {
      rdm = rand() % maze.size();
    }
    maps.push_back(rdm);
  }
}

void Maze::takeMap() {
  for (int i = 0; i < maps.size(); i++) {
    if (maps[i] == player.pos) {
      player.vision += 1;
      maps.erase(maps.begin() + i);
    }
  }
}

void Maze::takeGem() {
  for (int i = 0; i < gems.size(); i ++) {
    if (gems[i] == player.pos) {
      gems.erase(gems.begin() + i);
      player.updateItems();
    }
  }
}

void Maze::toggleBreakMode() {
  int input;

  if (player.breaks == 0) {
    return;
  }
  
  attron(COLOR_PAIR(3));
  mvprintw((int) player.pos / width, player.pos % width, "%c", 'B');
  attroff(COLOR_PAIR(3));

  input = getch();

  if (input == 'w' && player.pos - width >= 0 &&
      maze[player.pos - width].data == '%') {
    maze[player.pos - width].data = ' ';
    player.breaks--;
  }

  if (input == 's' && player.pos + width < maze.size() &&
      maze[player.pos + width].data == '%') {
    maze[player.pos + width].data = ' ';
    player.breaks--;
  }

  if (input == 'a' && player.pos - 1 >= 0 &&
      player.pos / width == (player.pos - 1) / width &&
      maze[player.pos - 1].data == '%') {
    maze[player.pos - 1].data = ' ';
    player.breaks--;
  }

  if (input == 'd' && player.pos + 1 < maze.size() &&
      player.pos / width == (player.pos + 1) / width &&
      maze[player.pos + 1].data == '%') {
    maze[player.pos + 1].data = ' ';
    player.breaks--;
  }
  
  attron(COLOR_PAIR(3));
  mvprintw((int) player.pos / width, player.pos % width, "%c", '@');
  attroff(COLOR_PAIR(3));
}

void Maze::generateGemLocations() {
  int count = 0;
  int rdm;

  for (int i = 0; i < maze.size(); i++) {
    if (maze[i].data == ' ') {
      count++; 
    }
  }

  count /= 20;

  for (int i = 0; i < count; i++) {
    rdm = rand() % maze.size();

    while (maze[rdm].data != ' ' || checkMap(rdm)) {
      rdm = rand() % maze.size();
    }
    gems.push_back(rdm);
  }
}

bool Maze::checkMap(const int position) {
  for (int i = 0; i < maps.size(); i++) {
    if (maps[i] == position) {
      return true;
    }
  }
  return false;
}

bool Maze::winCondition() {
  std::string winMSG = "You Collected All Gems!!!";
  std::string exitMSG = "Click Any Button To End Game";
  if (gems.size() == 0) {
    clear();
    mvprintw(0, (WW / 2) - (winMSG.length() / 2),
	     "%s", winMSG.c_str());
    mvprintw(WH / 2, (WW / 2) - (exitMSG.length() / 2),
	     "%s", exitMSG.c_str());
    getch();

    return true;
  }
 return false;
}

void Maze::explode() {
  if (player.bombs == 0) {
    return;
  }
  
  if (player.pos + width < maze.size() &&
      maze[player.pos + width].data == '%') {
    maze[player.pos + width].data = ' ';
  }

  if (player.pos + width + 1 < maze.size() &&
      maze[player.pos + width + 1].data == '%') {
    maze[player.pos + width + 1].data = ' ';
  }

  if (player.pos + width - 1 < maze.size() &&
      maze[player.pos + width - 1].data == '%') {
    maze[player.pos + width - 1].data = ' ';
  }

  if (player.pos - width >= 0 &&
      maze[player.pos - width].data == '%') {
    maze[player.pos - width].data = ' ';
  }

  if (player.pos - width - 1 >= 0 &&
      maze[player.pos - width - 1].data == '%') {
    maze[player.pos - width - 1].data = ' ';
  }

  if (player.pos - width + 1 >= 0 &&
      maze[player.pos - width + 1].data == '%') {
    maze[player.pos - width + 1].data = ' ';
  }

  if (player.pos - 1 >= 0 &&
      player.pos / width == (player.pos - 1) / width &&
      maze[player.pos - 1].data == '%') {
    maze[player.pos - 1].data = ' ';
  }

  if (player.pos + 1 < maze.size() &&
      player.pos / width == (player.pos + 1) / width &&
      maze[player.pos + 1].data == '%') {
    maze[player.pos + 1].data = ' ';
  }
  player.bombs--;
}

bool Maze::checkInVisibleMaze(const int position) {
  for (int i = 0; i < visibleMaze.size(); i++) {
    if (visibleMaze[i].position == position) {
      return false;
    }
  }
  return true;
}
