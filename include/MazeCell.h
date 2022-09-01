#ifndef INCLUDE_MAZECELL_H
#define INCLUDE_MAZECELL_H

class MazeCell {
 public:
  int visited;
  char data;
  int position;

  MazeCell(const int position);

  virtual ~MazeCell();

  inline void visit() { visited = 1; };
};

#endif
