#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int star1(char *content) {
  regex_t regex;
  int regexRes;

  regcomp(&regex, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED);
  regmatch_t match[3];

  int res = 0;
  int start = 0;
  while (1) {
    char *substr = content + start;
    regexRes = regexec(&regex, substr, 3, match, 0);
    if (regexRes) {
      break;
    }

    int leftSize = match[1].rm_eo - match[1].rm_so;
    char *leftSubstr = malloc(leftSize * sizeof(char));
    strncpy(leftSubstr, content + start + match[1].rm_so, leftSize);

    int rightSize = match[2].rm_eo - match[2].rm_so;
    char *rightSubstr = malloc(rightSize * sizeof(char));
    strncpy(rightSubstr, content + start + match[2].rm_so, rightSize);

    res += (atoi(leftSubstr) * atoi(rightSubstr));
    start += match[0].rm_eo;
    free(leftSubstr);
    free(rightSubstr);
  }

  return res;
}

void delete_range(char *str, long long N, long long M) {
  int len = strlen(str);
  int start = (int)(N - 1);
  int end = (int)(M);

  for (int i = end; i < len; i++) {
    str[start + i - end] = str[i];
  }

  // Null-terminate the string after shifting
  str[len - (end - start)] = '\0';
}

int main() {
  FILE *file = fopen("./day03/input", "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);
  printf("star1: %d\n", star1(content));

  char *finalSubsts = malloc(fileSize + 1);
  strncpy(finalSubsts, content, fileSize + 1);

  regex_t regex;
  int regexRes;
  regcomp(&regex, "don\'t\\(\\)(.*?)do\\(\\)", REG_EXTENDED | REG_ENHANCED);
  while (1) {
    regmatch_t match[1];
    regexRes = regexec(&regex, finalSubsts, 1, match, 0);
    if (regexRes) {
      break;
    }
    delete_range(finalSubsts, match[0].rm_so + 1, match[0].rm_eo);
  }

  regcomp(&regex, "don\'t\\(\\).*", REG_EXTENDED | REG_ENHANCED);
  regmatch_t match[1];
  regexRes = regexec(&regex, finalSubsts, 1, match, 0);
  delete_range(finalSubsts, match[0].rm_so + 1, match[0].rm_eo);

  printf("star2: %d\n", star1(finalSubsts));

  return EXIT_SUCCESS;
}
