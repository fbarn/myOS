#include <stdio.h>
int addToRAM(FILE *p, int *start, int *end);
int getFromRAM(int PC, char **dest);
void memFree(int start, int end); 
