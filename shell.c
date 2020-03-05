#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

int shellUI(){
	printf("Welcome to the Filippo Baronello shell!\n");
	printf("Version 2.0 Created February 2020\n");
	char input[1000];
	int run=1;
	while (run) {
		fprintf(stdout, "$ ");
		fflush(stdout);
		fgets(input, 999, stdin);
		run = interpreter(input);
	}
	return 1;
}
