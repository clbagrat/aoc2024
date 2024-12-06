#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {
  int x;
  int y;
  int dx;
  int dy;
  int meta;
} RECORD;

struct Point {
  int x;
  int y;
};

int is_in(struct Record *all, int count, struct Record a) {
  for (int i = 0; i < count; i += 1) {
    if (all[i].x == a.x && all[i].y == a.y && all[i].dx == a.dx &&
        all[i].dy == a.dy) {
      return 1;
    }
  }

  return 0;
}

int is_in_points(struct Point *all, int count, struct Point a) {
  for (int i = 0; i < count; i += 1) {
    if (all[i].x == a.x && all[i].y == a.y) {
      return 1;
    }
  }

  return 0;
}

void rotate(struct Record *p) {
  int temp = p->dx;
  p->dx = -p->dy;
  p->dy = temp;
}

void print_record(struct Record a) {
  printf(":: x:%d y:%d dx:%d dy:%d meta:%d ::\n", a.x, a.y, a.dx, a.dy, a.meta);
}

void print_records(struct Record *all, int count) {
  for (int i = 0; i < count; i += 1) {
    printf(":: x:%d y:%d dx:%d dy:%d meta:%d ::\n", all[i].x, all[i].y,
           all[i].dx, all[i].dy, all[i].meta);
  }
}
void print_points(struct Point *all, int count) {
  for (int i = 0; i < count; i += 1) {
    printf(":: x:%d y:%d ::\n", all[i].x, all[i].y);
  }
}

int go(struct Point *obstales, int obstaclesCount, struct Record position,
        int max, struct Record **log, int *logCount) {
  *logCount = 0;
  int logCapacity = 10;

  while (1) {
    if (!is_in(*log, *logCount, position)) {
      if (*logCount >= logCapacity - 1) {
        logCapacity *= 2;
        struct Record *temp = realloc(*log, logCapacity * sizeof(RECORD));

        *log = temp;
      }
      position.meta = *logCount;
      (*log)[*logCount].x = position.x;
      (*log)[*logCount].y = position.y;
      (*log)[*logCount].dx = position.dx;
      (*log)[*logCount].dy = position.dy;
      (*log)[*logCount].meta = position.meta;
      *logCount += 1;
    } else {
      return 1;
    }

    struct Record next = {.x = position.x + position.dx,
                          .y = position.y + position.dy,
                          .dx = position.dx,
                          .dy = position.dy};

    if (next.x >= max || next.x < 0 || next.y >= max || next.y < 0) {
      return 0;
    }

    struct Point nextPoint = {.x = next.x, .y = next.y};
    if (is_in_points(obstales, obstaclesCount, nextPoint)) {
      struct Record toRotate = {.x = position.x,
                                .y = position.y,
                                .dx = position.dx,
                                .dy = position.dy};
      rotate(&toRotate);
      position = toRotate;
    } else {
      position = next;
    }
  }
  return 0;
}

RECORD find_last(RECORD *log, int logCount, int x, int y) {
  RECORD res = {.x = -1, .y = -1};

  for (int i = 0; i < logCount; i += 1) {
    if (log[i].x == x && log[i].y == y) {
      res = log[i];
    }
  }

  return res;
}

void print_beautyful(RECORD *log, int logCount, struct Point *obstacles,
                     int obstaclesCount, int max) {
  struct Point *grid = malloc((max * max + 1) * sizeof(struct Point));

  for (int i = 0; i < max * max; i += 1) {
    if (i % (max) == 0) {
      printf("\n");
    }
    int x = i % (max);
    int y = i / (max);

    RECORD lastLog = find_last(log, logCount, x, y);
    struct Point p = {.x = x, .y = y};
    if (is_in_points(obstacles, obstaclesCount, p)) {
      if (x == obstacles[obstaclesCount - 1].x &&
          y == obstacles[obstaclesCount - 1].y) {
        printf("O");
      } else {

        printf("#");
      }
    } else if (lastLog.x >= 0) {
      if (x == log[0].x && y == log[0].y) {

        if (log[0].dy == -1) {
          printf("^");
        } else if (log[0].dy == 1) {
          printf("v");
        } else if (log[0].dx == 1) {
          printf(">");
        } else if (log[0].dx == -1) {
          printf("<");
        }

      } else if (lastLog.dy == 0) {
        printf("-");
      } else if (lastLog.dx == 0) {
        printf("|");
      }
    } else {
      printf(".");
    }
  }
  printf("\n");
  printf("\n");
}

void star1(struct Point *obstales, int obstaclesCount,
           struct Point startPosition, int max) {

  struct Record position = {
      .x = startPosition.x, .y = startPosition.y, .dx = 0, .dy = -1};
  int isLooped = 0;
  int logCount = 0;
  struct Record *log = malloc(10 * sizeof(RECORD));

  go(obstales, obstaclesCount, position, max, &log, &logCount);

  struct Point *unique = malloc(max*max * sizeof( struct Point));
  int uniqueCount = 0;

  for (int i = 0; i < logCount; i += 1) {
    struct Point p = {.x=log[i].x, .y = log[i].y};
    if (!is_in_points(unique, uniqueCount, p)) {
      unique[uniqueCount] = p;
      uniqueCount += 1;
    }
  }

  printf("star1: %d\n", uniqueCount);
}

void star2(struct Point *obstales, int obstaclesCount,
           struct Point startPosition, int max) {

  struct Record position = {
      .x = startPosition.x, .y = startPosition.y, .dx = 0, .dy = -1};
  int isLooped = 0;
  int logCount = 0;
  struct Record *log = malloc(10 * sizeof(RECORD));

  go(obstales, obstaclesCount, position, max, &log, &logCount);
  //print_beautyful(log, logCount, obstales, obstaclesCount, max);

  int loops = 0;
  struct Point *unique = malloc(max*max * sizeof( struct Point));
  int uniqueCount = 0;
  int copies = 0;

  for (int i = 1; i < logCount; i += 1) {
    printf("%d/%d - %d  %d\n", i, logCount, loops, copies);
    struct Point p = {.x=log[i].x, .y = log[i].y};
    if (!is_in_points(unique, uniqueCount, p)) {
      unique[uniqueCount] = p;
      uniqueCount += 1;
    } else {
      copies += 1;
      continue;
    }
    struct Point *newObstacles =
        malloc((obstaclesCount + 1) * sizeof(struct Point));
    memcpy(newObstacles, obstales, obstaclesCount * sizeof(struct Point));
    newObstacles[obstaclesCount].x = log[i].x;
    newObstacles[obstaclesCount].y = log[i].y;

    int isL = 0;
    int newLogCount = 0;
    struct Record *newLog = malloc(10 * sizeof(RECORD));
    isL = go(newObstacles, obstaclesCount + 1, log[i-1], max, &newLog, &newLogCount);
    if (isL && 0) {
      print_beautyful(newLog, newLogCount, newObstacles, obstaclesCount + 1,
                      max);
    }

    loops += isL;
    free(newObstacles);
    free(newLog);
  }

  printf("%d \n", loops);
}

int main() {

  FILE *file = fopen("./day06/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  fclose(file);


  struct Point *obstacles = malloc(fileSize * sizeof(struct Point));
  struct Point position;

  int i = 0;
  int max = 0;
  int obstaclesCount = 0;
  while (i < fileSize) {
    char c = content[i];
    int x = max ? i % (max + 1) : i;
    int y = max ? i / (max + 1) : 0;

    if (!max && c == '\n') {
      max = i;
    }

    if (c == '#') {
      obstacles[obstaclesCount].x = x;
      obstacles[obstaclesCount].y = y;
      obstaclesCount += 1;
    }

    if (c == '^') {
      position.x = x;
      position.y = y;
    }

    i++;
  }

  // print_points(obstacles, obstaclesCount);

  star2(obstacles, obstaclesCount, position, max);
  star1(obstacles, obstaclesCount, position, max);
}
