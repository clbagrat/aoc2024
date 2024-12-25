#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum operator{ AND, OR, XOR } operator;

typedef struct operand {
  char *name;
  int val;
} operand;

typedef struct equation {
  operand *a;
  operand *b;
  operand *res;
  operator operator;
} equation;

typedef struct operandlist {
  operand *list;
  int count;
} operandlist;

typedef struct equationlist {
  equation *list;
  int count;
} equationlist;

operand *regoperand(operandlist *olist, char *name, int val) {
  for (int i = 0; i < olist->count; i += 1) {
    if (strcmp(olist->list[i].name, name) == 0) {
      return &(olist->list[i]);
    }
  }

  operand *new = malloc(sizeof(operand));
  new->name = (char *)malloc(strlen(name) + 1);
  strcpy(new->name, name);
  new->val = val;
  olist->list[olist->count] = *new;
  olist->count += 1;
  return &(olist->list[olist->count - 1]);
}

equation *regequation(equationlist *elist, operand *a,
                      operand *b, operator oper, operand *res) {
  equation *eq = malloc(sizeof(equation));
  eq->a = a;
  eq->b = b;
  eq->operator= oper;
  eq->res = res;
  elist->list[elist->count] = *eq;
  elist->count += 1;
  return &(elist->list[elist->count - 1]);
}

void printoperands(operandlist *olist) {
  for (int i = 0; i < olist->count; i += 1) {
    printf("%s = %d \n", olist->list[i].name, olist->list[i].val);
  }
}

void printequations(equationlist *elist) {
  for (int i = 0; i < elist->count; i += 1) {
    equation eq = elist->list[i];
    char *op;
    switch (eq.operator) {
    case AND:
      op = "AND";
      break;
    case OR:
      op = "OR";
      break;
    case XOR:
      op = "XOR";
      break;
    }
    printf("%s %s %s = %s \n", eq.a->name, op, eq.b->name, eq.res->name);
  }
}

void solveoperand(operand *operand, equationlist elist) {
  // see if all other operands are solved
  // if not, solve recursively

  for (int i = 0; i < elist.count; i += 1) {
    equation eq = elist.list[i];
    if (strcmp(operand->name, eq.res->name) == 0) { // find equation for operand
      if (eq.a->val < 0) {
        solveoperand(eq.a, elist);
      }
      if (eq.b->val < 0) {
        solveoperand(eq.b, elist);
      }
      if (eq.operator== AND) {
        eq.res->val = eq.a->val & eq.b->val;
      }
      if (eq.operator== OR) {
        eq.res->val = eq.a->val | eq.b->val;
      }
      if (eq.operator== XOR) {
        eq.res->val = eq.a->val ^ eq.b->val;
      }
    }
  }
}

long binary2decimal(int *binary, int length) {
  long decimal = 0;

  for (int i = 0; i < length; i++) {
    if (binary[i] == 1) {
      decimal += pow(2, i);
    }
  }
  return decimal;
}

int *binaryaddition(int *a, int *b, int size, int *resultsize) {
  int carry = 0;
  int *result =
      (int *)malloc((size + 1) * sizeof(int));

  for (int i = 0; i < size; i += 1) {
    int sum = a[i] + b[i] + carry;
    result[i] = sum % 2;
    carry = sum / 2;  
  }

  if (carry) {
    result[size] = 1;
    *resultsize = size + 1;
  } else {
    *resultsize = size;
  }

  return result; 
}

void fill(int *arr, int count) {
  for (int i = 0; i < count; i += 1) {
    arr[i] = 0;
  }
}

int main() {
  FILE *file = fopen("./day24/input", "r");

  char line[256];
  int maxamount = 0;

  while (fgets(line, sizeof(line), file)) {
    maxamount += 1;
  }
  rewind(file);

  equation *eqs = malloc(maxamount * sizeof(equation));
  equationlist elist = {.list = eqs, .count = 0};

  operand *ops = malloc(maxamount * sizeof(operand));
  operandlist olist = {.list = ops, .count = 0};

  int *zbinaryarray = malloc(100 * sizeof(int));
  int zbinarycount = 0;
  int *xbinaryarray = malloc(100 * sizeof(int));
  fill(xbinaryarray, 100);
  int xbinarycount = 0;
  int *ybinaryarray = malloc(100 * sizeof(int));
  fill(ybinaryarray, 100);
  int ybinarycount = 0;

  while (fgets(line, sizeof(line), file)) {
    if (line[3] == ':') { // operand
      char *token = strtok(line, ": ");
      char name[4];
      int val;
      strncpy(name, token, 3);
      token = strtok(NULL, ": ");
      val = atoi(token);
      regoperand(&olist, name, val);
      if (name[0] == 'x') {
        xbinaryarray[atoi(name + 1)] = val;
        xbinarycount += 1;
      }
      if (name[0] == 'y') {
        ybinaryarray[atoi(name + 1)] = val;
        ybinarycount += 1;
      }
    } else if (line[0] != '\n') { // equation
      char *delim = " ";
      char *token = strtok(line, delim);
      char aname[4];
      strncpy(aname, token, 3);
      operand *a = regoperand(&olist, aname, -1);

      token = strtok(NULL, delim);
      operator oper = XOR;
      if (token[0] == 'O') {
        oper = OR;
      } else if (token[0] == 'A') {
        oper = AND;
      }

      token = strtok(NULL, delim);
      char bname[4];
      strncpy(bname, token, 3);
      operand *b = regoperand(&olist, bname, -1);
      token = strtok(NULL, delim); // skipping ->
      token = strtok(NULL, delim);
      char resname[4];
      strncpy(resname, token, 3);
      operand *res = regoperand(&olist, resname, -1);

      regequation(&elist, a, b, oper, res);
    }
  }
  for (int i = 0; i < elist.count; i += 1) {
    equation eq = elist.list[i];
    if (eq.res->name[0] == 'z') {
      solveoperand(eq.res, elist);
      zbinaryarray[atoi(eq.res->name + 1)] = eq.res->val;
      zbinarycount += 1;
    }
  }

  printf("Star 1: %lu\n", binary2decimal(zbinaryarray, zbinarycount));
  printf("\n-------\n");
  printf("Start 2: second part is solved manually.\ni noticed that all good Zs created by XOR or anything random and another XOR of x and y of the same level. \nThen i was chaning equations of broken\nZ's untill they'll look as i just described.\n");

  printf("x: ");
  for (int u = 0; u < zbinarycount; u += 1) {
    printf("%d", xbinaryarray[zbinarycount - u - 1]);
  }
  printf("\n");
  printf("y: ");
  for (int u = 0; u < zbinarycount; u += 1) {
    printf("%d", ybinaryarray[zbinarycount - u - 1]);
  }
  printf("\n");
  printf("z: ");
  for (int u = 0; u < zbinarycount; u += 1) {
    printf("%d", zbinaryarray[zbinarycount - u - 1]);
  }
  int resultsize;
  int *xplusy =
      binaryaddition(xbinaryarray, ybinaryarray, xbinarycount, &resultsize);
  printf("\n");
  printf("c: ");
  for (int u = 0; u < resultsize; u += 1) {
    printf("%d", xplusy[zbinarycount - u - 1]);
  }


  return EXIT_SUCCESS;
}
