#include "grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int opt = 0;
  int fileAmount = 0;

  static struct Flags flags = {0, 0, 0, 0, 0, 0, 0, 0};
  opterr = 0;
  while ((opt = getopt(argc, argv, "eivclnhsfo")) != -1) {
    updateFlags(opt, &flags);
  }

  if ((flags.eFlag == 0) && (flags.iFlag == 0)) {
    flags.eFlag = 1;
  }

  fileAmount = argc - optind - 1;

  iterateThroughFiles(&flags, argc, argv, fileAmount);

  return 0;
}

void updateFlags(int opt, struct Flags *flags) {
  switch (opt) {
    case 'e':
      flags->eFlag = 1;
      break;
    case 'i':
      flags->iFlag = 1;
      break;
    case 'v':
      flags->vFlag = 1;
      break;
    case 'c':
      flags->cFlag = 1;
      break;
    case 'l':
      flags->lFlag = 1;
      break;
    case 'n':
      flags->nFlag = 1;
      break;
    case 'h':
      flags->hFlag = 1;
      break;
    case 's':
      flags->sFlag = 1;
      break;
    case '?':
      fprintf(stderr, "flag %c does not exist\n", opt);
      break;
  }
}

void iterateThroughFiles(struct Flags *flags, int argc, char *argv[],
                         int fileAmount) {
  for (int i = (optind + 1); i < argc; i++) {
    char template[256];
    strcpy(template, argv[optind]);

    FILE *file = fopen(argv[i], "r");

    if (file != NULL) {
      processFile(file, flags, template, argv, i, fileAmount);
      fclose(file);
    } else if (flags->sFlag == 0) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
    }
  }
}

void processFile(FILE *file, struct Flags *flags, char *template, char *argv[],
                 int fileIndex, int fileAmount) {
  int globalIsFound = 0;
  int isFound = 0;
  int count = 0;
  int lineCounter = 1;

  char *line = NULL;
  size_t len = 0;
  int curChar = 0;

  regex_t regexStruct;
  int regCompiled;
  regCompiled = regcomp(&regexStruct, template, REG_EXTENDED);

  while ((curChar = getline(&line, &len, file)) != -1) {
    EIFlags(flags, regCompiled, regexStruct, template, line, &isFound,
            &globalIsFound, &count);

    if (((flags->hFlag == 0) && (flags->cFlag == 0) && (flags->lFlag == 0) &&
         (fileAmount > 1) && (isFound == 1) && (flags->vFlag == 0))) {
      printf("%s:", argv[fileIndex]);
    } else if ((fileAmount > 1) && (flags->vFlag == 1) && (isFound == 0) &&
               (flags->cFlag == 0) && (flags->lFlag == 0) &&
               (flags->hFlag == 0)) {
      printf("%s:", argv[fileIndex]);
    }

    if ((flags->vFlag == 1) && (isFound == 0) && (flags->lFlag == 0) &&
        (flags->cFlag == 0)) {
      output(flags, line, lineCounter);
    } else if ((flags->vFlag == 0) && (isFound == 1) && (flags->cFlag == 0) &&
               (flags->lFlag == 0)) {
      output(flags, line, lineCounter);
    }

    isFound = 0;
    lineCounter++;
  }

  if ((flags->cFlag == 1) && (fileAmount > 1) && (flags->lFlag == 0) &&
      (flags->hFlag == 0)) {
    printf("%s:", argv[fileIndex]);
  }

  CLVFlags(flags, argv, fileIndex, count, lineCounter, globalIsFound);
  regfree(&regexStruct);
  free(line);
}

void EIFlags(struct Flags *flags, int regCompiled, regex_t regexStruct,
             char *template, char *line, int *isFound, int *globalIsFound,
             int *count) {
  if (flags->iFlag == 1) {
    regCompiled = regcomp(&regexStruct, template, REG_ICASE);
    regCompiled = regexec(&regexStruct, line, 0, NULL, REG_ICASE);
    if (regCompiled == 0) {
      *isFound = 1;
      *globalIsFound = 1;
      (*count)++;
    }
  } else if ((flags->eFlag == 1)) {
    regCompiled = regexec(&regexStruct, line, 0, NULL, 0);
    if (regCompiled == 0) {
      *isFound = 1;
      *globalIsFound = 1;
      (*count)++;
    }
  }
}

void CLVFlags(struct Flags *flags, char *argv[], int fileIndex, int count,
              int lineCounter, int globalIsFound) {
  if ((flags->cFlag == 1) && (flags->lFlag == 0)) {
    if (flags->vFlag == 0) {
      printf("%d\n", count);
    } else {
      printf("%d\n", lineCounter - count - 1);
    }
  }
  if ((flags->lFlag == 1) && (flags->vFlag == 1)) {
    printf("%s\n", argv[fileIndex]);
  } else if ((flags->lFlag == 1) && (globalIsFound == 1)) {
    printf("%s\n", argv[fileIndex]);
  }
}

void output(struct Flags *flags, char *line, int lineCounter) {
  if ((flags->nFlag == 1) && (flags->hFlag == 1)) {
    printf("%d:%s", lineCounter, line);
  } else if (flags->hFlag == 1) {
    printf("%s", line);
  } else if (flags->nFlag == 1) {
    printf("%d:%s", lineCounter, line);
  } else {
    printf("%s", line);
  }
}