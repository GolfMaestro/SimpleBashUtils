#include <stdio.h>

struct Flags {
  int bFlag;
  int eFlag;
  int nFlag;
  int sFlag;
  int tFlag;
  int vFlag;
};

void processFile(FILE *file, struct Flags *flags);

void updateFlags(int opt, struct Flags *flags);

int processVFlag(int c);

void processFlags(int c, int prevC, struct Flags flags, int *emptyLines,
                  int *countLines);

int numberWidth(int number);