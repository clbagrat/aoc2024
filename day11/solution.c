#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pair {
  long val;
  long count;
} pair;

int cansplit(long t) {
  int digits = (int)log10(t) + 1;
  return digits % 2 == 0;
}

void split(long t, long *left, long *right) {
  int digits = (int)log10(t) + 1;
  int half = digits / 2;
  long divisor = (long)pow(10, half);

  *left = t / divisor;
  *right = t % divisor;
}

void addOrIncrement(pair *stones, pair p, long *stonecount) {
  for (long i = 0; i < *stonecount; i += 1) {
    if (stones[i].val == p.val) {
      stones[i].count += p.count;
      return;
    }
  }

  stones[*stonecount] = (pair){.val = p.val, .count = p.count};
  *stonecount += 1;
}

pair *applyrule(pair *stones, long *stonecount) {
  pair *newstones = malloc((*stonecount * 2) * sizeof(pair));
  long newstonecount = 0;

  for (long i = 0; i < *stonecount; i += 1) {
    pair p = stones[i];
    if (p.val == 0) {
      p.val = 1;
      addOrIncrement(newstones, p, &newstonecount);
    } else if (cansplit(p.val)) {
      long left;
      long right;
      split(p.val, &left, &right);
      addOrIncrement(newstones, (pair){.val = left, .count = p.count},
                     &newstonecount);
      addOrIncrement(newstones, (pair){.val = right, .count = p.count},
                     &newstonecount);
    } else {
      p.val *= 2024;
      addOrIncrement(newstones, p, &newstonecount);
    }
  }
  *stonecount = newstonecount;
  return newstones;
}

int main() {
  FILE *file = fopen("./day11/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  fclose(file);
  content[fileSize] = '\0';

  size_t stonecapacity = 10;
  long stonecount = 0;
  pair *stones = malloc(stonecapacity * sizeof(pair));

  char *delim = " ";
  char *token = strtok(content, delim);

  while (token != NULL) {
    long val = atoi(token);
    stones[stonecount] = (pair){.val = val, .count = 1};
    stonecount += 1;
    token = strtok(NULL, delim);
  }

  long cachecapacity = 1000;
  long **cache = malloc(cachecapacity * sizeof(long *));

  clock_t start, end;
  double cpu_time_used;
  start = clock();

  // ADJUST it COUNT FOR STAR 1
  for (int it = 0; it < 75; it += 1) {
    stones = applyrule(stones, &stonecount);
  }

  long res = 0;

  for (long u = 0; u < stonecount; u += 1) {
    res += stones[u].count;
  }
  end = clock();
  cpu_time_used =
      ((double)(end - start)) / CLOCKS_PER_SEC; // Convert to seconds

  printf("time: %f \n", cpu_time_used);
  printf("%lu \n", res);

  free(content);
}
