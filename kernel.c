#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "pcb.h"
#include "ram.h"
#include "shell.h"

#define QUANTA 2

struct readyQueue{
    struct readyNode *head, *tail;
};

struct readyNode{
    struct PCB *val;
    struct readyNode *next;
};

struct readyQueue myReadyQueue={NULL, NULL};

int main(){
    return shellUI();
}

void delHead(){
    struct readyNode *temp;
    temp = myReadyQueue.head;
    myReadyQueue.head=temp->next;
    free(temp);
}

int addToReady(struct PCB *myPCBp){
    if(!myPCBp) return 0;
    struct readyNode *myNode=(struct readyNode *) malloc(sizeof(struct readyNode));
    myNode->val=myPCBp;
    myNode->next=NULL;
    if (!myReadyQueue.head){
        myReadyQueue.head=myNode;
        myReadyQueue.tail=myNode;
    }
    else{
        myReadyQueue.tail->next=myNode;
        myReadyQueue.tail=myNode;
    }
    return 1;
}

void clearAll(){
    memFree(0, 1000);
    while (myReadyQueue.head){
        free(myReadyQueue.head->val);
        delHead();
    }
    freeCPU();
}

int scheduler(){
    if(!CPUisFree()) return 1;
    while (myReadyQueue.head){
        int start=getStartPCB(myReadyQueue.head->val);
        int end=getEndPCB(myReadyQueue.head->val);
        int PC=incPCPCB(myReadyQueue.head->val, 0);
        setIP(PC);
        if (end-PC<=QUANTA){
            int ret=runCPU(end-PC);
            memFree(start, end);
            free(myReadyQueue.head->val);
            delHead();
            freeCPU();
            if (!ret){
                clearAll();
                return 0;
            }
        }
        else{
            int ret=runCPU(QUANTA);
            if (!ret){
                clearAll();
                return 0;
            }
            if (ret==2){
                memFree(start, end);
                free(myReadyQueue.head->val);
                delHead();
                freeCPU();
            }
            else{
                incPCPCB(myReadyQueue.head->val, QUANTA);
                if (!addToReady(myReadyQueue.head->val)) return 0;
                myReadyQueue.head=myReadyQueue.head->next;
            }
        }
    }
    myReadyQueue.tail=NULL;
    return 1;
}

int myinit(char *filename){
    FILE *fp;
    int start, end;
    fp=fopen(filename, "r");
    if (!fp) return 0;
    if (!addToRAM(fp, &start, &end)){
        clearAll();
        return -1;
    }
    fclose(fp);
    struct PCB *myPCBp=makePCB(start, end);
    if(!addToReady(myPCBp)){
        clearAll();
        return -2;
    }
    return 1;
}
