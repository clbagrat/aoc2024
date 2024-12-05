#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rule {
  int left;
  int right;
};

int main() {
  FILE *file = fopen("./day05/input", "r");

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *content = malloc(fileSize + 1);
  fread(content, 1, fileSize, file);

  //  char line[256];
  //  while (fgets(line, sizeof(line), file)){
  //    if (strchr(line, '|')) {
  //
  //      printf("%s", line);
  //    }
  //  }

  //  split_by_crlf(content);
  // split by 2 new lines
  // first part rules, second pages
  // go over each line in pages
  // get a number
  // find all rules where this number is last
  // check if encountered number conteins what was first
  // add to encountered

  char *delim = "\r\n";
  char *token = strtok(content, delim);

  struct Rule *rules = NULL;
  size_t rule_count = 0;

  int super_res = 0;
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
      char *start = token;
      printf("%s\n", start);
      int *already_met = NULL;
      size_t already_met_count = 0;

      int broken = 0;
      // printf("Segment: %s\n", lol);
      while (*start != '\0' && !broken) {
        char tmp[2];
        memcpy(tmp, start, 2);
        int num = atoi(tmp);

        printf("::: %d\n", num);
        int i = 0;
        while (i <= rule_count) {
          if (rules[i].left == num) {
            int u = 0;
            while (u < already_met_count) {
              if (already_met[u] == rules[i].right) {
                broken = 1;
                printf("broken %d\n", already_met[u]);
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
        printf("--%s--\n", start);
        start += 3;
      }

      if (!broken) {
        int a = already_met[already_met_count / 2];


        super_res += a;
      }

      free(already_met);
    }

    token = strtok(NULL, delim);
  }

  printf("%d", super_res);
  return EXIT_SUCCESS;
}
