#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *file = fopen("./day25/input", "r");

  char line[256];
  int maxamount = 0;

  while (fgets(line, sizeof(line), file)) {
    if (line[0] != '\n')  {
      maxamount += 1;
    }
  }
  rewind(file);
  int keys[maxamount][5];
  int keycount = 0;
  int locks[maxamount][5];
  int lockcount = 0;
  int i = -1;
  int islockinprogress = 0;
  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '\n')  {
      continue;
    }
    i += 1;
    if (i % 7 == 0) {
      int hasdot = 0;
      for (int t = 0; t < 5; t += 1) {
        hasdot = hasdot || line[t] == '.';
        keys[keycount][t] = 0;
        locks[lockcount][t] = 0;
      }

      islockinprogress = hasdot == 0;
      continue;
    }

    if ((i+1)%7 == 0) {
      if (islockinprogress) {
        lockcount += 1;
      } else {
        keycount += 1;
      }
      continue;
    }

    for (int c = 0; c < 5; c += 1) {
      if (line[c] == '#') {
        if (islockinprogress) {
          locks[lockcount][c] += 1;
        } else {
          keys[keycount][c] += 1;
        }
      }
    }
  }
  int res = 0;
  for (int k = 0; k < keycount; k += 1)  {
    int *key = keys[k];
    for (int l =0; l < lockcount; l += 1) {
      int *lock = locks[l];
      int dontfit = 0;
      for (int i = 0; i < 5; i += 1) {
        if (key[i] + lock[i] > 5) {
          dontfit = 1;
          break;
        };
      }
      res += dontfit ? 0 : 1;
    }
  }

  printf("result: %d\n", res);
  return EXIT_SUCCESS;
}
