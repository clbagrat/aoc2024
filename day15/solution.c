#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct point {
  int x;
  int y;
} point;

void printpoint(point a) { printf("%d - %d\n", a.x, a.y); }

void printmove(int move) {
  int signs[4] = {'<', '^', '>', 'v'};
  printf("%c\n", signs[move]);
}

void printgrid(char *grid, int maxx, int maxy) {
  char str[maxx*maxy+maxy+1 ];
  str[maxx*maxy+maxy] = '\0';
  int u = 0;
  for (int i = 0; i < maxx * maxy; i += 1) {
    if (i % maxx == 0) {
      str[u] = '\n';
      u+=1;
      //printf("\n");
    }
    str[u] = grid[i];
    u += 1;
  }
  printf("\r%s", str);
  fflush(stdout);
  usleep(50000);
}

int ptoi(point p, int max) { return p.y * max + p.x; }

point padd(point p, point add) {
  return (point){.x = p.x + add.x, .y = p.y + add.y};
}

void swap(char *list, int a, int b) {
  int temp = list[a];
  list[a] = list[b];
  list[b] = temp;
}

int calcgrid(char *grid, int maxx, int maxy) {
  int res = 0;
  for (int i = 0; i < maxx * maxy; i += 1) {
    if (grid[i] == '[') {
      int x = i % (maxx);
      int y = i / (maxx);
      res += x + y * 100;
    }
  }
  return res;
}

int canpush(char *grid, int maxx, point mypos, point d) {
  point nextpos = padd(mypos, d);
  int nextchar = grid[ptoi(nextpos, maxx)];
  if (nextchar == '.') {
    return 1;
  }
  if (nextchar == '#') {
    return 0;
  }

  // if vertically, need to take brothers
  // if horizontally, need to just call;
  if (d.y == 0) {
    if (nextchar == '[' || nextchar == ']') {
      return canpush(grid, maxx, nextpos, d);
    }
  } else {
    return canpush(grid, maxx, nextpos, d) &&
           canpush(grid, maxx,
                   (point){.x = nextpos.x + (nextchar == '[' ? 1 : -1),
                           .y = nextpos.y},
                   d);
  }

  return 0;
}

point dopush(char *grid, int maxx, point mypos, point d) {
  point nextpos = padd(mypos, d);
  int nextchar = grid[ptoi(nextpos, maxx)];
  if (nextchar == '.') {
    swap(grid, ptoi(mypos, maxx), ptoi(nextpos, maxx));
    return nextpos;
  }

  // if vertically, need to take brothers
  // if horizontally, need to just call;
  if (d.y == 0) {
    if (nextchar == '[' || nextchar == ']') {
      dopush(grid, maxx, nextpos, d);
      // return canpush(grid, maxx, nextpos, d);
    }
  } else {
    dopush(grid, maxx, nextpos, d);
    dopush(grid, maxx,
           (point){.x = nextpos.x + (nextchar == '[' ? 1 : -1), .y = nextpos.y},
           d);
  }

  swap(grid, ptoi(mypos, maxx), ptoi(nextpos, maxx));
  return nextpos;
}

point trypush(char *grid, int maxx, int maxy, point robotpos, point d) {
 // printf("~~~\n");
 // printpoint(robotpos);
 // printpoint(d);
  //printf("%d", canpush(grid, maxx, robotpos, d));
  //printf("~~~\n");
  if (canpush(grid, maxx, robotpos, d)) {
    return dopush(grid, maxx, robotpos, d);
  } else {
    return robotpos;
  }
}

// < 0
// ^ 1
// > 2
// v 4
void makemove(char *grid, int maxx, int maxy, point *robotpos, int move) {
  point directions[4] = {
      {.x = -1, .y = 0}, {.x = 0, .y = -1}, {.x = 1, .y = 0}, {.x = 0, .y = 1}};
  point d = directions[move];
  point shift = {.x = robotpos->x, .y = robotpos->y};

  // push obj by coord in direction
  // if pushed, take free space
  // if not -> chill
  point newrobotpos = trypush(grid, maxx, maxy, *robotpos, d);
  robotpos->x = newrobotpos.x;
  robotpos->y = newrobotpos.y;
  //printmove(move);
  printgrid(grid, maxx, maxy);
}

//. 0
// # 1
// O 2
//@ 3
void star2(char *grid, int maxx, int maxy, int *moves, int movecount,
           point robotpos) {
  int movesdone = 0;
  while (movesdone < movecount) {
    makemove(grid, maxx, maxy, &robotpos, moves[movesdone]);
    movesdone += 1;
  }

  printf("star1: %d \n", calcgrid(grid, maxx, maxy));
}

int main() {
  FILE *file = fopen("./day15/example", "r");

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
  int oldmax = 0;
  int maxx = 0;
  int maxy = 0;
  int cury = 0;

  char *grid = malloc(strlen(content) * 2 * sizeof(char));
  int gridcount = 0;

  int *moves = malloc(strlen(content) * sizeof(int));
  int movecount = 0;

  point robotpos = {};

  while (i < filesize) {
    char c = content[i];

    int x = oldmax ? i % (oldmax + 1) : i;
    int y = oldmax ? i / (oldmax + 1) : 0;

    if (!oldmax && c == '\n') {
      oldmax = i;
    }
    if (!maxx && c == '\n') {
      maxx = i * 2;
    }
    i += 1;

    if (c == '\n') {
      cury += 1;
      continue;
    }

    if (strstr(gridstencil, (char[2]){c, '\0'})) {
      if (c == '.' || c == '#') {
        grid[gridcount] = c;
        grid[gridcount + 1] = c;
      } else if (c == 'O') {
        grid[gridcount] = '[';
        grid[gridcount + 1] = ']';
      } else if (c == '@') {
        grid[gridcount] = '@';
        grid[gridcount + 1] = '.';
      }
      gridcount += 2;
      if (c == '@') {
        robotpos.x = x * 2;
        robotpos.y = y;
      }
    }

    if (strstr(movestencil, (char[2]){c, '\0'})) {
      if (!maxy) {
        maxy = cury - 1;
      }
      int index = strrchr(movestencil, c) - movestencil;
      moves[movecount] = index;
      movecount += 1;
    }
  }

  //printgrid(grid, maxx, maxy);
  //printpoint(robotpos);
  star2(grid, maxx, maxy, moves, movecount, robotpos);

  free(content);
  free(moves);
  free(grid);
}
