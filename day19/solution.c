#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long MAX_COUNT = 5;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct node {
  char val;
  long end;
  struct node **next;
  long nextcount;
} node;

node *createnode(char value, long end) {
  node *newnode = (node *)malloc(sizeof(node));
  newnode->val = value;
  newnode->end = end;
  newnode->next = malloc(MAX_COUNT * sizeof(node *));
  newnode->nextcount = 0;
  return newnode;
}

long find(node *n, char letter, node **res) {
  for (long i = 0; i < n->nextcount; i += 1) {
    if (n->next[i]->val == letter) {
      *res = n->next[i];
      return 1;
    };
  }
  return 0;
}

void addword(char *str, long len, node *head) {
  node *cur = head;
  for (long i = 0; i < len; i += 1) {
    char t = str[i];
    node *next;
    if (find(cur, t, &next)) {
      cur = next;
    } else {
      next = createnode(t, 0);
      cur->next[cur->nextcount] = next;
      cur->nextcount += 1;
      cur = next;
    }
  }
  cur->end = 1;
}

long COMPLETE = 2;
long PART = 1;
long DONT_EXIST = 0;
long checkword(char *str, long len, node *head) {
  node *cur = head;
  for (long i = 0; i < len; i += 1) {
    char t = str[i];
    node *next;
    if (find(cur, t, &next)) {
      cur = next;
    } else {
      return 0;
    }
  }
  if (cur->val == '$')
    return 0;
  if (cur->end) {
    return 2;
  }
  return 1;
}

long checkpattern(char *towel, long len, node *head) {
  long dp[len+1];
  dp[0] = 1;
  for(long i = 1; i <= len; i += 1) {
    dp[i] = 0;
  }

  for (long i = 1; i <= len; i += 1) {
    for (long j = 0; j < i; j += 1)  {
      char otherpart[i-j + 1];
      strncpy(otherpart, towel+j, i-j);
      otherpart[i-j] = '\0';
      if (dp[j] && checkword(otherpart, i-j, head) == COMPLETE) {
        dp[i] = 1;
        break;
      }
    }
  }

  return dp[len];
}

long countpatterns(char *towel, long len, node *head) {
  long dp[len+1];
  dp[0] = 1;
  for(long i = 1; i <= len; i += 1) {
    dp[i] = 0;
  }

  for (long i = 1; i <= len; i += 1) {
    for (long j = 0; j < i; j += 1)  {
      char otherpart[i-j + 1];
      strncpy(otherpart, towel+j, i-j);
      otherpart[i-j] = '\0';
      if (dp[j] && checkword(otherpart, i-j, head) == COMPLETE) {
        dp[i] += dp[j];
      }
    }
  }

  return dp[len];
}


void printwords(node *cur, char *buffer, long depth) {
  if (cur->val != '\0') {
    buffer[depth] = cur->val;
  }

  if (cur->end) {
    buffer[depth + 1] = '\0';
    printf("%s\n", buffer);
  }

  for (long i = 0; i < cur->nextcount; i++) {
    printwords(cur->next[i], buffer, depth + 1);
  }
}

void printtree(node *head) {
  long maxdepth = 100;
  char *buffer = (char *)malloc(maxdepth * sizeof(char));
  printwords(head, buffer, 0);

  free(buffer);
}

int main() {
  FILE *file = fopen("./day19/input", "r");

  char line[3000];
  node *trie = createnode('$', 0);

  long i = 0;
  long res1 = 0;
  long res2 = 0;
  while (fgets(line, sizeof(line), file)) {
    if (i == 0) {

      char *delim = ", \n";
      char *token = strtok(line, delim);

      while (token != NULL) {
        addword(token, strlen(token), trie);
        token = strtok(NULL, delim);
      }
    }

    if (i > 1) {
        res1 += checkpattern(line, strlen(line) - 1, trie);
        res2 += countpatterns(line, strlen(line) - 1, trie);
      }
    i += 1;
  }

  printf("star 1 %lu\n", res1);
  printf("star 2 %lu\n", res2);
  return EXIT_SUCCESS;
}
