#include <stdio.h>
#include <string.h>

#define INDEXLEN 16   // Bit Length of Index
#define WAYLEN 2      // Bit Length of Way Size
#define BLOCKLEN 2    // Bit Length of Block Size

typedef unsigned char Byte;
typedef unsigned char bool;
typedef struct Cache {
  bool valid;
  unsigned long tag;
};

long MISS;
long HIT;
struct Cache cache[1<<WAYLEN][1<<INDEXLEN];
int cur[1<<INDEXLEN];
int init() {
  MISS = 0;
  HIT = 0;
  int i, j;
  for (i = 0; i < 1<<INDEXLEN; i++) {
    cur[i] = 0;
    for(j = 0; j < 1<<WAYLEN; j++) {
      cache[j][i].valid = 0;
      cache[j][i].tag = 0;
    }
  }
}
int calAddr(unsigned long addr, unsigned long *tag, unsigned long *idx,
            unsigned long *offset) {
  unsigned long tmp;
  int i;
  *tag = addr >> (INDEXLEN + BLOCKLEN);
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

  bool isHit = 0;

  for(int i=0; i<(1<<WAYLEN); i++) {
    if(cache[i][idx].valid && cache[i][idx].tag == tag) {
      isHit = 1;
    }
  }

  if (isHit) {
    HIT++;
  } else {
    MISS++;

    cache[cur[idx]][idx].valid = 1;
    cache[cur[idx]][idx].tag = tag;

    cur[idx] = (cur[idx] + 1) % (1<<WAYLEN);
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
