#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct sys {
  long ax;
  long bx;
  long ay;
  long by;
  long px;
  long py;
} sys;

typedef struct {
    double a;
    double b;
    long success;
} solution;

void printsys(sys sys) {
  printf("AX: %lu, AY: %lu, BX: %lu, BY: %lu, X: %lu, Y:%lu\n", sys.ax, sys.ay, sys.bx, sys.by, sys.px, sys.py);
}

long isactuallyint(double value) {
    return (value == floor(value));
}

solution solvesystem(sys sys) {
    long ax = sys.ax;
    long bx = sys.bx;
    long x = sys.px;
    long ay = sys.ay;
    long by = sys.by;
    long y = sys.py;

    solution result;

    double determinant = ax * by - bx * ay;

    if (determinant == 0) {
        result.success = 0;
        return result;
    }

    result.a = (x * by - bx * y) / determinant;
    result.b = (ax * y - x * ay) / determinant;

    if (isactuallyint(result.a) && isactuallyint(result.b)) {
      result.success = 1;
    } else {
      result.success = 0;
    }

    return result;
}


int main() {
  FILE *file = fopen("./day13/input", "r");

  char line[256];
  size_t capacity = 10;
  long count = 0;
  sys *allsystems = malloc(capacity * sizeof(sys));

  sys sysinptogress = (sys){};

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '\n') {
      if (sysinptogress.px >= 0) {
        if (count >= capacity - 1) {
          capacity *= 2;
          allsystems = realloc(allsystems, capacity * sizeof(sys));
        }
        allsystems[count] = sysinptogress;
        count += 1;
        sysinptogress = (sys){};
      }
      continue;
    }

    if (line[0] == 'P') {
      long x;
      long y;
      sscanf(line, "Prize: X=%lu, Y=%lu", &x, &y);
      sysinptogress.px = x + 10000000000000;
      sysinptogress.py = y + 10000000000000;
    } else if (line[7] == 'A') {
      long ax;
      long ay;
      sscanf(line, "Button A: X+%lu, Y+%lu", &ax, &ay);
      sysinptogress.ax = ax;
      sysinptogress.ay = ay;
    } else if (line[7] == 'B') {
      long bx;
      long by;
      sscanf(line, "Button B: X+%lu, Y+%lu", &bx, &by);
      sysinptogress.bx = bx;
      sysinptogress.by = by;
    }
  }

  allsystems[count] = sysinptogress;
  count += 1;
  sysinptogress = (sys){};

  long res = 0;
  for (long i = 0; i < count; i += 1) {
    solution solution = solvesystem(allsystems[i]);
    if (solution.success) {
      res += solution.a * 3 + solution.b;
    }
  }

  printf("%lu", res);
  free(allsystems);

  return EXIT_SUCCESS;
}
