#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int star1() {

  FILE *file = fopen("./day02/example", "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  int res = 0;
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    bool isSafe = true;
    bool trend = false;
    int lastNumber = 0;
    char forToken[256];
    char *token = strtok(memcpy(forToken, line, sizeof(line)), " ");
    int i = 0;

    while (token != NULL) {
      int num = atoi(token);
      if (i > 0) {
        int diff = num - lastNumber;

        if (abs(diff) == 0 || abs(diff) > 3) {
          isSafe = false;
          break;
        }
        if (i == 1) {
          trend = diff > 0;
        }

        if ((trend == true && diff < 0 || trend == false && diff > 0)) {
          isSafe = false;
          break;
        }
      }

      token = strtok(NULL, " ");
      i += 1;
      lastNumber = num;
    }

    if (isSafe) {
      res += 1;
    }
  };

  printf("star 1 : %d \n", res);

  fclose(file);
  return EXIT_SUCCESS;
}

void parse_line(const char *line, int *array, int *count) {
  char *copy = strdup(line);
  char *token = strtok(copy, " ");

  while (token != NULL) {
    int num = atoi(token);
    array[*count] = num;
    (*count)++;
    token = strtok(NULL, " ");
  }

  free(copy);
}

bool is_safe(int *array, int count) {
    int copy[1000];
    memcpy(copy, array, sizeof(copy));

    bool isSafe = true;
    bool alreadyEncountered = false;
    bool trend = false;
    int lastNumber = 0;
    char forToken[256];
    int i = 0;

    while (i < count) {
      int num = copy[i];
      if (i > 0) {
        int diff = num - lastNumber;

        if (abs(diff) == 0 || abs(diff) > 3) {
          if (alreadyEncountered) {
            isSafe = false;
            break;
          } else {
            alreadyEncountered = true;
            i += 1;
            continue;
          }
        }
        if (i == 1) {
          trend = diff > 0;
        }

        if ((trend == true && diff < 0 || trend == false && diff > 0)) {
          if (alreadyEncountered) {
            isSafe = false;
            break;
          } else {
            alreadyEncountered = true;
            i += 1;
            continue;
          }
        }
      }

      i += 1;
      lastNumber = num;

    }

   return isSafe;
}

int* reverseArray(int *arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
    return arr;
}


int star2() {

  FILE *file = fopen("./day02/input", "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  int res = 0;
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    int array[1000];
    int count = 0;
    parse_line(line, array, &count);

    if (is_safe(array, count) || is_safe(reverseArray(array, count), count)) {
      res += 1;
    } 
  };

  printf("star 2 : %d \n", res);

  fclose(file);
  return EXIT_SUCCESS;
}

int main() {
  star1();
  star2();
  return EXIT_SUCCESS;
}
