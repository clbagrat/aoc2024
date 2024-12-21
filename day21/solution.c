#include <stdio.h>
#include <stdlib.h>


char numpad[4][4] = {"789", "456", "123", ".0A"};

char dirpad[4][4] = {".^A", "<v>", "...", "..."};

//        sy sx ey ex  inception
long costs[4][4][4][4][26];

long min(long a, long b) {
  if (a < b) {
    return a;
  }
  return b;
}
void setindexes(char ch, char pad[4][4], int *y, int *x) {
  *y = -1;
  *x = -1;
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (pad[r][c] == ch) {
        *y = r;
        *x = c;
      }
    }
  }
}

long getcost(char start, char end, char pad[4][4], int inception) {
  if (inception == 26) {
    return 1;
  }

  int sy, sx, ey, ex;

  setindexes(start, pad, &sy, &sx);
  setindexes(end, pad, &ey, &ex);

  if (costs[sy][sx][ey][ex][inception] != -1) {
    return costs[sy][sx][ey][ex][inception];
  }

  long res = __LONG_MAX__;
  // horisontal to vertical
  long h2v = 0;
  // to avoid empty space marked .
  // if there is an empty space of horisontal intersection
  // then there is no point to start horisontally
  if (pad[sy][ex] != '.') {
    char lastbeforecorner = 'A';
    char lastaftercorner;
    // here we decide is we need to go left or right
    if (ex < sx) { // go left
      h2v += getcost('A', '<', dirpad, inception + 1);
      h2v += getcost('<', '<', dirpad, inception + 1) * (sx - ex - 1);
      lastbeforecorner = '<';
    }

    if (ex > sx) { // go right
      h2v += getcost('A', '>', dirpad, inception + 1);
      h2v += getcost('>', '>', dirpad, inception + 1) * (ex - sx - 1);
      lastbeforecorner = '>';
    }

    lastaftercorner = lastbeforecorner;
    // now we decide top or bottom
    if (ey < sy) { // go top
      h2v += getcost(lastbeforecorner, '^', dirpad, inception + 1);
      h2v += getcost('^', '^', dirpad, inception + 1) * (sy - ey - 1);
      lastaftercorner = '^';
    }
    if (ey > sy) { // go bot
      h2v += getcost(lastbeforecorner, 'v', dirpad, inception + 1);
      h2v += getcost('v', 'v', dirpad, inception + 1) * (ey - sy - 1);
      lastaftercorner = 'v';
    }

    // always coming back
    h2v += getcost(lastaftercorner, 'A', dirpad, inception + 1);
    res = min(res, h2v);
  }

  // now the same but for vertical then horisonatal
  long v2h = 0;
  if (pad[ey][sx] != '.') {
    char lastbeforecorner = 'A';
    char lastaftercorner;
    if (ey < sy) {
      v2h += getcost('A', '^', dirpad, inception + 1);
      v2h += getcost('^', '^', dirpad, inception + 1) * (sy - ey - 1);
      lastbeforecorner = '^';
    }
    if (ey > sy) {
      v2h += getcost('A', 'v', dirpad, inception + 1);
      v2h += getcost('v', 'v', dirpad, inception + 1) * (ey - sy - 1);
      lastbeforecorner = 'v';
    }
    lastaftercorner = lastbeforecorner;
    if (ex < sx) {
      v2h += getcost(lastbeforecorner, '<', dirpad, inception + 1);
      v2h += getcost('<', '<', dirpad, inception + 1) * (sx - ex - 1);
      lastaftercorner = '<';
    }
    if (ex > sx) {
      v2h += getcost(lastbeforecorner, '>', dirpad, inception + 1);
      v2h += getcost('>', '>', dirpad, inception + 1) * (ex - sx - 1);
      lastaftercorner = '>';
    }
    v2h += getcost(lastaftercorner, 'A', dirpad, inception + 1);
    res = min(res, v2h);
  }

  costs[sy][sx][ey][ex][inception] = res;
  return res;
}

int main() {
  FILE *file = fopen("./day21/input", "r");

  // chatgpt - how to generate a 5d array of ints in c
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        for (int l = 0; l < 4; l++) {
          for (int m = 0; m < 26; m++) {
            costs[i][j][k][l][m] = -1;
          }
        }
      }
    }
  }
  char line[256];

  long overall = 0;
  while (fgets(line, sizeof(line), file)) {
    char start = 'A';
    long cost = 0;
    for (int i = 0; i < 4; i += 1) {
      cost += getcost(start, line[i], numpad, 0);
      start = line[i];
    }

    printf("%lu\n", cost);
    overall += cost * atoi(line);
  }

  printf("res %lu", overall);
  // printf("%lu", getcost('A', '0', numpad, 0));

  return EXIT_SUCCESS;
}

// <p
// v<<p>>^p
// v<p<pp>>^pvpp<^p>p
// v<p<p>>^pv<<p>>^ppvpp<^p>pv<p>^ppv<<p>^p>pvp^p
//
// <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A
// v<<A>>^A<A>AvA<^AA>A<vAAA>^A
// <A^A>^^AvvvA
