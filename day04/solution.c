#include <stdio.h>
#include <stdlib.h>


int get(char lookFor, int myX, int myY, int dirX, int dirY, int *max, char *content) {
  if (myX + dirX >= *max || myX + dirX < 0) {
    return 0;
  }
  if (myY + dirY >= *max || myY + dirY < 0) {
    return 0;
  }

  char c = content[(myY + dirY) * (*max + 1) + (myX + dirX)];
  if (lookFor == c && lookFor == 'S') {
    return 1;
  }

  if (lookFor == c) {
    switch (c) {
      case 'M':
        return get('A', myX + dirX, myY + dirY, dirX, dirY, max, content);
      case 'A':
        return get('S', myX + dirX, myY + dirY, dirX, dirY, max, content);
    }
  }

  return 0;
}

int getChar(int myX, int myY, int dirX, int dirY, int *max, char *content) {
  if (myX + dirX >= *max || myX + dirX < 0) {
    return 0;
  }

  if (myY + dirY >= *max || myY + dirY < 0) {
    return 0;
  }

  char c = content[(myY + dirY) * (*max + 1) + (myX + dirX)];
  if (c != 'M' && c != 'S') {
    return 0;
  }
  return c;
}


int main() {
  FILE *file = fopen("./day04/input", "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file); // in bytes
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);

  int maxX = 0;
  while (content[maxX] != '\n') {
    maxX += 1;
  }
  int maxY = fileSize / (maxX + 1);

  int resStar1 = 0;
  int resStar2 = 0;

  for (int i = 0; i < maxX * maxY; i += 1) {
    int x = i % (maxX);
    int y = i / (maxX);
    char c = content[y * (maxX + 1) + x];

    if (c == 'X') {
      resStar1 += get('M', x, y, 0, 1, &maxX, content);
      resStar1 += get('M', x, y, 0, -1, &maxX, content);
      resStar1 += get('M', x, y, 1, 1, &maxX, content);
      resStar1 += get('M', x, y, 1, -1, &maxX, content);
      resStar1 += get('M', x, y, 1, 0, &maxX, content);
      resStar1 += get('M', x, y, -1, 0, &maxX, content);
      resStar1 += get('M', x, y, -1, 1, &maxX, content);
      resStar1 += get('M', x, y, -1, -1, &maxX, content);
    }

    if (c == 'A') {
      char lt = getChar(x, y, -1, 1, &maxX, content);
      char rb = getChar(x, y, 1, -1, &maxX, content);
      char rt = getChar(x, y, 1, 1, &maxX, content);
      char lb = getChar(x, y, -1, -1, &maxX, content);

      if (lt * rb * rt * lb != 0) {
        if (lt != rb && rt != lb) {
          resStar2 += 1;
        }
      }
    }
  }

  printf("star1: %d\n", resStar1);
  printf("star2: %d\n", resStar2);

  return EXIT_SUCCESS;
}
