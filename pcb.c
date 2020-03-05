#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

struct PCB{
	int PC;
	int start;
	int end;
};

struct PCB *makePCB(int start, int end){
    struct PCB myPCB={start, start, end};
    struct PCB *myPCBp=(struct PCB *) malloc(sizeof(struct PCB));
    *myPCBp=myPCB;
    return myPCBp;
}

void freePCB(struct PCB *myPCB){
    free(myPCB);
}

int getStartPCB(struct PCB *myPCB){
    return myPCB->start;
}

int getEndPCB(struct PCB *myPCB){
    return myPCB->end;
}

int incPCPCB(struct PCB *myPCB, int inc){
    myPCB->PC+=inc;
    return myPCB->PC;
}
