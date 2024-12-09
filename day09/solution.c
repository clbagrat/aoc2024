#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printmem(int *mem, int count) {
  for (int i = 0; i < count; i += 1) {
    if (mem[i] < 0) {
      printf(".");
    } else {
      printf("%d", mem[i]);
    }
  }
  printf("\n");
}

int getfirstnonempy(int *list, int start)
{
  while (start) {
    if (list[start] >= 0) {
      return start;
    }
    start -= 1;
  }
  return -1;
}

void swap(int *list, int a, int b) {
  int temp = list[a];
  list[a] = list[b];
  list[b] = temp;
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
    if (unfolded[left] < 0 ) {
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

 printf("star 1: %lu\n", star1(content));
 // unfold memory
 // condence
 free(content);
}
