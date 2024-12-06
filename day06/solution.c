#include <stdio.h>
#include <stdlib.h>

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
    printf(":: x:%d y:%d dx:%d dy:%d meta:%d ::\n", all[i].x, all[i].y, all[i].dx,
           all[i].dy, all[i].meta);
  }
}
//
// void print(struct Point *obstales, int obstaclesCount, struct Point *visited,
//            int visitedCount, int max) {
//
//   for (int i = 0; i < max * max; i += 1) {
//     int x = i % (max);
//     int y = i / (max);
//     struct Point p = {.x = x, .y = y};
//
//     if (is_in_light(obstales, obstaclesCount, p)) {
//       printf("#");
//     } else if (is_in_light(visited, visitedCount, p)) {
//       printf("X");
//     } else {
//       printf(".");
//     }
//     if (x == max - 1) {
//       printf("\n");
//     }
//   }
//   printf("\n\n");
// }
//

void go(struct Point *obstales, int obstaclesCount, struct Record position,
        int max, struct Record **log, int *logCount, int *isLooped) {
  *logCount = 0;
  int logCapacity = 10;

  while (1) {
    if (!is_in(*log, *logCount, position)) {
      if (*logCount >= logCapacity - 1) {
        logCapacity *= 2;
        printf("alloc %d %lu\n", logCapacity,
               logCapacity * sizeof(RECORD));
        struct Record *temp =  realloc(*log, logCapacity * sizeof(RECORD));

        *log = temp;
        printf("alloc %p \n", log);

      }
      position.meta = *logCount;
      print_record(position);
      (*log)[*logCount].x = position.x;
      (*log)[*logCount].y = position.y;
      (*log)[*logCount].dx = position.dx;
      (*log)[*logCount].dy = position.dy;
      (*log)[*logCount].meta = position.meta;
      *logCount += 1;
    } else {
      *isLooped = 1;
      return;
    }

    struct Record next = {.x = position.x + position.dx,
                          .y = position.y + position.dy,
                          .dx = position.dx,
                          .dy = position.dy};

    if (next.x >= max || next.x < 0 || next.y >= max || next.y < 0) {
      return;
      break;
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

  // while (!isOut) {
  //   if (!is_in(visited, *visitedCount, position)) {
  //     visited[*visitedCount].x = position.x;
  //     visited[*visitedCount].y = position.y;
  //     visited[*visitedCount].dx = direction.x;
  //     visited[*visitedCount].dy = direction.y;
  //     *visitedCount += 1;
  //   }

  //   struct Point next = {.x = position.x + direction.x,
  //                        .y = position.y + direction.y,
  //                        .dx = position.dx,
  //                        .dy = position.dy};
  //   if (next.x >= max || next.x < 0 || next.y >= max || next.y < 0) {
  //     isOut = 1;
  //     break;
  //   }

  //   if (is_in(obstales, obstaclesCount, next)) {
  //     rotate(&position);
  //   } else {
  //     position = next;
  //   }
  // }
}

void star2(struct Point *obstales, int obstaclesCount,
           struct Point startPosition, int max) {

  struct Record position = {
      .x = startPosition.x, .y = startPosition.y, .dx = 0, .dy = -1};
  int isLooped = 0;
  int logCount = 0;
  struct Record *log = (RECORD *)malloc(10 * sizeof(RECORD));

  printf("%p\n", &log);
  go(obstales, obstaclesCount, position, max, &log, &logCount, &isLooped);

  printf("%p\n", &log);
  print_records(log, logCount);

  // struct Point *potential = malloc(obstaclesCount * sizeof(struct Point));
  // int potentialCount = 0;
  // get_all_visited(obstales, obstaclesCount, position, max, potential,
  //                 &potentialCount);

  // int loops = 0;
  // for (int i = 1; i < potentialCount; i += 1) {
  //   printf("%d %d \n", i, potentialCount);
  //   struct Point *newObstacles =
  //       malloc((obstaclesCount + 1) * sizeof(struct Point));
  //   memcpy(newObstacles, obstales, obstaclesCount * sizeof(struct Point));
  //   newObstacles[obstaclesCount].x = potential[i].x;
  //   newObstacles[obstaclesCount].y = potential[i].y;

  //   loops += is_looped(newObstacles, obstaclesCount + 1, potential[i - 1],
  //   max); free(newObstacles);
  // }

  // printf("star 2: %d\n", loops);
  // free(potential);
}

int main() {

 // struct Record *log = (RECORD *)malloc(10 * sizeof(RECORD));
 // struct Record *log2 = (RECORD *)malloc(10 * sizeof(RECORD));
 // log[9].meta = 1;
 // print_records(log, 10);
 // return EXIT_SUCCESS;
  FILE *file = fopen("./day06/example", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);

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

  // star1(obstacles, obstaclesCount, position, max);
  star2(obstacles, obstaclesCount, position, max);
}
