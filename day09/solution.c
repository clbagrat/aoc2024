#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printmem(int *mem, int count) {
  int c = -1;
  for (int i = 0; i < count; i += 1) {
      if (mem[i] != c) {
    //    printf("|");
        c = mem[i];
      }
    if (mem[i] < 0) {
      printf(".");
    } else {
      printf("(%d)", mem[i]);
    }
  }
  printf("\n");
}

int getfirstnonempy(int *list, int start) {
  while (start) {
    if (list[start] >= 0) {
      return start;
    }
    start -= 1;
  }
  return -1;
}

void findnonemptyrange(int *list, int right, int *s, int *l) {
  int lookFor = -1;
  int sfake = 0;

  while (right) {
    int cur = list[right];
    if (lookFor >= 0) {
      if (cur != lookFor) {
        *l = sfake - right;
        *s = right + 1;
        return;
      }
    } else if (lookFor < 0 && cur >= 0) {
      lookFor = cur;
      sfake = right;
    }
    right -= 1;
  }

  *l = sfake + 1;
  *s = right;
}

int findemptyrange(int *list, int count, int search) {
  int l = 0;
  while (l < count - search) {
    int cur = list[l];
    int r = l + 1;
    if (cur == -1) {
      while (r < count) {
        if (r - l == search) {
          return l;
        }
        if (list[r] != -1) {
          l = r;
          break;
        }
        r += 1;
      }
    } else {

      l += 1;
    }
  }

  return -1;
}

void swap(int *list, int a, int b) {
  int temp = list[a];
  list[a] = list[b];
  list[b] = temp;
}

void swaprange(int *list, int count, int a, int b) {
  for (int i = 0; i < count; i += 1) {
    swap(list, a + i, b + i);
  }
}

long star2(char *content) {
  long len = strlen(content);

  size_t unfoldcapacity = len;
  int *unfolded = malloc(len * sizeof(int));
  long unfoldedlen = 0;

  for (long i = 0; i < len - 1; i += 1) {
    int isEmpty = i % 2 == 1;
    long id = i / 2;
    int amount = content[i] - '0';

    while (amount--) {
      if (unfoldedlen >= unfoldcapacity) {
        unfoldcapacity *= 2;
        unfolded = realloc(unfolded, unfoldcapacity * sizeof(int));
      }
      unfolded[unfoldedlen] = isEmpty ? -1 : id;
      unfoldedlen += 1;
    }
  }

  int left = 0;
  int right = unfoldedlen - 1;

  while (right >= 0) {
    int s = 0;
    int l = 0;
    findnonemptyrange(unfolded, right, &s, &l);
    int emptyindex = findemptyrange(unfolded, right + 1, l);
    if (emptyindex >= 0) {
      swaprange(unfolded, l, emptyindex, s);
    }
    right = s - 1;
  }

  printmem(unfolded, unfoldedlen);
  long res = 0;
  for (long i = 0; i < unfoldedlen; i += 1) {
    if (unfolded[i] >= 0) {
      res += unfolded[i] * i;
    }
  }

  return res;
}

long star1(char *content) {
  long len = strlen(content);

  size_t unfoldcapacity = len;
  int *unfolded = malloc(len * sizeof(int));
  long unfoldedlen = 0;

  for (long i = 0; i < len - 1; i += 1) {
    int isEmpty = i % 2 == 1;
    long id = i / 2;
    int amount = content[i] - '0';

    while (amount--) {
      if (unfoldedlen >= unfoldcapacity) {
        unfoldcapacity *= 2;
        unfolded = realloc(unfolded, unfoldcapacity * sizeof(int));
      }
      unfolded[unfoldedlen] = isEmpty ? -1 : id;
      unfoldedlen += 1;
    }
  }

  int left = 0;
  int right = unfoldedlen - 1;
  long res = 0;

  while (left < right) {
    if (unfolded[left] < 0) {
      right = getfirstnonempy(unfolded, right);
      swap(unfolded, left, right);
    }

    if (unfolded[left] >= 0) {
      res += unfolded[left] * left;
    }
    left += 1;
  }

  return res;
}

int main() {
  FILE *file = fopen("./day09/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  content[fileSize] = '\0';
  fclose(file);

  // printf("star 1: %lu\n", star1(content));
  printf("star 2: %lu\n", star2(content));
  // unfold memory
  // condence
  free(content);
}
