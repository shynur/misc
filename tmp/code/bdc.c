/*
  The Brain-Dead Calculator.
  Build:
  gcc -g -O -Wall -o bdc bdc.c -lm -lreadline -lcurses
  Run:
  ./bdc    or   ./bdc path-to-command-file   or   path-to-exe-bdc-command-file
  Example commands:
  3 q c -2.15 * s +     # calculates sqrt(3) + sin(-2.15*sqrt(3))
  ?                     # prints help message
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <math.h>
#include <string.h>

#define MAXLEVS 12
int levs = 8;
double st[MAXLEVS];

int do_file(char *path);

int parse(char *comm) {
  int cc, pop, ii, ps,
    cp = 0, cp0,
    cl = strlen(comm);
  double rollin;
  char *endp, *comfile;

  if (cl == 0)
    return 0;
  
  do {
    rollin = 0.0;
    pop = 1,
      cc = comm[cp];
    
    switch (cc) {
      default:
        if ((cc >= '0') && (cc <= '9'))
          goto num;
        printf("\nERROR at input position %d\n", cp);
        pop = 5;
        break;
        
      case '#':
        pop = 3;
        break;

      case '?':
        printf("\nBrain-Dead Calculator with %d-level stack;  x=quit\n", levs);
        printf("op=  + - * /   perform arithmetic:"
               "   st[1] = st[1] op st[0]   and shift st[j-1] <-- st[j]\n"
               "op=   s  q     perform functions:"
               "  st[0] = func(st[0])  s= sine  q= square-root\n"
               "op=   d c r    manipulate stack:"
               " d= discard st[0];  c= copy st[0];  r= rotate st[j-1] <-- st[j]\n"
               "op=   &path    run the command file specified by path\n");

      case ' ':
        pop = 0;
        break;

      case '+':
        if ((comm[cp + 1] >= '0') && (comm[cp + 1] <= '9')) {
        num:
          memmove(st + 1, st, (levs - 1) * sizeof(double));
          st[0] = strtod(comm + cp, &endp);
          cp += (endp - (comm + cp)) - 1,
            pop = 0;
          break;
        }
        st[1] += st[0];
        break;

      case '-':
        if ((comm[cp + 1] >= '0') && (comm[cp + 1] <= '9'))
          goto num;
        st[1] -= st[0];
        break;

      case '.':
        goto num;

      case '*':
        st[1] *= st[0];
        break;

      case '/':
        st[1] /= st[0];
        break;

      case 'q':
        st[0] = sqrt(st[0]);
        pop = 0;
        break;

      case 's':
        st[0] = sin(st[0]);
        pop = 0;
        break;

      case 'd':
        break;

      case 'c':
        memmove(st + 1, st, (levs - 1) * sizeof(double));
        pop = 0;
        break;

      case 'r':
        pop = 2;
        break;

      case '&':
        ii = 0,
          cp += 1,
          cp0 = cp;
        while (comm[cp] > ' ') 
          cp += 1,
            ii += 1;
        comfile = malloc(ii + 1);
        memcpy(comfile, comm + cp0, ii);
        comfile[ii] = 0;
        ps = do_file(comfile);
        free(comfile);
        pop = 0;
        break;
        
      case 'x':
        pop = 4;
        break;
    }
    
    switch (pop) {
      case 0:
        break;

      case 2:
        rollin = st[0];

      case 1:
        memmove(st, st + 1, (levs - 1) * sizeof(double));
        st[levs - 1] = rollin;
        break;

      default:
        return pop;
    }
  } while (++cp < cl);

  return 0;
}

int do_file(char *path) {
  FILE *do_fb;
  char *comm;
  ssize_t len;
  size_t len1;
  int ps, ii, comcount = 0;

  do_fb = fopen(path, "r");
  if (do_fb == 0) {
    perror("do_file: cannot open");
    return 0;
  }

  do {
    comm = NULL;
    len = getline(&comm, &len1, do_fb);

    if (len == -1) {
      ps = 0;
      break;
    }
    
    comm[len - 1] = 0;
    ps = parse(comm);
    free(comm);
    ++comcount;
  } while ((ps != 4) && (ps != 5));

  fclose(do_fb); 
  printf("file %d st=", comcount);

  for (ii = 0; ii < levs; ++ii)
    printf("  %g", st[ii]);

  puts("");

  return ps;
}

int main(int argc, char *argv[]) {
  char *comm;
  int ii, ps, comcount = 0;

  if (argc > 1) {
    ps = do_file(argv[1]);

    if (ps == 4)
      return 0;

    if (ps == 5)
      return 1;
  }
  
  do {
    do {
      comm = readline("bdc> ");
    } while (comm == NULL);

    add_history(comm);
    ps = parse(comm);
    free(comm);
    printf("comm %d st=", ++comcount);

    for (ii = 0; ii < levs; ++ii)
      printf("  %g", st[ii]);

    puts("");
  } while (ps != 4);

  return 0;
}
