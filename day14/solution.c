#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point  {
  int x;
  int y;
} point;

int pmod(int a, int b) { return ((a % b) + b) % b; }

int ptoi(int x, int y, int max) {
  return y * max + x;
}

int main() {
  FILE *file = fopen("./day14/input", "r");

  // for example
  // int w = 11;
  // int h = 7;
  // for input
  int w = 101;
  int h = 103;

  char line[256];
  int seconds = 100;
  int res[4] = {0, 0, 0, 0};
  int robox[500][10000];
  int roboy[500][10000];
  int count = 0;
  while (fgets(line, sizeof(line), file)) {
    int x;
    int y;
    int vx;
    int vy;

    sscanf(line, "p=%d,%d v=%d,%d", &x, &y, &vx, &vy);
    for (int s = 0; s < 10000; s += 1) {
      robox[count][s] = pmod(x + vx * s, w);
      roboy[count][s] = pmod(y + vy * s, h);
    }
    count += 1;

    int nx = pmod(x + vx * seconds, w);
    int ny = pmod(y + vy * seconds, h);
    char b[3] = {'0', '0', '\0'};

    if (nx == w / 2 || ny == h / 2)
      continue;

    if (nx > w / 2) {
      b[0] = '1';
    }

    if (ny > h / 2) {
      b[1] = '1';
    }
    int qindex = (int)strtol(b, NULL, 2);
    res[qindex] += 1;
  }

  int sum = 1;
  for (int i = 0; i < 4; i += 1) {
    sum *= res[i];
  }

  for (int i = 0; i < 10000; i += 1) {
    char str [103*101 + 1];
    memset(str, '.', 103*101); 
    str[103*101] = '\0';
    for (int u = 0; u < 500; u += 1) {
      str[ptoi(robox[u][i], roboy[u][i], 101)] = '#';
    }
    if (strstr(str, "########") != NULL) {
      printf("tree %d\n", i);
    }
  }


  printf("sum %d\n", sum);
  return EXIT_SUCCESS;
}
