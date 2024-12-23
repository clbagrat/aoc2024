#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_len 3
int maxcount = 0; // will change

void add_connection(char nodes[][max_len], int graph[][maxcount], char *a,
                    char *b, int *node_count) {
  int index_a = -1, index_b = -1;

  for (int i = 0; i < *node_count; i++) {
    if (strcmp(nodes[i], a) == 0) {
      index_a = i;
      break;
    }
  }

  for (int i = 0; i < *node_count; i++) {
    if (strcmp(nodes[i], b) == 0) {
      index_b = i;
      break;
    }
  }

  if (index_a == -1) {
    index_a = *node_count;
    strncpy(nodes[index_a], a, max_len);
    (*node_count)++;
  }

  if (index_b == -1) {
    index_b = *node_count;
    strncpy(nodes[index_b], b, max_len);
    (*node_count)++;
  }

  graph[index_a][index_b] = 1;
  graph[index_b][index_a] = 1;
}

int findloopswitht(int graph[maxcount][maxcount], char ns[max_len][max_len],
                   int n) {
  int res = 0;
  for (int u = 0; u < n; u++) {
    for (int v = u + 1; v < n; v++) {
      for (int w = v + 1; w < n; w++) {
        if (graph[u][v] && graph[u][w] && graph[v][w]) {
          if (ns[u][0] == 't' || ns[v][0] == 't' || ns[w][0] == 't') {
            res += 1;
          }
        }
      }
    }
  }
  return res;
}

void bronkerbosch(int graph[maxcount][maxcount], int n, int *R, int r_size,
                  int *P, int p_size, int *X, int x_size, int *max_clique,
                  int *max_size) {
  if (p_size == 0 && x_size == 0) {
    if (r_size > *max_size) {
      *max_size = r_size;
      memcpy(max_clique, R, r_size * sizeof(int));
    }
    return;
  }

  for (int i = 0; i < p_size; i++) {
    int v = P[i];
    int new_R[r_size + 1];
    memcpy(new_R, R, r_size * sizeof(int));
    new_R[r_size] = v;

    int new_P[maxcount], new_X[maxcount];
    int new_p_size = 0, new_x_size = 0;

    for (int j = 0; j < p_size; j++) {
      if (graph[v][P[j]]) {
        new_P[new_p_size++] = P[j];
      }
    }

    for (int j = 0; j < x_size; j++) {
      if (graph[v][X[j]]) {
        new_X[new_x_size++] = X[j];
      }
    }

    bronkerbosch(graph, n, new_R, r_size + 1, new_P, new_p_size, new_X,
                 new_x_size, max_clique, max_size);

    P[i] = -1;       // Удаляем v из P
    X[x_size++] = v; // Добавляем v в X
  }
}

void insert(int *array, int *size, int value) {
  int i = *size - 1;
  while (i >= 0 && array[i] > value) {
    array[i + 1] = array[i];
    i--;
  }
  array[i + 1] = value;
  (*size)++;
}

int main() {
  FILE *file = fopen("./day23/input", "r");

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    maxcount += 1;
  }
  rewind(file);

  char nodes[maxcount][max_len];
  int graph[maxcount][maxcount];

  int nodecount = 0;

  while (fgets(line, sizeof(line), file)) {
    char a[max_len] = {0};
    char b[max_len] = {0};
    strncpy(a, line, 2);
    strncpy(b, line + 3, 2);

    add_connection(nodes, graph, a, b, &nodecount);
  }

  fclose(file);

  printf("star1 - %d\n", findloopswitht(graph, nodes, nodecount));

  // find largest clique using Bron–Kerbosch
  int R[maxcount], P[maxcount], X[maxcount];
  for (int i = 0; i < nodecount; i++) {
    P[i] = i;
  }
  int max_clique[maxcount];
  int max_size = 0;

  bronkerbosch(graph, nodecount, R, 0, P, nodecount, X, 0, max_clique,
               &max_size);


  char password[max_size][max_len];
  for (int i = 0; i < max_size; i++) {
    strncpy(password[i], nodes[max_clique[i]], max_len);
  }

  for (int i = 0; i < max_size - 1; i++) {
    for (int j = i + 1; j < max_size; j++) {
      if (strcmp(password[i], password[j]) > 0) {
        char temp[max_len];
        strncpy(temp, password[i], max_len);
        strncpy(password[i], password[j], max_len);
        strncpy(password[j], temp, max_len);
      }
    }
  }

  printf("star2 - ");
  for (int i = 0; i < max_size; i++) {
    if (i == max_size - 1) {
      printf("%s", password[i]);
    } else {
      printf("%s,", password[i]);
    }
  }
  printf("\n");

  return EXIT_SUCCESS;
}
