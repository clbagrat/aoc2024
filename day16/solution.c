#include <stdio.h>
#include <stdlib.h>

typedef struct point {
  int x;
  int y;
  int dir;
} point;

typedef struct grid {
  char *data;
  int max;
} grid;

typedef struct revnode {
  point pos;
  int dir;
  struct revnode *revs[3];
  int revcount;
} revnode;

typedef struct state {
  point pos;
  int dir; // 0 - 3 (l, t, r, b)
  int cost;
  revnode *prevrevnode;
} state;

typedef struct pqueue {
  state *heap;
  int size;
  int capacity;
} pq;

pq *createpq(int capacity) {
  pq *pqu = (pq *)malloc(sizeof(pq));
  pqu->heap = (state *)malloc(capacity * sizeof(state));
  pqu->size = 0;
  pqu->capacity = capacity;
  return pqu;
}

void swap(state *a, state *b) {
  state temp = *a;
  *a = *b;
  *b = temp;
}

void push(pq *pq, state state) {
  if (pq->size >= pq->capacity)
    return;
  pq->heap[pq->size] = state;
  int idx = pq->size++;

  while (idx > 0) {
    int parent = (idx - 1) / 2;
    if (pq->heap[parent].cost <= pq->heap[idx].cost)
      break;
    swap(&pq->heap[parent], &pq->heap[idx]);
    idx = parent;
  }
}

state pop(pq *pq) {
  state top = pq->heap[0];
  pq->heap[0] = pq->heap[--pq->size];
  int idx = 0;

  while (1) {
    int left = 2 * idx + 1, right = 2 * idx + 2, smallest = idx;
    if (left < pq->size && pq->heap[left].cost < pq->heap[smallest].cost)
      smallest = left;
    if (right < pq->size && pq->heap[right].cost < pq->heap[smallest].cost)
      smallest = right;
    if (smallest == idx)
      break;
    swap(&pq->heap[idx], &pq->heap[smallest]);
    idx = smallest;
  }

  return top;
}

int isempty(pq *pq) { return pq->size == 0; }

void freepriorityqueue(pq *pq) {
  free(pq->heap);
  free(pq);
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
  return grid.data[ptoi(p, grid.max)];
}

void setgridchar(grid grid, point p) {
  if (!ispointingrid(grid, p)) {
    return;
  }
  grid.data[ptoi(p, grid.max)] = '0';
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

point rc(point p) {
  point rotated;
  rotated.x = p.y;
  rotated.y = -p.x;
  return rotated;
}

point rcc(point p) {
  point rotated;
  rotated.x = -p.y;
  rotated.y = p.x;
  return rotated;
}

revnode *getorcreate(revnode *nodes, int *count, point pos, int dir) {
  for (int i = 0; i < *count; i += 1) {
    if (nodes[i].pos.x == pos.x && nodes[i].pos.y == pos.y &&
        nodes[i].dir == dir) {
      return &nodes[i];
    }
  }
  revnode newnode = {.pos = pos, .dir = dir, .revcount = 0};
  nodes[*count] = newnode;
  *count += 1;
  return &nodes[*count - 1];
}

int pmod(int a, int b) { return ((a % b) + b) % b; }

int isequal(struct point p1, struct point p2) {
    return (p1.x == p2.x) && (p1.y == p2.y);
}

int countuniquepoints(struct point points[], int n) {
    int uniquecount = 0;

    int isduplicate[n];
    for (int i = 0; i < n; i+=1) {
        isduplicate[i] = 0;
    }

    for (int i = 0; i < n; i+=1) {
        if (isduplicate[i]) continue;

        uniquecount+=1; 
        for (int j = i + 1; j < n; j++) {
            if (isequal(points[i], points[j])) {
                isduplicate[j] = 1;
            }
        }
    }
    return uniquecount;
}

void calcstar2(revnode *node, point *uniq, int *count, grid grid) {
  for (int i = 0; i < *count; i += 1) {
    if (uniq[i].x == node->pos.x && uniq[i].y == node->pos.y &&
        uniq[i].dir == node->dir) {
      return;
    }
  }
  uniq[*count] = node->pos;
  uniq[*count].dir = node->dir;
  *count += 1;
  for (int r = 0; r < node->revcount; r += 1) {
    calcstar2(node->revs[r], uniq, count, grid);
  }
}

int main() {
  FILE *file = fopen("./day16/input", "r");
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  rewind(file);

  char *content = malloc(filesize + 1);
  fread(content, 1, filesize, file);
  fclose(file);
  content[filesize] = '\0';

  int i = 0;
  char *griddata = malloc(filesize + 1);
  int gridcount = 0;
  grid grid = {.data = griddata, .max = 0};

  point start;
  point end;

  point ds[4] = {
      {.x = -1, .y = 0}, {.x = 0, .y = -1}, {.x = 1, .y = 0}, {.x = 0, .y = 1}};

  while (i < filesize) {
    char c = content[i];

    int x = grid.max ? i % (grid.max + 1) : i;
    int y = grid.max ? i / (grid.max + 1) : 0;

    if (!grid.max && c == '\n') {
      grid.max = i;
    }

    if (c != '\n') {
      grid.data[gridcount] = c;
      gridcount += 1;
    }

    if (c == 'S') {
      start = (point){.x = x, .y = y};
    }

    if (c == 'E') {
      end = (point){.x = x, .y = y};
    }

    i += 1;
  }

  pq *pq = createpq(grid.max * grid.max * 3);
  state startstate = {.cost = 0, .pos = start, .dir = 2};
  revnode *revnodes = malloc(grid.max * grid.max * 3 * sizeof(revnode));
  int ***costs = (int ***)malloc(grid.max * sizeof(int **));
  for (int i = 0; i < grid.max; i++) {
    costs[i] = (int **)malloc(grid.max * sizeof(int *));
    for (int j = 0; j < grid.max; j++) {
      costs[i][j] = (int *)malloc(4 * sizeof(int));
      for (int d = 0; d < 4; d++) {
        costs[i][j][d] = 99999999;
      }
    }
  }

  int revnodescount = 0;

  push(pq, startstate);
  while (!isempty(pq)) {
    state s = pop(pq);

    int oldcost = costs[s.pos.x][s.pos.y][s.dir];
    if (oldcost == s.cost) {
      revnode *rn = getorcreate(revnodes, &revnodescount, s.pos, s.dir);
      rn->revs[rn->revcount] = s.prevrevnode;
      rn->revcount += 1;
    }

    if (oldcost > s.cost) {
      revnode *rn = getorcreate(revnodes, &revnodescount, s.pos, s.dir);

      if (getgridchar(grid, s.pos) != 'S') {
        rn->revs[0] = s.prevrevnode;
        rn->revcount = 1;
      }
    }
    if (getgridchar(grid, s.pos) == 'E') {
      revnode *rn = getorcreate(revnodes, &revnodescount, s.pos, s.dir);
      point uniq[grid.max * grid.max * 3];
      int uniqcount = 0;
      calcstar2(rn, uniq, &uniqcount, grid);
      printf("star1 %d\n", s.cost);
      printf("star2 %d\n", countuniquepoints(uniq, uniqcount));
      return 1;
    }

    if (oldcost <= s.cost)
      continue;

    revnode *cur = getorcreate(revnodes, &revnodescount, s.pos, s.dir);

    costs[s.pos.x][s.pos.y][s.dir] = s.cost;

    int newdirs[3] = {s.dir, pmod(s.dir + 1, 4), pmod(s.dir - 1, 4)};

    for (int i = 0; i < 3; i += 1) {
      point next = i == 0 ? padd(s.pos, ds[newdirs[i]]) : s.pos;
      if (ispointingrid(grid, next) && getgridchar(grid, next) != '#') {
        int nextdir = newdirs[i];
        int nextcost = s.cost + (i == 0 ? 1 : 1000);
        state newstate = {
            .pos = next, .dir = nextdir, .cost = nextcost, .prevrevnode = cur};
        push(pq, newstate);
      }
    }
  }

}
