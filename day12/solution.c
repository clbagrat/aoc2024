#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
  int x;
  int y;
  char name;
} point;

typedef struct node {
  point data;
  struct node *next;
} node;

typedef struct q {
  node *head;
  node *tail;

} q;

void initq(q *q) {
  q->head = NULL;
  q->tail = NULL;
}

int isempty(q *q) { return q->head == NULL; }

void enq(q *q, point point) {
  node *newnode = malloc(sizeof(node));
  newnode->data = point;
  newnode->next = NULL;

  if (isempty(q)) {
    q->head = newnode;
  } else {
    q->tail->next = newnode;
  }

  q->tail = newnode;
}

point deq(q *q) {
  if (isempty(q)) {
    return (point){.x = -1, .y = -1, .name = '0'};
  }

  node *tmp = q->head;
  point val = tmp->data;
  q->head = q->head->next;

  if (q->head == NULL) {
    q->tail = NULL;
  }

  free(tmp);
  return val;
}

void freeq(q *q) {
  while (!isempty(q)) {
    deq(q);
  }
}

void printgrid(point *grid, int max) {
  for (int i = 0; i < max * max; i += 1) {
    if (i % max == 0) {
      printf("\n");
    }
    printf("%c", grid[i].name);
  }
  printf("\n");
}

int compare(point a, point b) { return a.x == b.x && a.y == b.y; }

int ispointin(point a, int max) {
  return !(a.x >= max || a.x < 0 || a.y >= max || a.y < 0);
}

int ptoi(point p, int max) { return p.y * max + p.x; }

int getneighbor(point *grid, int max, int x, int y, point *neighbor) {
  if (x >= 0 && x < max && y >= 0 && y < max) {
    *neighbor = grid[y * max + x];
    return 1;
  }
  *neighbor = (point){-1, -1};
  return 0;
}

int isin(point *arr, int count, point p) {
  for (int i = 0; i < count; i += 1) {
    if (compare(arr[i], p)) {
      return 1;
    }
  }
  return 0;
}

int getcorners(point *figure, int count) {
  int r = 0;
  for (int i = 0; i < count; i += 1) {
      point p = figure[i];
      point ds[4] = {{.x = -1, .y = -1},
                     {.x = 1, .y = 1},
                     {.x = 1, .y = -1},
                     {.x = -1, .y = 1}};

      for (int u = 0; u < 4; u += 1) {
        point t = {.x = p.x + ds[u].x, .y = p.y + ds[u].y};
        point projx =  {.x = p.x, .y = t.y};
        point projy =  {.x = t.x, .y = p.y};
        int pxin = isin(figure, count, projx);
        int pyin = isin(figure, count, projy);
        if (pxin == 0 && pyin == 0) {
          r += 1;
        }
        if (pxin == 1 && pyin == 1 && !isin(figure, count, t)) {
          r += 1;
        }
      }
  }

  return r;
}

int star1(point *grid, int max) {
  int *visited = malloc(max * max * sizeof(int));
  char cur = '0';
  int star1 = 0;
  int star2 = 0;

  for (int i = 0; i < max * max; i += 1) {
    point nextp = grid[i];
    if (visited[i] == 1) {
      continue;
    }
    int perimeter = 0;
    int area = 0;
    point *figure = malloc(max * max * sizeof(point));
    q realq;
    q *q = &realq;
    initq(q);
    enq(q, nextp);

    while (!isempty(q)) {
      point p = deq(q);
      if (visited[ptoi(p, max)] == 1)
        continue;
      visited[ptoi(p, max)] = 1;
      figure[area] = p;
      area += 1;
      point ds[4] = {{.x = -1, .y = 0},
                     {.x = 1, .y = 0},
                     {.x = 0, .y = -1},
                     {.x = 0, .y = 1}};
      for (int n = 0; n < 4; n += 1) {
        point neighbor;
        if (getneighbor(grid, max, p.x + ds[n].x, p.y + ds[n].y, &neighbor)) {
          if (neighbor.name != p.name) {
            perimeter += 1;
          } else {
            if (visited[ptoi(neighbor, max)] != 1) {
              enq(q, neighbor);
            }
          }
        } else {
          perimeter += 1;
        }
      }
    }
    star1 += perimeter * area;
    star2 += area * getcorners(figure, area);
    freeq(q);
    free(figure);
  }

  printf("res1: %d \n", star1);
  printf("res2: %d \n", star2);

  return 0;
}

int main() {
  FILE *file = fopen("./day12/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  fclose(file);
  content[fileSize] = '\0';
  point *grid = malloc(strlen(content) * sizeof(point));

  int max = 0;
  int i = 0;
  int gridcount = 0;

  while (i < fileSize) {
    char c = content[i];
    int x = max ? i % (max + 1) : i;
    int y = max ? i / (max + 1) : 0;

    if (!max && c == '\n') {
      max = i;
    }

    if (c != '\n') {
      point newpoint = {.name = c, .x = x, .y = y};
      grid[gridcount] = newpoint;
      gridcount += 1;
    }

    i += 1;
  }

  printgrid(grid, max);
  star1(grid, max);

  free(content);
  free(grid);
  return EXIT_SUCCESS;
}
