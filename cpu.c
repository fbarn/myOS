#include "interpreter.h"
#include "ram.h"
#include <string.h>

struct CPU{
    int IP;
    char IR[1000];
    int quanta;
};

struct CPU myCPU={0, 0, 0};

void setIP(int PC){
    myCPU.IP=PC;
}

int CPUisFree(){
    return !strcmp(myCPU.IR, "\0") && !myCPU.quanta;
}

void freeCPU(){
    myCPU.IR[0]='\0';
    myCPU.quanta=0;
}

int runCPU(int quanta){
    int ret=1;
    for(myCPU.quanta=quanta; myCPU.quanta>0; myCPU.quanta--){
        char *IRp=myCPU.IR;
        if (!getFromRAM(myCPU.IP, &IRp)) ret=0;
        int result=interpreter(myCPU.IR);
        if (result!=1 && result) ret=0;
        if (!result){
            myCPU.quanta=0;
            return 2;
        }
        myCPU.IP+=1;
    }
    return ret;
}
