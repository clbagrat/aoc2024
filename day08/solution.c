#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
  int x;
  int y;
} point;

void pointprint(point a) {
  printf("%d - %d\n",a.x, a.y);
}
point direction(point a, point b) {
  return (point){.x= a.x - b.x, .y = a.y - b.y};
}

point shift(point a, point b) {
  return (point){.x= a.x + b.x, .y = a.y + b.y};
}

int ispointin(point a, int max) {
    return !(a.x >= max || a.x < 0 || a.y >= max || a.y < 0);
}

int indexbypoint(point a, int max) {
  return a.y*(max + 1) + a.x;
}

void insertuniq(char *array, int *size, char val) {
    int alreadyExists = 0;
    for (int i = 0; i < *size; i++) {
        if (array[i] == val) {
            alreadyExists = 1;
            break;
        }
    }
    if (!alreadyExists) {
        array[*size] = val;
        (*size)++;
    }
}

char* copy(char* line, long size) {
    char* copy = malloc(size + 1);
    if (copy) {
      memcpy(copy, line, size);
    }
    return copy;
}

void markantinode(char *map, char *result, int max, char freq) {
  point *met = malloc(max * max * sizeof(point));
  int metcount = 0;
  for (int i = 0; i < max * max; i += 1) {
    int x = i % (max);
    int y = i / (max);
    point cur = {.x=x, .y=y};
    if (map[indexbypoint(cur, max)] != freq) {
      continue;
    } 
    int ci = 0;
    while (ci < metcount) {
      point antinode1 = shift(cur, direction(cur, met[ci]));
      point antinode2 = shift(met[ci], direction(met[ci], cur));
      if (ispointin(antinode1, max)) {
        result[indexbypoint(antinode1, max)] = '#';
      }

     if (ispointin(antinode2, max)) {
       result[indexbypoint(antinode2, max)] = '#';
     }

      ci += 1;
    }
      printf("%c was met %d times \n", map[indexbypoint(cur, max)], metcount);
    met[metcount] = cur;
    metcount += 1;
  }

  free(met);
}

void star1(char *content, long contentsize ){
  char *antenafreqs = malloc((26*26+10) * sizeof(char));
  char *result = malloc(contentsize + 1);
  long i = 0;
  int max = 0;

  int antenafreqscount = 0;
  while (i < contentsize) {
    char c = content[i];
    if (!max && c == '\n') {
      max = i;
    }
    if (c != '.' && c != '\n') {
      insertuniq(antenafreqs, &antenafreqscount, c);
    }
    if (c == '\n') {
      result[i] = c;
    } else {
      result[i] = '.';
    }
    i += 1;
  }
  result[i] = '\0';

  int curantena = 0;
  while (curantena < antenafreqscount) {
    printf("marking %c \n", antenafreqs[curantena]);
    markantinode(content, result,  max, antenafreqs[curantena]);
    curantena += 1;
  }

  printf("%s", result);

  int u = 0;
  int antinodecount = 0;
  while (u < contentsize) {
    antinodecount += result[u] == '#';
    u += 1;
  }
  printf("%d\n", antinodecount);

  // create result string
  // for each antenafreqs
  //    
  free(antenafreqs);
  free(result);
}

int main () {
  FILE *file = fopen("./day08/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  fclose(file);

  char *star1content = copy(content, fileSize);
  star1(star1content, fileSize);
  free(star1content);
  free(content);
}
