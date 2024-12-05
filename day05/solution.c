#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rule {
  int left;
  int right;
};


int getMiddleNumberStar1(char *start, struct Rule *rules, size_t rule_count) {
      int *already_met = NULL;
      size_t already_met_count = 0;

      int broken = 0;
      while (*start != '\0' && !broken) {
        char tmp[2];
        memcpy(tmp, start, 2);
        int num = atoi(tmp);

        int i = 0;
        while (i <= rule_count) {
          if (rules[i].left == num) {
            int u = 0;
            while (u < already_met_count) {
              if (already_met[u] == rules[i].right) {
                broken = 1;
                break;
              }
              u += 1;
            }
          }
          i += 1;
        }
        already_met =
            realloc(already_met, (already_met_count + 1) * sizeof(int));
        already_met[already_met_count] = num;
        already_met_count += 1;

        if (*(start + 2) == '\0') {
          break;
        }
        start += 3;
      }

      int a = 0;
      if (!broken) {
        a = already_met[already_met_count / 2];
      }

      free(already_met);
      return a;
}

int getMiddleNumberStar2(char *line, struct Rule *rules, size_t rule_count) {
      int *already_met = NULL;
      size_t already_met_count = 0;
      int pos = 0;

      int broken = 0;
      while (line[pos] != '\0') {
        char tmp[2];
        memcpy(tmp, &line[pos], 2);
        int num = atoi(tmp);

        int i = 0;
        while (i <= rule_count) {
          if (rules[i].left == num) {
            int u = 0;
            while (u < already_met_count) {
              if (already_met[u] == rules[i].right) {
                char tmp = line[u*3];
                line[u*3] = line[pos];
                line[pos] = tmp;
                tmp = line[u*3 + 1];
                line[u*3+1] = line[pos+1];
                line[pos+1] = tmp;

                broken = 1;
                pos = 0;
                already_met = NULL;
                already_met_count = 0;
                break;
              }
              u += 1;
            }
          }
          i += 1;
        }
        already_met =
            realloc(already_met, (already_met_count + 1) * sizeof(int));
        already_met[already_met_count] = num;
        already_met_count += 1;

        if (line[pos + 2] == '\0') {
          break;
        }
        pos += 3;
      }

      int a = 0;
      if (broken) {
        a = already_met[already_met_count / 2];
      }

      free(already_met);
      return a;
}

int main() {
  FILE *file = fopen("./day05/example", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);

  char *delim = "\r\n";
  char *token = strtok(content, delim);

  struct Rule *rules = NULL;
  size_t rule_count = 0;

  int star1 = 0;
  int star2 = 0;
  while (token != NULL) {
    if (strchr(token, '|')) {
      int small, big;
      if (sscanf(token, "%d|%d", &small, &big) != 2) {
        fprintf(stderr, "Error: Invalid line format: %s", token);
        continue;
      }
      rules = realloc(rules, (rule_count + 1) * sizeof(struct Rule));
      rules[rule_count].right = big;
      rules[rule_count].left = small;
      rule_count += 1;
    }

    if (strchr(token, ',')) {
      char *t1 = malloc(strlen(token) + 1);
      memcpy(t1, token, strlen(token) + 1);
      star1 += getMiddleNumberStar1(t1, rules,  rule_count);
      char *t2 = malloc(strlen(token) + 1);
      memcpy(t2, token, strlen(token) + 1);
      star2 += getMiddleNumberStar2(t2, rules,  rule_count);
      free(t1);
      free(t2);
    }

    token = strtok(NULL, delim);
  }

  printf("star1 %d\n", star1);
  printf("star2 %d\n", star2);
  return EXIT_SUCCESS;
}
