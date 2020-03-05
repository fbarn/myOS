CC=gcc -g 

all : cpu.o interpreter.o kernel.o pcb.o shell.o shellmemory.o ram.o
	$(CC) cpu.o interpreter.o kernel.o pcb.o ram.o shellmemory.o shell.o -o mykernel

cpu.o : cpu.c 
	$(CC) -c cpu.c 

interpreter.o : interpreter.c 
	$(CC) -c interpreter.c 

kernel.o : kernel.c 
	$(CC) -c kernel.c 

pcb.o : pcb.c
	$(CC) -c pcb.c 

ram.o : ram.c 
	$(CC) -c ram.c 

shell.o : shell.c 
	$(CC) -c shell.c 

shellmemory.o : shellmemory.c 
	$(CC) -c shellmemory.c 

.PHONY:
clean:
	rm -f cpu.o interpreter.o kernel.o pcb.o ram.o shell.o shellmemory.o
	rm -f mykernel
