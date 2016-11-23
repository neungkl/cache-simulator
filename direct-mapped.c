#include <stdio.h>
#include <string.h>

#define INDEXLEN 10		// Bit Length of Index
#define BLOCKLEN 2		// Bit Length of Block Size

typedef unsigned char Byte;
typedef unsigned char bool;
typedef struct Cache {
  bool valid;
  unsigned long tag;
};

long MISS;
long HIT;
struct Cache cache[1<<INDEXLEN];
int init() {
  MISS = 0;
  HIT = 0;
  int i;
  for (i = 0; i < 1<<INDEXLEN; i++) {
    cache[i].valid = 0;
    cache[i].tag = 0;
  }
}
int calAddr(unsigned long addr, unsigned long *tag, unsigned long *idx,
            unsigned long *offset) {
  unsigned long tmp;
  int i;
  *tag = addr >> (BLOCKLEN + INDEXLEN);
  tmp = 0;
  for (i = 0; i < INDEXLEN; i++) {
    tmp <<= 1;
    tmp |= 0x01;
  }
  *idx = addr >> BLOCKLEN & tmp;
  tmp = 0;
  for (i = 0; i < BLOCKLEN; i++) {
    tmp <<= 1;
    tmp |= 0x01;
  }
  *offset = addr & tmp;
}
int access(unsigned long addr) {
  unsigned long offset;
  unsigned long tag;
  unsigned long idx;
  int i;
  int valid = 0;
  calAddr(addr, &tag, &idx, &offset);

  bool isHit = 1;

  if (!cache[idx].valid) isHit = 0;
  if (cache[idx].tag != tag) isHit = 0;

  if (isHit) {
    HIT++;
  } else {
    MISS++;
    cache[idx].valid = 1;
    cache[idx].tag = tag;
  }
}
int main(int argc, char *argv[]) {
  FILE *input;
  char buff[1025];
  unsigned long myaddr;
  if (argc < 2) {
    fprintf(stderr, "Please specify address file\n");
    exit(-1);
  }
  input = fopen(argv[1], "r");
  // read file
  while (fgets(&buff[0], 1024, input)) {
    sscanf(buff, "0x%x", &myaddr);
    access(myaddr);
  }
  printf("HIT:%7d MISS: %7d\n", HIT, MISS);
}
