#include <stdio.h>
#include <stdlib.h>

typedef struct point {
  int x;
  int y;
  int count; // distance from start
  char ch;
} point;

typedef struct grid {
  point *data;
  int max;
} grid;

void printgrid(grid grid) {
  for (int i = 0; i < grid.max * grid.max; i += 1) {
    if (i % grid.max == 0) {
      printf("\n");
    }
    printf("%c", grid.data[i].ch);
  }
  printf("\n");
}

int isequal(struct point p1, struct point p2) {
  return (p1.x == p2.x) && (p1.y == p2.y);
}

int ptoi(point p, int max) { return p.y * max + p.x; }

point padd(point p, point add) {
  return (point){.x = p.x + add.x, .y = p.y + add.y};
}

int ispointingrid(grid grid, point p) {
  return !(p.x >= grid.max || p.x < 0 || p.y >= grid.max || p.y < 0);
}

char getgridchar(grid grid, point p) {
  if (!ispointingrid(grid, p)) {
    return '#';
  }
  return grid.data[ptoi(p, grid.max)].ch;
}

point getgriditem(grid grid, point p) {
  if (!ispointingrid(grid, p)) {
    return (point){.x = -1, .y = -1};
  }
  return grid.data[ptoi(p, grid.max)];
}

point *setgridpointcount(grid grid, point p, int c) {
  if (!ispointingrid(grid, p)) {
    return NULL;
  }
  grid.data[ptoi(p, grid.max)].count = c;
  return &grid.data[ptoi(p, grid.max)];
}

void printall(grid grid, point *path, int count) {
  for (int i = 0; i < count; i += 1) {
    point c = getgriditem(grid, path[i]);
    printf("%d, %d  count:%d ch: %c\n", c.x, c.y, c.count, c.ch);
  }
}

void setgridchar(grid grid, point p, char c) {
  if (!ispointingrid(grid, p)) {
    return;
  }
  grid.data[ptoi(p, grid.max)].ch = c;
}

char getdirchar(point dir) {
  if (dir.x == -1) {
    return '<';
  }
  if (dir.x == 1) {
    return '>';
  }
  if (dir.y == 1) {
    return 'V';
  }
  return '^';
}

// thanks chat gpt
point *getmanhattanedgepoints(point p, int distance, int *resultcount) {
  int count = 4 * distance;
  int x = p.x;
  int y = p.y;

  point *points = (point *)malloc(count * sizeof(point));
  int index = 0;

  for (int dx = -distance; dx <= distance; dx++) {
    int dy = distance - abs(dx);
    points[index++] = (point){.x = x + dx, .y = y + dy};
    if (dy != 0) {
      points[index++] = (point){.x = x + dx, .y = y - dy};
    }
  }

  *resultcount = index;
  return points;
}

int main() {
  FILE *file = fopen("./day20/input", "r");
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  rewind(file);
  char *content = malloc(filesize + 1);
  fread(content, 1, filesize, file);
  fclose(file);
  content[filesize] = '\0';

  int i = 0;
  point *griddata = malloc((filesize + 1) * sizeof(point));
  int gridcount = 0;
  grid grid = {.data = griddata, .max = 0};

  point start;
  point end;

  while (i < filesize) {
    char c = content[i];

    int x = grid.max ? i % (grid.max + 1) : i;
    int y = grid.max ? i / (grid.max + 1) : 0;
    if (!grid.max && c == '\n') {
      grid.max = i;
    }

    if (c != '\n') {
      point newpoint = (point){.x = x, .y = y, .ch = c};

      if (c == 'S') {
        start = newpoint;
      }

      if (c == 'E') {
        end = newpoint;
      }

      grid.data[gridcount] = newpoint;
      gridcount += 1;
    }

    i += 1;
  }

  // this path contains coords ONLY
  point *path = malloc(grid.max * grid.max * sizeof(point));
  int pathcount = 0;
  point prev = {.x = -1, .y = -1};
  point cur = start;

  point ds[4] = {
      {.x = -1, .y = 0}, {.x = 0, .y = -1}, {.x = 1, .y = 0}, {.x = 0, .y = 1}};

  path[0] = start;
  pathcount = 1;
  // go over the path once and record all the steps
  while (1) {
    if (isequal(cur, end)) {
      break;
    };

    for (int i = 0; i < 4; i += 1) {
      point next = padd(cur, ds[i]);
      if (isequal(next, prev))
        continue;

      char c = getgridchar(grid, next) != '#';
      if (getgridchar(grid, next) != '#') {
        setgridpointcount(grid, next, pathcount);
        path[pathcount] = next;
        pathcount += 1;

        prev = cur;
        cur = next;
        // setgridchar(grid, next, getdirchar(ds[i]));
      }
    }
  }

  int res1 = 0;
  // go over the path again and find cheat spots
  for (int i = 0; i < pathcount; i += 1) {
    point cur = getgriditem(grid, path[i]);

    point ds[4] = {{.x = -2, .y = 0},
                   {.x = 0, .y = -2},
                   {.x = 2, .y = 0},
                   {.x = 0, .y = 2}};

    for (int i = 0; i < 4; i += 1) {
      point next = getgriditem(grid, padd(cur, ds[i]));
      if ((next.count - cur.count) - 2 >= 100) {
        res1 += 1;
      }
    }
  }

  int res2 = 0;
  for (int i = 0; i < pathcount; i += 1) {
    point cur = getgriditem(grid, path[i]);

    for (int m = 2; m <= 20; m += 1) {
      int manhcount = 0;
      point *manhneighbors = getmanhattanedgepoints(cur, m, &manhcount);
      for (int n = 0; n < manhcount; n += 1) {
        point next = getgriditem(grid, manhneighbors[n]);
        if ((next.count - cur.count) - m >= 100) {
          res2 += 1;
        }
      }
    }
  }

  printf("star1 %d\n", res1);
  printf("star2 %d\n", res2);

  return EXIT_SUCCESS;
}
