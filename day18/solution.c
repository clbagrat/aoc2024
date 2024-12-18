#include <stdio.h>
#include <stdlib.h>

typedef struct point {
  int x;
  int y;
} point;

typedef struct grid {
  char *data;
  int max;
} grid;

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
    return (point){.x = -1, .y = -1};
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

void printgrid(grid grid) {
  for (int i = 0; i < grid.max * grid.max; i += 1) {
    if (i % grid.max == 0) {
      printf("\n");
    }
    printf("%c", grid.data[i]);
  }
  printf("\n");
}

grid creategrid(int max) {
  char *griddata = malloc(max * max * sizeof(char));
  grid grid = {.data = griddata, .max = max};
  for (int i = 0; i < max * max; i += 1) {
    grid.data[i] = '.';
  }
  return grid;
}

int ptoi(point p, int max) { return p.y * max + p.x; }

int ispointingrid(grid grid, point p) {
  return !(p.x >= grid.max || p.x < 0 || p.y >= grid.max || p.y < 0);
}

void setgridchar(grid grid, point p, char ch) {
  if (!ispointingrid(grid, p)) {
    return;
  }
  grid.data[ptoi(p, grid.max)] = ch;
}

int maxofthree(int a, int b, int c) {
  return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}
int comparepoints(point a, point b) { return a.x == b.x && a.y == b.y; }

int ispointinlist(point *list, int count, point p) {
  for (int i = 0; i < count; i += 1) {
    if (comparepoints(p, list[i])) {
      return 1;
    }
  }
  return 0;
}

int getindexinlist(point *list, int count, point p) {
  for (int i = 0; i < count; i += 1) {
    if (comparepoints(p, list[i])) {
      return i;
    }
  }
  return -1;
}

point padd(point p, point add) {
  return (point){.x = p.x + add.x, .y = p.y + add.y};
}

int hasneighbor(grid grid, point p, point dir) {
  return ispointingrid(grid, padd(p, dir));
}

char getgridchar(grid grid, point p) {
  if (!ispointingrid(grid, p)) {
    return '#';
  }
  return grid.data[ptoi(p, grid.max)];
}

int trackback(grid grid, point *visited, point *parents, int count,
              point startfrom) {
  int curindex = getindexinlist(visited, count, startfrom);
  int res = 0;
  while (curindex != -1) {
    res += 1;
    point parentbyindex = parents[curindex];
    curindex = getindexinlist(visited, count, parentbyindex);
  }
  return res - 1;
}

int findpath(grid grid, point from, point to) {
  q parentq;
  q *pq = &parentq;
  q realq;
  q *q = &realq;
  initq(q);
  initq(pq);
  point visited[grid.max * grid.max];
  point parents[grid.max * grid.max];
  int count = 0;
  enq(q, from);
  while (!isempty(q)) {
    point cur = deq(q);
    point parent = deq(pq);
    if (ispointinlist(visited, count, cur)) {
      continue;
    }

    visited[count] = cur;
    parents[count] = parent;
    count += 1;

    if (comparepoints(cur, to)) {
      return trackback(grid, visited, parents, count, cur);
    }

    point ds[4] = {{.x = -1, .y = 0},
                   {.x = 1, .y = 0},
                   {.x = 0, .y = -1},
                   {.x = 0, .y = 1}};
    for (int i = 0; i < 4; i += 1) {
      point n = padd(cur, ds[i]);
      if (getgridchar(grid, n) != '#') {
        enq(q, n);
        enq(pq, cur);
      }
    }
  }
  return -1;
}
int main() {
  FILE *file = fopen("./day18/input", "r");

  char line[256];
  size_t capacity = 10;
  int count = 0;
  point *fbytes = malloc(capacity * sizeof(point));
  int max = 0;
  int tosimulate = 1024; // change for the input

  while (fgets(line, sizeof(line), file)) {
    int x, y;
    sscanf(line, "%d,%d", &x, &y);
    if (count == capacity) {
      capacity *= 2;
      fbytes = reallocf(fbytes, capacity * sizeof(point));
    }
    max = maxofthree(max, x, y);

    fbytes[count] = (point){.x = x, .y = y};
    count += 1;
  }
  max += 1;

  grid grid = creategrid(max);
  for (int i = 0; i < tosimulate; i += 1) {
    setgridchar(grid, fbytes[i], '#');
  }
  int star1 = findpath(grid, (point){.x = 0, .y = 0},
                       (point){.x = max - 1, .y = max - 1});
  printf("star 1: %d \n", star1);

  int low = tosimulate, high = count;
  int result = -1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    struct grid newgrid = creategrid(max);
    for (int i = 0; i < mid; i += 1) {
      setgridchar(newgrid, fbytes[i], '#');
    }
    int steps = findpath(newgrid, (point){.x = 0, .y = 0},
                 (point){.x = max - 1, .y = max - 1});
    if (steps == -1) {
      result = mid; 
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  printf("star 2: %d,%d", fbytes[result-1].x, fbytes[result-1].y);

  return EXIT_SUCCESS;
}
