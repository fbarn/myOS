#include <string.h>
#include <stdio.h>

char *ram[1000];

int findAvail(int start){
    int ramPos;
    for (ramPos=start; ramPos<1000 && ram[ramPos]; ramPos++);
    if (ramPos>999) return -1;
    return ramPos;
}

void memFree(int start, int end){
    for (int cell=start; cell<end; cell++){
        ram[cell]=NULL;
    }
}

int moveLines(int start, int end, int dest){
    int destCell=dest;
    for (int cell=start; cell<end; cell++, destCell++){
        if (destCell>999) return -1;
        if (ram[destCell]){
            memFree(dest, destCell);
            cell=start;
        }
        ram[destCell]=strdup(ram[cell]);
    }
    memFree(start, end);
    return destCell;
}

int addToRAM(FILE *p, int *start, int *end){
	char line[1000];
	int len = 1000;
    int ramPos;
    for (ramPos=0; ramPos<1000 && ram[ramPos]; ramPos++);
    *start=ramPos;
    for (char* command=fgets(line, len, p); command; command=fgets(line, len, p)){
        if (ramPos>999 || ramPos<0) return 0;
        if (ram[ramPos]){
            int tmp=*start;
            *start=findAvail(ramPos);
            ramPos=moveLines(tmp, ramPos, *start);
        }
        else{
        ram[ramPos]=strdup(line);
        ramPos+=1;
        }
    }
    *end=ramPos;
    return 1;
}

int getFromRAM(int PC, char **dest){
    if (PC>=0 && PC<1000){
        char *destp=*dest;
        strcpy(destp,(ram[PC]));
        return 1;
    }
    return 0;
}
