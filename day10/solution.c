#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
  int x;
  int y;
  int value;
  struct point *targets;
  int targetcount;
} point;

void printmap(point *map, int max) {
  for (int i = 0; i < max * max; i += 1) {
    if (i % max == 0) {
      printf("\n");
    }
    point p = map[i];
    printf("%d", p.value);
  }
  printf("\n");
}

int ispointin(point a, int max) {
  return !(a.x >= max || a.x < 0 || a.y >= max || a.y < 0);
}

int ptoi(point p, int max) { return p.y * max + p.x; }

void addtargetuniq(point *p, point t) {
  // COMMENT THIS FOR STAR 2
  for (int i = 0; i < p->targetcount; i += 1) {
    if (p->targets[i].x == t.x && p->targets[i].y == t.y) {
      return;
    }
  }

  if (p->targetcount == 0) {
    p->targets = malloc(sizeof(point));
    p->targetcount = 1;
  } else {
    p->targetcount += 1;
    p->targets = realloc(p->targets, p->targetcount * sizeof(point));
  }
  p->targets[p->targetcount - 1] = t;
}

void star1and2(point *map, int max) {
  int lookfor = 8;

  int res = 0;
  while (lookfor >= 0) {
    for (int i = 0; i < max * max; i += 1) {
      point *p = &map[i];
      if (p->value == lookfor) {
        point neighbors[4] = {{.x = p->x + 1, .y = p->y},
                              {.x = p->x - 1, .y = p->y},
                              {.x = p->x, .y = p->y + 1},
                              {.x = p->x, .y = p->y - 1}};

        for (int u = 0; u < 4; u += 1) {
          point tmp = neighbors[u];
          if (!ispointin(tmp, max))
            continue;
          ;
          point *neighbor = &map[ptoi(tmp, max)];
          if (neighbor->value == lookfor + 1) {
            if (neighbor->value == 9) {
              addtargetuniq(p, *neighbor);
            } else {
              for (int z = 0; z < neighbor->targetcount; z += 1) {
                addtargetuniq(p, neighbor->targets[z]);
              }
            }
          }
        }
      }
      if (lookfor == 0) {
        if (p->value == 0) {
          printf("%d\n", p->targetcount);
          res += p->targetcount;
        }
      }
    }

    lookfor -= 1;
  }
  printf("res: %d \n", res);
}

int main() {
  FILE *file = fopen("./day10/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  fclose(file);
  content[fileSize] = '\0';

  point *wholemap = malloc(strlen(content) * sizeof(point));
  int mapcount = 0;
  int i = 0;
  int max = 0;
  while (i < fileSize) {
    char c = content[i];
    int x = max ? i % (max + 1) : i;
    int y = max ? i / (max + 1) : 0;

    if (!max && c == '\n') {
      max = i;
    }
    if (c != '\n') {
      point newpoint = {.x = x, .y = y, .value = content[i] - '0'};
      wholemap[mapcount] = newpoint;
      mapcount += 1;
    }
    i += 1;
  }

  printmap(wholemap, max);
  star1and2(wholemap, max);

  free(content);
  free(wholemap);
}
