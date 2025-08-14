#include <regex.h>
#include <stdio.h>

struct Flags {
  int eFlag;
  int iFlag;
  int vFlag;
  int cFlag;
  int lFlag;
  int nFlag;
  int hFlag;
  int sFlag;
};

void updateFlags(int opt, struct Flags *flags);

void iterateThroughFiles(struct Flags *flags, int argc, char *argv[],
                         int fileAmount);

void processFile(FILE *file, struct Flags *flags, char *template, char *argv[],
                 int fileIndex, int fileAmount);

void EIFlags(struct Flags *flags, int regCompiled, regex_t regexStruct,
             char *template, char *line, int *isFound, int *globalIsFound,
             int *count);

void CLVFlags(struct Flags *flags, char *argv[], int fileIndex, int count,
              int lineCounter, int globalIsFound);

void output(struct Flags *flags, char *line, int lineCounter);