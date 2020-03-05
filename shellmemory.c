#include <string.h>
#include <stdio.h>
#include "shellmemory.h"

struct MEM{
	char *var;
	char *val;
};

struct MEM dict[1000];

int getShell(char *var, char **dest){
    int shellPos;
	for (shellPos=0; shellPos<1000 && dict[shellPos].var; shellPos++){
		if (!strcmp(dict[shellPos].var, var)){
			if (!dest) return shellPos+1;
            char *destp=*dest;
            strcpy(destp,(dict[shellPos].val));
            return shellPos+1;
		}
	}
    return shellPos-1000;
}

int addShell(char *var, char *val){
	int shellPos=getShell(var, NULL);
    if (!shellPos) return 0;
	if (shellPos>0) dict[shellPos-1].val=strdup(val);
    else {
        dict[shellPos+1000].var=strdup(var);
        dict[shellPos+1000].val=strdup(val);
    }
	return 1;
	}
