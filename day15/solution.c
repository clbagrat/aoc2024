#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
  int x;
  int y;
} point;

void printmove(int move) {
  int signs[4] = {'<', '^', '>', 'v'};
  printf("%c\n", signs[move]);
}

void printgrid(int *grid, int max) {
  //printf("%d\n", max);
  for (int i = 0; i < max * max; i += 1) {
    if (i % max == 0) {
      printf("\n");
    }
    //.#O@
    printf("%c", (char[4]){'.', '#', 'O', '@'}[grid[i]]);
  }
  printf("\n");
}

int ptoi(point p, int max) { return p.y * max + p.x; }

point padd(point p, point add) {
  return (point){.x = p.x + add.x, .y = p.y + add.y};
}

void swap(int *list, int a, int b) {
  int temp = list[a];
  list[a] = list[b];
  list[b] = temp;
}

int calcgrid (int *grid, int max) {
  int res = 0;
  for (int i = 0; i < max * max; i += 1) {
    if (grid[i] == 2) {
    int x = i % (max ) ;
    int y = i / (max );
      res += x + y * 100;
    }
  }
  return res;
}

// < 0
// ^ 1
// > 2
// v 4
void makemove(int *grid, int max, point *robotpos, int move) {
  printmove(move);
  point directions[4] = {
      {.x = -1, .y = 0}, {.x = 0, .y = -1}, {.x = 1, .y = 0}, {.x = 0, .y = 1}};
  // raycast in direction until 0 or 1 is met
  point d = directions[move];
  point shift = {.x = robotpos->x, .y = robotpos->y};
  while (1) {
    shift = padd(shift, d);
    int shiftedchar = grid[ptoi(shift, max)];
    if (shiftedchar == 1) { // cant move
      return;
    }
    if (shiftedchar == 0) {
      break;
    }
  }
  int freespace = ptoi(shift, max);
  point pnextbox = padd(*robotpos, d);
  int nextbox = ptoi(pnextbox, max);
  int robot = ptoi(*robotpos, max);
  // .0000@ -> .000@0
  swap(grid, robot, nextbox);
  printgrid(grid, max);
  printf("-------");
  if (nextbox != freespace) {
    // .000@0 -> 0000@.
    swap(grid, robot, freespace);
  }
  robotpos->x = pnextbox.x;
  robotpos->y = pnextbox.y;
  printgrid(grid, max);
}

//. 0
// # 1
// O 2
//@ 3
void star1(int *grid, int max, int *moves, int movecount, point robotpos) {
  int movesdone = 0;
  while (movesdone < movecount) {
    makemove(grid, max, &robotpos, moves[movesdone]);
    movesdone += 1;
  }

  printf("star1: %d \n", calcgrid(grid, max));
}

int main() {
  FILE *file = fopen("./day15/input", "r");

  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  rewind(file);

  char *content = malloc(filesize + 1);
  fread(content, 1, filesize, file);
  fclose(file);
  content[filesize] = '\0';

  char *gridstencil = ".#O@";
  char *movestencil = "<^>v";
  int i = 0;
  int max = 0;

  int *grid = malloc(strlen(content) * sizeof(int));
  int gridcount = 0;

  int *moves = malloc(strlen(content) * sizeof(int));
  int movecount = 0;

  point robotpos = {};

  while (i < filesize) {
    char c = content[i];

    int x = max ? i % (max + 1) : i;
    int y = max ? i / (max + 1) : 0;


    if (!max && c == '\n') {
      max = i;
    }
    i += 1;

    if (c == '\n') {
      continue;
    }

    if (strstr(gridstencil, (char[2]){c, '\0'})) {
      int index = strrchr(gridstencil, c) - gridstencil;
      grid[gridcount] = index;
      gridcount += 1;
      if (c == '@') {
        robotpos.x = x;
        robotpos.y = y;
      }
    }

    if (strstr(movestencil, (char[2]){c, '\0'})) {
      int index = strrchr(movestencil, c) - movestencil;
      moves[movecount] = index;
      movecount += 1;
    }
  }

  printgrid(grid, max);
  star1(grid, max, moves, movecount, robotpos);

  free(content);
  free(moves);
  free(grid);
}
