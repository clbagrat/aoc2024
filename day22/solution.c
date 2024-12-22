#include <stdio.h>
#include <stdlib.h>

#define S 20
#define SCount 4
#define ITERS 2000

long getsecret(long val) {
  long sn = val;
  sn = ((sn * 64) ^ sn) % 16777216;
  sn = ((sn / 32) ^ sn) % 16777216;
  sn = ((sn * 2048) ^ sn) % 16777216;
  return sn;
}

long getsecretit(long init, int its) {
  long sn = init;
  for (int i = 0; i < its; i += 1) {
    sn = getsecret(sn);
  }
  return sn;
}

int D (int v) {
  return v + 10;
}

void fill(int (*hash)[S][S][S][S], int (*sequence)[ITERS][SCount], long value) {
  int diffs[ITERS];
  int bananas[ITERS];
  long sn = value;
  int prev = sn % 10;
  bananas[0] = prev;
  for (int it = 1; it < ITERS; it += 1) {
    sn = getsecret(sn);
    int n = sn % 10;
    bananas[it] = n;
    diffs[it] = n - prev;
    prev = n;
    if (it > 4) {
      int a = D(diffs[it - 3]);
      int b = D(diffs[it - 2]);
      int c = D(diffs[it - 1]);
      int d = D(diffs[it]);
      int v = bananas[it];
      if ((*hash)[a][b][c][d] < 0) {
        (*hash)[a][b][c][d] = v;
      }
      (*sequence)[it - 5][0] = a;
      (*sequence)[it - 5][1] = b;
      (*sequence)[it - 5][2] = c;
      (*sequence)[it - 5][3] = d;
    }
  }

}

long max(long a, long b) {
  if (a > b) {
    return a;
  }
  return b;
}
int main() {
  FILE *file = fopen("./day22/input", "r");
  char line[256];

  int amount = 0;

  // star 1
  long res1 = 0;
  while (fgets(line, sizeof(line), file)) {
    long cost = atol(line);
    res1 += getsecretit(cost, 2000);
    amount += 1;
  }
  printf("star 1 %lu\n", res1);

  // star 2
  int(*hashes[amount])[S][S][S][S];

  for (int i = 0; i < amount; i += 1) {
    hashes[i] = malloc(sizeof(int[S][S][S][S]));
  }

  for (int n = 0; n < amount; n += 1) {
    for (int i = 0; i < S; i += 1) {
      for (int j = 0; j < S; j += 1) {
        for (int k = 0; k < S; k += 1) {
          for (int l = 0; l < S; l += 1) {
            (*hashes[n])[i][j][k][l] = -1;
          }
        }
      }
    }
  }

  int(*sequences[amount])[ITERS][SCount];

  for (int i = 0; i < amount; i += 1) {
    sequences[i] = malloc(sizeof(int[ITERS][SCount]));
  }

  for (int i = 0; i < amount; i += 1) {
    for (int j = 0; j < ITERS; j += 1) {
      for (int k = 0; k < SCount; k += 1) {
        (*sequences[i])[j][k] = -1;
      }
    }
  }

  rewind(file);
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    fill(hashes[i], sequences[i], atoi(line));
    i += 1;
  }

  long res2 = 0;
  long maximum = 0;
  for (int m = 0; m < ITERS - SCount - 1; m += 1) {
    int a = (*sequences[0])[m][0];
    int b = (*sequences[0])[m][1];
    int c = (*sequences[0])[m][2];
    int d = (*sequences[0])[m][3];

    long interm = 0;
    for (int n = 0; n < amount; n += 1) {
      int v = (*hashes[n])[a][b][c][d];
      interm += v > 0 ? v : 0;
    }
    maximum = max(maximum, interm);
  }

  printf("star 2 %lu\n", maximum);
  return EXIT_SUCCESS;
}
