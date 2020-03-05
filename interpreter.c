#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shellmemory.h"
#include "interpreter.h"
#include "kernel.h"
#include "ram.h"

int split(char *input, char *(*words)[]){
	int inInput=0;
	int inTmp=0;
	int wordcount=0;
	char tmp[200];
	char **wordsp=*words;
	while (inInput<1000) {
		if (input[inInput]<33){
			if (inTmp>0){
				tmp[inTmp]='\0';
				wordsp[wordcount]=strdup(tmp);
				wordcount++;
			}
			if(input[inInput]!=32 && input[inInput]!=9) break;
			inTmp=0;
		}
		else{
			tmp[inTmp]=input[inInput];
			inTmp++;
		}
		inInput++;
	}
	return wordcount-1;
}

int quit(int nargs){
	if (nargs!=0){
		printf("Function \"quit\" takes no arguments.\n");
		return 2;
	}
	printf("Bye!\n");
	return 0;
}

int help(int nargs){
	if (nargs!=0){
	       printf("Function \"help\" takes no argumants.\n");
	       return 2;
	}
	printf("help\t\t\tDisplays all the commands.\n");
	printf("quit\t\t\tExits / terminates the shell with “Bye!”\n");
	printf("set VAR STRING\t\tAssigns a value to shell memory\n");
	printf("print VAR\t\tDisplays the STRING assigned to VAR\n");
	printf("run SCRIPT.txt\t\tExecutes the file SCRIPT.TXT \n");
	return 1;
}

int set(int nargs, char *var, char* val[]){
	if (nargs<2){ 
		printf("Function \"set\" takes two argumants.\n");
		return 2;
	}
    char tmp[1000]="";
    for (int wordPos=0; wordPos<nargs-1; wordPos++){
        strcat(tmp, val[wordPos]);
        strcat(tmp, " ");
    }
    if (!addShell(var, tmp)){
        printf("Out of shell memory. Must overwrite an already assigned variable.\n");
        return 2;
    }
	return 1;
}

int print(int nargs, char *var){
	char *dest=strdup("");
	if (nargs!=1){
	       	printf("Function \"print\" takes one argument.\n");
		return 2;
	}
    if (getShell(var, &dest)<=0){
           printf("Variable does not exist.\n");
           return 2;
    }
    else printf("%s\n", dest);	return 1;
}

int run(int nargs, char *script){
	FILE *fp;
	char line[1000];
	int len = 1000;
	int ret=1;
	if (nargs!=1){
		printf("Function \"run\" takes one argument.\n");
		return 2;
	}
    fp=fopen(script, "r");
    if (!fp){
        printf("Script not found.\n");
        return 2;
    }
    for (char* command=fgets(line, len, fp); command && ret==1; command=fgets(line, len, fp)){
        ret=interpreter(line);
    }
	fclose(fp);
	return 1;
}

int exec(int nargs, char *script[]){
	if (nargs<1 || nargs>3){
		printf("Function \"exec\" takes at least one and at most three arguments.\n");
		return 2;
	}
    for (int procid=0; procid<nargs; procid++){
        for (int i=0; i<procid; i++){
            if (!strcmp(script[i], script[procid])){
                printf("Error: script %s already loaded\n", script[procid]);
                clearAll();
                return 2;
            }
        }
        int init=myinit(script[procid]);
        if (init!=1){
            if (!init) printf("Program %s not found.\n", script[procid]);
            if (init==-1) printf("Could not load the program %s to RAM.\n", script[procid]);
            if (init==-2) printf("Could not add the program %s to the Ready Queue.\n", script[procid]);
            return 2;
        }
    }
    if (!scheduler()){
        printf("A scheduling error has occured.\n");
        return 2;
    }
	return 1;
}

int interpreter(char *input){
	char *words[100]={0};
	int nargs=split(input, &words);
	char **args=&words[1];
	if (nargs<0) return 1;
	if (!strcmp(words[0], "quit")) return quit(nargs);
	if (!strcmp(words[0], "help")) return help(nargs);
	if (!strcmp(words[0], "set")) return set(nargs, words[1], words+2);
	if (!strcmp(words[0], "print")) return print(nargs, words[1]);
	if (!strcmp(words[0], "run")) return run(nargs, words[1]);
	if (!strcmp(words[0], "exec")) return exec(nargs, words+1);
	printf("Unknown command.\n");
	return 1;
}
