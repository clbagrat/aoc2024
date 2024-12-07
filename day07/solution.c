#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct move {
  long prevValue;
  int ismult;
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
    return (move){0, 0, 0, -1};
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

long getnumberfromseq(long main, long *items, int amount) {
  stack realstack;
  stack* stack = &realstack;

  initstack(stack);

  move startmove = {.index = 0, .value = items[0], .prevValue = 0, .ismult = 0};
  push(stack, (move){0, 0, items[0], 0});
  long res = 0;

  while (!isempty(stack)) {
    move move = pop(stack);
    long t = move.ismult ? move.prevValue * move.value : move.prevValue + move.value;

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
    struct move multmove = {t ,1, items[newindex], newindex};
    struct move plusmove = {t ,0, items[newindex], newindex};
    push(stack, multmove);
    push(stack, plusmove);
  }

  freestack(stack);
  return res;
}

long getnumberfromline(char *line) {
  char *delim = " ";
  char *token = strtok(line, delim);

  long mainnum;
  long nums[50];
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
  return getnumberfromseq(mainnum, nums, i);
}

int main() {
  FILE *file = fopen("./day07/input", "r");

  char line[256];

  long star1 = 0;
  while (fgets(line, sizeof(line), file)) {
    star1 += getnumberfromline(line);
  }
  printf("star1: %lu\n", star1);

  return EXIT_SUCCESS;
}
