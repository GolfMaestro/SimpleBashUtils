#include "cat.h"

#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int opt = 0;

  static struct option long_options[] = {
      {"number", no_argument, 0, 'n'},
      {"number-nonblank", no_argument, 0, 'b'},
      {"squeeze-blank", no_argument, 0, 's'}};

  static struct Flags flags = {0, 0, 0, 0, 0, 0};

  while ((opt = getopt_long(argc, argv, "benstvET", long_options, NULL)) !=
         -1) {
    updateFlags(opt, &flags);
  }

  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");

    if (file == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[i]);
    } else {
      processFile(file, &flags);
      fclose(file);
    }
  }

  return 0;
}

void updateFlags(int opt, struct Flags *flags) {
  switch (opt) {
    case 'b':
      flags->bFlag = 1;
      break;
    case 'e':
      flags->eFlag = 1;
      flags->vFlag = 1;
      break;
    case 'n':
      flags->nFlag = 1;
      break;
    case 's':
      flags->sFlag = 1;
      break;
    case 't':
      flags->tFlag = 1;
      flags->vFlag = 1;
      break;
    case 'v':
      flags->vFlag = 1;
      break;
    case 'E':
      flags->eFlag = 1;
      break;
    case 'T':
      flags->tFlag = 1;
      break;
  }
}

void processFile(FILE *file, struct Flags *flags) {
  int c = 0;

  if ((flags->bFlag == 0) && (flags->eFlag == 0) && (flags->nFlag == 0) &&
      (flags->sFlag == 0) && (flags->tFlag == 0) && (flags->vFlag == 0)) {
    while ((c = fgetc(file)) != EOF) {
      putchar(c);
    }
  } else {
    c = fgetc(file);
    int prevC = 0;
    int countLines = 1;

    if ((flags->eFlag == 1) && (c == '\n') && (flags->nFlag == 0)) {
      printf("$");
    }

    if ((flags->bFlag == 1) && (c != '\n')) {
      printf("     %d\t", countLines);
      countLines++;
    } else if ((flags->nFlag == 1) && (flags->bFlag == 0)) {
      printf("     %d\t", countLines);
      countLines++;
    }

    if ((flags->tFlag == 1) && (c == '\t')) {
      printf("^I");
    }

    int temp = 0;
    if (flags->vFlag == 1) {
      temp = 0;
      temp = processVFlag(c);
      if (temp == 0) {
        putchar(c);
      }
    } else {
      putchar(c);
    }

    prevC = c;

    int emptyLines = 0;

    while ((c = fgetc(file)) != EOF) {
      processFlags(c, prevC, *flags, &emptyLines, &countLines);

      prevC = c;
    }
  }
}

int processVFlag(int c) {
  int didIPrintSomething = 0;

  if ((c < 32) && (c != '\n') && (c != '\t')) {
    printf("^%c", c + 64);
    didIPrintSomething = 1;
  } else if (c == 127) {
    printf("^?");
    didIPrintSomething = 1;
  }

  return didIPrintSomething;
}

void processFlags(int c, int prevC, struct Flags flags, int *emptyLines,
                  int *countLines) {
  if (flags.sFlag == 1) {
    if ((prevC == '\n') && (c == '\n')) {
      (*emptyLines)++;
    } else {
      *emptyLines = 0;
    }
  }

  if ((flags.bFlag == 1) && (prevC == '\n') && (c != '\n')) {
    int currentNumberWidth = numberWidth(*countLines);
    for (int i = 6; i > currentNumberWidth; i--) {
      printf(" ");
    }
    printf("%d\t", *countLines);
    (*countLines)++;
  } else if ((flags.nFlag == 1) && (prevC == '\n') && (flags.bFlag != 1) &&
             (!((flags.sFlag == 1) && (*emptyLines >= 2)))) {
    int currentNumberWidth = numberWidth(*countLines);
    for (int i = 6; i > currentNumberWidth; i--) {
      printf(" ");
    }
    printf("%d\t", *countLines);
    (*countLines)++;
  }

  if ((flags.eFlag == 1) && (c == '\n') && (*emptyLines < 2)) {
    printf("$");
  }

  if ((flags.tFlag == 1) && (c == '\t')) {
    printf("^I");
  }

  if (flags.vFlag == 1) {
    int tempIn = 0;
    tempIn = processVFlag(c);
    if ((tempIn == 0) && (!(flags.tFlag == 1 && c == '\t')) &&
        (!((flags.sFlag == 1) && (*emptyLines >= 2)))) {
      putchar(c);
    }
  } else if ((flags.sFlag == 1) && (*emptyLines >= 2)) {
    ;
  } else if ((flags.tFlag == 1) && (c == '\t')) {
    ;
  } else {
    putchar(c);
  }
}

int numberWidth(int number) {
  int width = 1;

  number = number / 10;

  while (number != 0) {
    number = number / 10;
    width++;
  }

  return width;
}