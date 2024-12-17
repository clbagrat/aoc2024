#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// EXAMPLE START
// long A = 117440;
// long B = 0;
// long C = 0;
// long program[6] = {0,3,5,4,3,0};
// long programcount = 6;
// EXAMPLE END

// INPUT START
long A = 17323786;
long B = 0;
long C = 0;
long program[16] = {2, 4, 1, 1, 7, 5, 1, 5, 4, 1, 5, 5, 0, 3, 3, 0};
long programcount = 16;
// INPUT END

long getcombo(long i) {
  long combo[7] = {0, 1, 2, 3, A, B, C};
  return combo[i];
}

void run(long *res, long *rescount) {
  *rescount = 0;
  long p = 0;
  while (p < programcount) {
    long inst = program[p];
    long operand = program[p + 1];
    p += 2;

    if (inst == 0) {
      A = A / (int)pow(2, getcombo(operand));
    }

    if (inst == 1) {
      B = B ^ operand;
    }

    if (inst == 2) {
      B = getcombo(operand) % 8;
    }

    if (inst == 3) {
      if (A != 0) {
        p = operand;
        continue;
      }
    }

    if (inst == 4) {
      B = B ^ C;
    }

    if (inst == 5) {
      res[*rescount] = (getcombo(operand) % 8);
      *rescount += 1;
    }

    if (inst == 6) {
      B = A / (int)pow(2, getcombo(operand));
    }

    if (inst == 7) {
      C = A / (int)pow(2, getcombo(operand));
    }
  }
}

long isequal(long *a, long *b, long count) {
  for (long i = 0; i < count; i += 1) {
    if (a[i] != b[i])
      return 0;
  }
  return 1;
}

void printarray(long *arr, long count) {
  for (long i = 0; i < count; i += 1) {
    printf("%lu", arr[i]);
  }
  printf("\n");
}

void getlastn(long *array, long size, long n, long *result) {
  if (n > size) {
    n = size; 
  }
  for (int i = 0; i < n; i++) {
    result[i] = array[size - n + i];
  }
}

int main() {
  long res[500];
  long rescount = 0;
  run(res, &rescount);

  printf("star1: ");
  for (long i = 0; i < rescount; i += 1) {
    if (i == rescount - 1) {
      printf("%lu \n", res[i]);
    } else {
      printf("%lu,", res[i]);
    }
  }

  long basea = 0;
  for (long i = 1; i <= programcount; i += 1) {
    long target[i];
    getlastn(program, programcount, i, target);
    while (1) {
      A = basea;
      B = 0;
      C = 0;
      run(res, &rescount);
      if (isequal(target, res, i)) {
        if (i == programcount) {
          printf("star2: %lu", basea);
        }
        break;
      }
      basea += 1;
    }
    basea *= 8;
  }

  return EXIT_SUCCESS;
}
