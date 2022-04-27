CC = mpicc
CFLAGS = -O2

LDFLAGS = -L/Users/sebastianamundsen/Desktop/IN4200/IN4200_High_Performane_Computing/Oblig_2/parallel_code/simple-jpeg -lsimplejpeg

PROJ = parallel_main

all: simple-jpeg $(PROJ)

serial_main: parallel_main.c parallel_functions.c
	$(CC) $(CFLAGS) -o $@ parallel_main.c parallel_functions.c $(LDFLAGS)

simple-jpeg:
	$(MAKE) -C ../../simple-jpeg
