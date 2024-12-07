#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum operator{ MULT, ADD, ELEPHANT };

typedef struct move {
  long prevValue;
  enum operator operator;
  long value;
  int index;
} move;

typedef struct node {
  move data;
  struct node *next;
} node;

typedef struct stack {
  node *top;
} stack;

void initstack(stack *stack) { stack->top = NULL; }

int isempty(stack *stack) { return stack->top == NULL; }

void push(stack *stack, move move) {
  node *newnode = malloc(sizeof(node));
  newnode->data = move;
  newnode->next = stack->top;
  stack->top = newnode;
}

move pop(stack *stack) {
  if (isempty(stack)) {
    printf("stack is empty\n");
    return (move){0, ADD, 0, -1};
  }
  node *temp = stack->top;
  move move = temp->data;
  stack->top = stack->top->next;
  free(temp);
  return move;
}

void freestack(stack *stack) {
  while (!isempty(stack)) {
    pop(stack);
  }
}

long concatlongs(long a, long b) {
    char temp[50]; 
    snprintf(temp, sizeof(temp), "%ld%ld", a, b);
    return atol(temp);
}

long getnumberfromseq(long main, long *items, int amount,
                      enum operator* operators, int operamount) {
  stack realstack;
  stack *stack = &realstack;

  initstack(stack);

  push(stack, (move){0, ADD, items[0], 0});
  long res = 0;

  while (!isempty(stack)) {
    move move = pop(stack);
    long t = 0;
    switch (move.operator) {
    case MULT:
      t = move.prevValue * move.value;
      break;
    case ADD:
      t = move.prevValue + move.value;
      break;
    case ELEPHANT:
      t = concatlongs(move.prevValue, move.value);
      break;
    }

    if (t == main && move.index == amount - 1) {
      res = main;
      break;
    }
    if (t > main) {
      continue;
    }
    int newindex = move.index + 1;
    if (newindex == amount) {
      continue;
    }

    for (int i = 0; i < operamount; i += 1) {
      push(stack, (struct move){t, operators[i], items[newindex], newindex});
    }
  }

  freestack(stack);
  return res;
}

long getnumberfromline(char *line, enum operator* operators, int amount) {
  char *delim = " ";
  char *token = strtok(line, delim);

  long mainnum;
  long nums[150];
  int i = -1;

  while (token != NULL) {
    long n = atol(token);
    if (i == -1) {
      mainnum = n;
    } else {
      nums[i] = n;
    }
    i += 1;
    token = strtok(NULL, delim);
  }
  return getnumberfromseq(mainnum, nums, i, operators, amount);
}

char* copyline(const char* line) {
    char* copy = malloc(strlen(line) + 1);
    if (copy) {
        strcpy(copy, line); 
    }
    return copy;
}

int main() {
  FILE *file = fopen("./day07/example", "r");

  char line[256];

  long star1 = 0;
  long star2 = 0;
  enum operator star1operators[2] = {MULT, ADD};
  enum operator star2operators[3] = {MULT, ADD, ELEPHANT};
  while (fgets(line, sizeof(line), file)) {
    char *star1line = copyline(line);
    star1 += getnumberfromline(star1line, star1operators, 2);
    free(star1line);

    char *star2line = copyline(line);
    star2 += getnumberfromline(star2line, star2operators, 3);
    free(star2line);
  }
  printf("star1: %lu\n", star1);
  printf("star2: %lu\n", star2);

  return EXIT_SUCCESS;
}
