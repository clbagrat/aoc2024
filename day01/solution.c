#include <stdio.h>
#include <stdlib.h>

void insert(int *array, int *size, int value) {
  int i = *size - 1;
  while (i >= 0 && array[i] > value) {
    array[i + 1] = array[i];
    i--;
  }
  array[i + 1] = value;
  (*size)++;
}

int get_similarity_index(int value, int* array, int size) {
  int count = 0;
  int i = 0;
  while (i < size) {
    if (array[i] == value) {
      count += 1;
    }
    i ++;
  }

  return value * count;
}

int main() {
    FILE *file = fopen("./day01/input", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // i know the max size :D 
    int *array1 = malloc(1000 * sizeof(int));
    int *array2 = malloc(1000 * sizeof(int));
    int size1 = 0;
    int size2 = 0;

    char line[256]; 
    while (fgets(line, sizeof(line), file)) {
        int num1, num2;
        if (sscanf(line, "%d %d", &num1, &num2) != 2) {
            fprintf(stderr, "Error: Invalid line format: %s", line);
            continue;
        }

        insert(array1, &size1, num1);
        insert(array2, &size2, num2);
    }

    int star1result = 0;
    int star2result = 0;

    for (int i = 0; i < size1; i++) {
        int sortedNum1 = array1[i];
        int sortedNum2 = array2[i];
        star1result += abs(sortedNum2 - sortedNum1);
        star2result += get_similarity_index(sortedNum1, array2, size1);
    }

    printf("result1: %d\n", star1result);
    printf("result2: %d\n", star2result);

    free(array1);
    free(array2);

    fclose(file);
    return EXIT_SUCCESS;
}
