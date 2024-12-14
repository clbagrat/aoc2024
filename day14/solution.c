#include <stdio.h>
#include <stdlib.h>

int pmod(int a, int b) { return ((a % b) + b) % b; }

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
  while (fgets(line, sizeof(line), file)) {
    int x;
    int y;
    int vx;
    int vy;

    sscanf(line, "p=%d,%d v=%d,%d", &x, &y, &vx, &vy);
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

  printf("sum %d\n", sum);
  return EXIT_SUCCESS;
}
